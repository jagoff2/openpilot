import copy
import inspect
from collections import deque

import numpy as np

from cereal import log
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.controls.lib.drive_helpers import get_curvature_from_plan, smooth_value
from openpilot.selfdrive.modeld.constants import ModelConstants, Plan
from openpilot.selfdrive.modeld.lane_centering import (
  CAMERA_OFFSET,
  EDGE_ACQUIRE_TIME,
  ENTRY_CONFIDENCE_GRACE_TIME,
  ENTRY_TIME,
  ENTRY_WINDOW_TIME,
  HOLD_MIN_LANE_WIDTH,
  INVALID_GRACE_TIME,
  LaneCenteringController,
  MAX_LANE_PATH_ACCEL,
  MAX_LANE_PATH_JERK,
  MAX_LATERAL_ACCEL_CORRECTION,
  NOMINAL_MERGE_TIME,
  POLICY_ZERO_AGREEMENT,
  RAMP_IN_TIME,
  get_lane_centering_input_status,
)
from openpilot.selfdrive.modeld import modeld as modeld_module


ACTION_T = 0.435053849
PLAN_SPEED = 19.2
TIME_IDXS = np.asarray(ModelConstants.T_IDXS, dtype=np.float64)
MODEL_X = np.asarray(ModelConstants.X_IDXS, dtype=np.float64)


def make_model_output(left_y=-1.84, right_y=1.76, plan_y=0.0, plan_speed=PLAN_SPEED):
  n = ModelConstants.IDX_N
  lane_lines = np.zeros((1, 4, n, 2), dtype=np.float32)
  lane_lines[0, 0, :, 0] = np.asarray(left_y) - 3.5
  lane_lines[0, 1, :, 0] = left_y
  lane_lines[0, 2, :, 0] = right_y
  lane_lines[0, 3, :, 0] = np.asarray(right_y) + 3.5
  lane_stds = np.full_like(lane_lines, 0.05)

  road_edges = np.zeros((1, 2, n, 2), dtype=np.float32)
  road_edges[0, 0, :, 0] = left_y
  road_edges[0, 1, :, 0] = right_y
  road_edge_stds = np.full_like(road_edges, 0.40)

  lane_probs = np.zeros((1, 8), dtype=np.float32)
  lane_probs[0, 1::2] = 0.95

  plan = np.zeros((1, n, ModelConstants.PLAN_WIDTH), dtype=np.float32)
  plan[0, :, Plan.POSITION.start] = plan_speed * TIME_IDXS
  plan[0, :, Plan.POSITION.start + 1] = plan_y
  plan[0, :, Plan.VELOCITY.start] = plan_speed

  return {
    "lane_lines": lane_lines,
    "lane_lines_stds": lane_stds,
    "lane_lines_prob": lane_probs,
    "road_edges": road_edges,
    "road_edges_stds": road_edge_stds,
    "plan": plan,
  }


def make_centerline_output(center_y=0.0, width=3.60, plan_y=None, plan_speed=PLAN_SPEED):
  center_y = np.broadcast_to(np.asarray(center_y, dtype=np.float32), (ModelConstants.IDX_N,))
  camera_center_y = center_y - CAMERA_OFFSET
  if plan_y is None:
    plan_y = np.interp(plan_speed * TIME_IDXS, MODEL_X, center_y)
  return make_model_output(
    left_y=camera_center_y - 0.5 * width,
    right_y=camera_center_y + 0.5 * width,
    plan_y=plan_y,
    plan_speed=plan_speed,
  )


def plan_curvature(model_output, v_ego=20.0):
  plan = model_output["plan"][0]
  return float(get_curvature_from_plan(
    plan[:, Plan.T_FROM_CURRENT_EULER.start + 2],
    plan[:, Plan.ORIENTATION_RATE.start + 2],
    ModelConstants.T_IDXS,
    v_ego,
    ACTION_T,
  ))


def set_plan_curvature(model_output, curvature, speed):
  model_output["plan"][0, :, Plan.T_FROM_CURRENT_EULER.start + 2] = curvature * speed * TIME_IDXS
  model_output["plan"][0, :, Plan.ORIENTATION_RATE.start + 2] = curvature * speed
  return model_output


def step(controller, model_output, frames=1, v_ego=20.0, current_curvature=0.0, frame_dt=DT_MDL, **kwargs):
  result = None
  for _ in range(frames):
    previous_base = getattr(controller, "_test_previous_base_curvature", 0.0)
    previous_selected = getattr(controller, "_test_previous_selected_curvature", 0.0)
    raw_base = plan_curvature(model_output, v_ego)
    base_smoothed = smooth_value(raw_base, previous_base, 0.1) if v_ego > 0.3 else previous_base
    result = controller.update(
      model_output,
      v_ego,
      current_curvature,
      ACTION_T,
      frame_dt,
      base_smoothed,
      previous_selected,
      kwargs.get("lat_active", True),
      kwargs.get("model_valid", True),
      kwargs.get("left_blinker", False),
      kwargs.get("right_blinker", False),
      kwargs.get("lane_change_active", False),
    )
    raw_selected = plan_curvature(result[0], v_ego)
    selected_smoothed = smooth_value(raw_selected, previous_selected, 0.1) if v_ego > 0.3 else previous_selected
    controller._test_previous_base_curvature = float(base_smoothed)
    controller._test_previous_selected_curvature = float(selected_smoothed)
  return result


def activate(controller, model_output, v_ego=20.0, current_curvature=0.0):
  frames = round((ENTRY_TIME + RAMP_IN_TIME) / DT_MDL) + 3
  selected, status = step(
    controller, model_output, frames, v_ego=v_ego, current_curvature=current_curvature,
  )
  assert status.state == "active"
  assert status.authority == 1.0
  return selected, status


class TestLaneCenteringGates:
  def test_input_frequency_is_diagnostic_not_an_enable_gate(self):
    class FakeSubMaster:
      def __init__(self, alive=True, valid=True, frequency_ok=False):
        self.alive = alive
        self.valid = valid
        self.frequency_ok = frequency_ok

      def all_alive(self, services):
        assert services == ("carState", "carControl")
        return self.alive

      def all_valid(self, services):
        assert services == ("carState", "carControl")
        return self.valid

      def all_freq_ok(self, services):
        assert services == ("carState", "carControl")
        return self.frequency_ok

    status = get_lane_centering_input_status(FakeSubMaster(), live_calib_seen=True)
    assert status.ready
    assert not status.services_frequency_ok
    assert not get_lane_centering_input_status(FakeSubMaster(alive=False), True).ready
    assert not get_lane_centering_input_status(FakeSubMaster(valid=False), True).ready
    assert not get_lane_centering_input_status(FakeSubMaster(), False).ready

  def test_off_and_lateral_inactive_are_exact_plan_passthrough(self):
    output = make_centerline_output(center_y=0.25)
    original = output["plan"].copy()

    selected, status = step(LaneCenteringController("off"), output, 20)
    assert selected is output
    assert np.array_equal(selected["plan"], original)
    assert status.reason == "mode_off"
    assert status.curvature_correction == 0.0

    selected, status = step(LaneCenteringController("absolute"), output, lat_active=False)
    assert selected is output
    assert np.array_equal(selected["plan"], original)
    assert status.reason == "lateral_inactive"

  def test_entry_and_hold_use_separate_minimum_widths(self):
    assert HOLD_MIN_LANE_WIDTH == 2.45
    narrow = make_centerline_output(width=2.50)
    controller = LaneCenteringController("absolute")
    _, status = step(controller, narrow, round(ENTRY_TIME / DT_MDL) + 2)
    assert status.state == "inactive"
    assert status.line_gate == "geometry_width_range"

    controller = LaneCenteringController("absolute")
    activate(controller, make_centerline_output(width=2.70))
    _, status = step(controller, narrow)
    assert status.state == "active"
    assert status.line_gate == "valid"

    _, status = step(controller, make_centerline_output(width=2.44))
    assert status.state == "active"
    assert status.reason == "boundary_invalid"
    assert status.line_gate == "geometry_width_range"

  def test_rejects_bad_width_and_ego_outside(self):
    controller = LaneCenteringController("capped")
    _, status = step(controller, make_model_output(left_y=-2.5, right_y=2.5), 30)
    assert status.state == "inactive"
    assert status.line_gate == "geometry_width_range"

    _, status = step(controller, make_model_output(left_y=-0.8, right_y=2.8), 30)
    assert status.state == "inactive"
    assert status.line_gate == "geometry_ego_left"

  def test_camera_offset_centers_symmetric_vehicle_lane(self):
    centered = make_model_output(left_y=-1.84, right_y=1.76, plan_y=0.0)
    selected, status = activate(LaneCenteringController("absolute"), centered)
    assert abs(status.center_offset) < 1e-6
    assert abs(plan_curvature(selected)) < 1e-8

    shifted = make_model_output(left_y=-1.64, right_y=1.96, plan_y=0.24)
    selected, status = activate(LaneCenteringController("absolute"), shifted)
    assert status.center_offset > 0.0
    assert plan_curvature(selected) > 0.0

  def test_active_hold_allows_body_overlap_while_vehicle_center_stays_inside(self):
    controller = LaneCenteringController("absolute")
    activate(controller, make_centerline_output(width=2.70))

    for center_offset in np.arange(0.15, 0.76, 0.15):
      _, status = step(controller, make_centerline_output(center_y=center_offset, width=2.70))
      assert status.state == "active"
      assert status.line_gate == "valid"

    assert status.center_offset > 0.70
    left_near = make_centerline_output(center_y=0.75, width=2.70)["lane_lines"][0, 1, 0, 0]
    assert left_near > -(1.08 + CAMERA_OFFSET)

  def test_entry_exit_and_confidence_hysteresis(self):
    output = make_centerline_output(center_y=0.20)
    controller = LaneCenteringController("absolute")
    _, status = step(controller, output, round(ENTRY_TIME / DT_MDL) - 1)
    assert status.state == "acquiring"
    assert status.authority == 0.0

    _, status = step(controller, output)
    assert status.state == "active"
    _, status = step(controller, output, round(RAMP_IN_TIME / DT_MDL))
    assert status.authority == 1.0

    dropout = copy.deepcopy(output)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0
    _, status = step(controller, dropout, round(INVALID_GRACE_TIME / DT_MDL))
    assert status.state == "active"
    assert status.authority == 1.0
    _, status = step(controller, dropout)
    assert status.state == "exiting"
    assert status.authority < 1.0

  def test_acquisition_tolerates_300ms_confidence_noise_but_not_more(self):
    output = make_centerline_output(center_y=0.20)
    noisy = copy.deepcopy(output)
    noisy["lane_lines_prob"][0, 5] = 0.65
    controller = LaneCenteringController("absolute")
    step(controller, output, 6)

    grace_frames = round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL)
    _, status = step(controller, noisy, grace_frames)
    assert status.state == "acquiring"
    assert status.reason == "entry_confidence_grace"
    _, status = step(controller, noisy)
    assert status.state == "inactive"
    assert status.reason == "entry_conditions"

  def test_acquisition_window_discards_stale_evidence(self):
    output = make_centerline_output(center_y=0.20)
    noisy = copy.deepcopy(output)
    noisy["lane_lines_prob"][0, 5] = 0.65
    controller = LaneCenteringController("absolute")

    strict_frames = 5
    noisy_frames = round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL) - 1
    assert 2 * (strict_frames + noisy_frames) + 1 == round(ENTRY_WINDOW_TIME / DT_MDL)
    step(controller, output, strict_frames)
    step(controller, noisy, noisy_frames)
    step(controller, output, strict_frames)
    step(controller, noisy, noisy_frames)
    step(controller, output)
    _, status = step(controller, output)
    assert status.reason == "entry_window_restart"
    assert status.authority == 0.0

  def test_same_pair_recovery_requires_strict_two_line_confidence(self):
    output = make_centerline_output(width=2.70)
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    dropout = copy.deepcopy(output)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0
    step(controller, dropout, round(INVALID_GRACE_TIME / DT_MDL) + 1)

    low_confidence = make_centerline_output(center_y=0.70, width=2.70)
    low_confidence["lane_lines_prob"][0, 5] = 0.65
    _, status = step(controller, low_confidence)
    assert status.state == "exiting"
    assert status.entry_gate == "right_probability"

    _, status = step(controller, make_centerline_output(center_y=0.70, width=2.70))
    assert status.state == "active"
    assert status.reason == "recovered"

  def test_recovery_keeps_same_pair_and_allows_body_overlap(self):
    output = make_centerline_output(width=2.70)
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    dropout = copy.deepcopy(output)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0
    _, status = step(controller, dropout, round(INVALID_GRACE_TIME / DT_MDL) + 1)
    assert status.state == "exiting"
    exit_authority = status.authority

    _, status = step(controller, make_centerline_output(center_y=0.75, width=2.70))
    assert status.state == "active"
    assert status.reason == "recovered"
    assert status.source == "lane_lines"
    assert status.authority > exit_authority
    assert status.line_gate == "valid"

  def test_recovery_rejects_a_pair_with_inconsistent_shape(self):
    output = make_centerline_output(width=2.70)
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    dropout = copy.deepcopy(output)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0
    step(controller, dropout, round(INVALID_GRACE_TIME / DT_MDL) + 1)

    inconsistent_center = 0.002 * MODEL_X * MODEL_X
    _, status = step(controller, make_centerline_output(center_y=inconsistent_center, width=2.70))
    assert status.state == "exiting"
    assert status.line_gate == "geometry_pair_shape"

  def test_same_pair_entry_geometry_interrupt_is_graced(self):
    output = make_centerline_output(center_y=0.10, width=2.70)
    narrow = make_centerline_output(center_y=0.10, width=2.55)

    controller = LaneCenteringController("absolute")
    step(controller, output, 6)
    evidence_before = controller.acquire_time
    _, status = step(controller, narrow)
    assert status.state == "acquiring"
    assert status.reason == "entry_geometry_grace"
    assert status.line_gate == "geometry_width_range"
    assert controller.acquire_time == evidence_before

    remaining_frames = round(ENTRY_TIME / DT_MDL) - 6
    _, status = step(controller, output, remaining_frames)
    assert status.state == "active"

    controller = LaneCenteringController("absolute")
    step(controller, output, 6)
    grace_frames = round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL)
    _, status = step(controller, narrow, grace_frames)
    assert status.state == "acquiring"
    _, status = step(controller, narrow)
    assert status.state == "inactive"

  def test_acquisition_does_not_mix_distinct_line_pairs(self):
    first_pair = make_centerline_output(center_y=0.0, width=3.60)
    second_pair = make_centerline_output(center_y=0.60, width=3.60)
    controller = LaneCenteringController("absolute")
    step(controller, first_pair, 6)
    assert np.isclose(controller.acquire_time, 6 * DT_MDL)

    _, status = step(controller, second_pair)
    assert status.state == "acquiring"
    assert status.reason == "entry_pair_restart"
    assert np.isclose(controller.acquire_time, DT_MDL)

    _, status = step(controller, second_pair, round(ENTRY_TIME / DT_MDL) - 1)
    assert status.state == "active"

  def test_stable_hold_uncertainty_counts_only_after_a_strict_anchor(self):
    output = make_centerline_output(center_y=0.20)
    moderate = copy.deepcopy(output)
    moderate["lane_lines_stds"][0, 1:3, :, 0] = 0.20

    controller = LaneCenteringController("absolute")
    _, status = step(controller, moderate, round(2.0 / DT_MDL))
    assert status.state == "inactive"
    assert controller.acquire_time == 0.0

    controller = LaneCenteringController("absolute")
    step(controller, output)
    _, status = step(controller, moderate, round(ENTRY_TIME / DT_MDL) - 1)
    assert status.state == "active"
    assert status.entry_gate == "left_uncertainty+right_uncertainty"

    above_hold = copy.deepcopy(output)
    above_hold["lane_lines_stds"][0, 1:3, :, 0] = 0.31
    controller = LaneCenteringController("absolute")
    step(controller, output, 6)
    evidence_before = controller.acquire_time
    step(controller, above_hold, round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL))
    assert controller.acquire_time == evidence_before
    _, status = step(controller, above_hold)
    assert status.state == "inactive"

  def test_absolute_soft_policy_acquires_while_capped_only_graces_it(self):
    for disagreement in (0.30, 0.44):
      soft_policy = make_centerline_output(plan_y=disagreement)
      _, status = step(
        LaneCenteringController("absolute"), soft_policy, round(ENTRY_TIME / DT_MDL),
      )
      assert status.state == "active"
      assert status.policy_gate == "blended"

      _, status = step(
        LaneCenteringController("capped"), soft_policy, round(2.0 / DT_MDL),
      )
      assert status.state == "inactive"

    output = make_centerline_output()
    soft_policy = make_centerline_output(plan_y=0.30)
    controller = LaneCenteringController("capped")
    step(controller, output, 6)
    grace_frames = round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL)
    _, status = step(controller, soft_policy, grace_frames)
    assert status.state == "acquiring"
    assert status.reason == "entry_policy_grace"
    _, status = step(controller, soft_policy)
    assert status.state == "inactive"

  def test_entry_interruption_causes_share_one_grace_timer(self):
    output = make_centerline_output(center_y=0.10, width=2.70)
    low_probability = copy.deepcopy(output)
    low_probability["lane_lines_prob"][0, 5] = 0.65
    narrow = make_centerline_output(center_y=0.10, width=2.55)
    soft_policy = copy.deepcopy(output)
    soft_policy["plan"][0, :, Plan.POSITION.start + 1] += 0.30

    controller = LaneCenteringController("capped")
    step(controller, output, 6)
    interruptions = (low_probability, narrow, soft_policy)
    grace_frames = round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL)
    for frame in range(grace_frames):
      _, status = step(controller, interruptions[frame % len(interruptions)])
      assert status.state == "acquiring"
    _, status = step(controller, low_probability)
    assert status.state == "inactive"

  def test_point08_probability_is_only_a_brief_same_pair_interruption(self):
    output = make_centerline_output(center_y=0.20)
    low_probability = copy.deepcopy(output)
    low_probability["lane_lines_prob"][0, 5] = 0.08

    controller = LaneCenteringController("absolute")
    _, status = step(controller, low_probability, round(2.0 / DT_MDL))
    assert status.state == "inactive"
    assert controller.width_reference is None

    controller = LaneCenteringController("absolute")
    step(controller, output, 6)
    evidence_before = controller.acquire_time
    grace_frames = round(ENTRY_CONFIDENCE_GRACE_TIME / DT_MDL)
    _, status = step(controller, low_probability, grace_frames)
    assert status.state == "acquiring"
    assert status.reason == "entry_confidence_grace"
    assert controller.acquire_time == evidence_before
    _, status = step(controller, low_probability)
    assert status.state == "inactive"

  def test_one_frame_geometry_spikes_are_rejected_without_reacquisition(self):
    output = make_centerline_output(center_y=0.20)
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    translation_spike = make_centerline_output(center_y=0.70)
    _, status = step(controller, translation_spike)
    assert status.state == "active"
    assert status.reason == "boundary_invalid"
    assert status.line_gate == "geometry_center_step"

    _, status = step(controller, output)
    assert status.state == "active"
    assert status.reason == "tracking"

    pivot_spike = make_centerline_output(center_y=0.20 + 0.002 * (MODEL_X - 20.0) ** 2)
    _, status = step(controller, pivot_spike)
    assert status.state == "active"
    assert status.reason == "boundary_invalid"
    assert status.line_gate == "geometry_center_innovation"

    _, status = step(controller, output)
    assert status.state == "active"
    assert status.reason == "tracking"

  def test_edge_cannot_cold_acquire_or_bypass_reference_period(self):
    cold = make_centerline_output()
    cold["lane_lines_prob"][0, 5] = 0.0
    controller = LaneCenteringController("absolute")
    _, status = step(controller, cold, 100)
    assert status.state == "inactive"
    assert status.source == "none"

    output = make_centerline_output()
    controller = LaneCenteringController("absolute")
    step(controller, output, round(ENTRY_TIME / DT_MDL))
    missing_right = copy.deepcopy(output)
    missing_right["lane_lines_prob"][0, 5] = 0.0
    _, status = step(controller, missing_right, round(EDGE_ACQUIRE_TIME / DT_MDL) + 1)
    assert status.source != "left_line_right_edge"
    assert status.reason != "edge_acquiring"

  def test_edge_bridge_requires_stable_two_line_reference(self):
    output = make_centerline_output(center_y=0.20)
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    missing_right = copy.deepcopy(output)
    missing_right["lane_lines_prob"][0, 5] = 0.0
    missing_right["road_edges"][0, 1, :, 0] = missing_right["lane_lines"][0, 2, :, 0]
    _, status = step(controller, missing_right, round(EDGE_ACQUIRE_TIME / DT_MDL) - 1)
    assert status.reason == "edge_acquiring"
    _, status = step(controller, missing_right)
    assert status.source == "left_line_right_edge"
    assert status.state == "active"

  def test_edge_cannot_reacquire_after_state_becomes_inactive(self):
    output = make_centerline_output(center_y=0.20)
    controller = LaneCenteringController("absolute")
    activate(controller, output)
    assert controller.line_reference_ready

    hard_policy = copy.deepcopy(output)
    hard_policy["plan"][0, :, Plan.POSITION.start + 1] += 0.60
    _, status = step(controller, hard_policy, round(0.5 / DT_MDL))
    assert status.state == "inactive"
    assert controller.line_reference_ready

    missing_right = copy.deepcopy(output)
    missing_right["lane_lines_prob"][0, 5] = 0.08
    missing_right["road_edges"][0, 1, :, 0] = missing_right["lane_lines"][0, 2, :, 0]
    _, status = step(controller, missing_right)
    assert status.state == "inactive"
    assert status.source == "none"
    assert status.edge_gate == "state_not_active"

  def test_subveto_avoidance_shape_remains_hard_in_all_modes(self):
    output = make_centerline_output()
    avoidance = copy.deepcopy(output)
    plan_x = avoidance["plan"][0, :, Plan.POSITION.start]
    avoidance["plan"][0, :, Plan.POSITION.start + 1] = np.interp(
      plan_x, [0.0, 10.0, 20.0, 30.0, 192.0], [0.0, 0.0, 0.36, 0.36, 0.0],
    )

    for mode in ("absolute", "capped"):
      controller = LaneCenteringController(mode)
      step(controller, output, 6)
      _, status = step(controller, avoidance)
      assert status.state == "inactive"
      assert status.reason == "policy_avoidance"
      assert status.policy_gate == "avoidance_shape"
      assert status.policy_disagreement < POLICY_ZERO_AGREEMENT

      controller = LaneCenteringController(mode)
      activate(controller, output)
      _, status = step(controller, avoidance)
      assert status.reason == "policy_avoidance"
      assert status.authority < 1.0

  def test_lane_change_veto_remains_immediate_during_acquisition(self):
    output = make_centerline_output()
    for mode in ("absolute", "capped"):
      controller = LaneCenteringController(mode)
      step(controller, output, 6)
      _, status = step(controller, output, left_blinker=True)
      assert status.state == "inactive"
      assert status.reason == "lane_change"
      assert status.authority == 0.0

  def test_policy_avoidance_and_lane_change_veto(self):
    output = make_centerline_output()
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    avoidance = copy.deepcopy(output)
    plan_x = avoidance["plan"][0, :, Plan.POSITION.start]
    avoidance["plan"][0, :, Plan.POSITION.start + 1] = np.interp(
      plan_x, [0.0, 10.0, 20.0, 30.0, 192.0], [0.0, 0.0, 0.5, 0.5, 0.0],
    )
    _, status = step(controller, avoidance)
    assert status.reason == "policy_avoidance"
    assert status.policy_disagreement >= POLICY_ZERO_AGREEMENT
    assert status.authority < 1.0
    _, status = step(controller, avoidance, round(1.0 / DT_MDL))
    assert status.state == "inactive"
    assert status.authority == 0.0

    controller = LaneCenteringController("absolute")
    activate(controller, output)
    _, status = step(controller, output, left_blinker=True)
    assert status.reason == "lane_change"
    assert status.authority < 1.0

  def test_guard_timers_use_real_elapsed_time_and_large_model_gap_fails_closed(self):
    output = make_centerline_output(center_y=0.20)
    dropout = copy.deepcopy(output)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0

    controller = LaneCenteringController("absolute")
    activate(controller, output)
    _, status = step(controller, dropout, frame_dt=0.29)
    assert status.state == "active"
    _, status = step(controller, dropout, frame_dt=0.05)
    assert status.state == "exiting"
    assert status.reason == "boundary_lost"

    controller = LaneCenteringController("absolute")
    activate(controller, output)
    selected, status = step(controller, output, frame_dt=1.0)
    assert np.array_equal(selected["plan"], output["plan"])
    assert status.reason == "model_gap"
    assert controller.filtered_center_y is None


class TestLaneTrajectory:
  def test_dynamically_infeasible_lane_path_abstains_instead_of_publishing_an_arc(self):
    output = make_centerline_output(plan_speed=30.0)
    controller = LaneCenteringController("absolute")
    controller.filtered_center_y = 0.01 * MODEL_X * MODEL_X

    lane_plan, _ = controller._build_lane_plan(output["plan"], 30.0, 0.0)

    assert lane_plan is None
    assert controller.last_lane_path_feasibility == 0.0

  def test_quintic_matches_ego_and_lane_center_with_c2_continuity(self):
    reference = np.array([0.24, -0.012, 0.0008])
    join_distance = 24.0
    current_curvature = -0.003
    coefficients = LaneCenteringController._quintic_coefficients(
      join_distance, reference, current_curvature,
    )

    x = np.array([0.0, join_distance - 1e-6, join_distance, join_distance + 1e-6])
    y, slope, second = LaneCenteringController._evaluate_spatial_path(
      x, join_distance, reference, coefficients,
    )
    target_y = reference[0] + reference[1] * join_distance + reference[2] * join_distance**2
    target_slope = reference[1] + 2.0 * reference[2] * join_distance

    assert y[0] == 0.0
    assert slope[0] == 0.0
    assert np.isclose(second[0], current_curvature)
    assert np.isclose(y[2], target_y)
    assert np.isclose(slope[2], target_slope)
    assert np.isclose(second[2], 2.0 * reference[2])
    assert abs(y[3] - y[1]) < 1e-5
    assert abs(slope[3] - slope[1]) < 1e-6
    assert abs(second[3] - second[1]) < 1e-6

  def test_lane_plan_is_body_frame_coherent_and_jerk_bounded(self):
    center = 0.28 - 0.006 * MODEL_X + 0.00025 * MODEL_X**2
    output = make_centerline_output(center_y=center)
    controller = LaneCenteringController("absolute")
    controller.filtered_center_y = center.copy()
    current_curvature = -0.002
    lane_plan, _ = controller._build_lane_plan(output["plan"], 20.0, current_curvature)
    assert lane_plan is not None

    speed = lane_plan[0, :, Plan.VELOCITY.start]
    yaw_rate = lane_plan[0, :, Plan.ORIENTATION_RATE.start + 2]
    lateral_accel = lane_plan[0, :, Plan.ACCELERATION.start + 1]
    assert lane_plan[0, 0, Plan.POSITION.start] == 0.0
    assert lane_plan[0, 0, Plan.POSITION.start + 1] == 0.0
    assert lane_plan[0, 0, Plan.T_FROM_CURRENT_EULER.start + 2] == 0.0
    assert np.allclose(lane_plan[0, :, Plan.VELOCITY.start + 1], 0.0)
    assert np.allclose(lateral_accel, yaw_rate * speed, atol=1e-10)
    assert np.isclose(yaw_rate[0], current_curvature * speed[0], atol=1e-10)

    lateral_jerk = np.diff(lateral_accel) / np.diff(TIME_IDXS)
    assert np.max(np.abs(lateral_jerk[TIME_IDXS[1:] <= 2.0])) <= MAX_LANE_PATH_JERK + 1e-9

  def test_lane_plan_feasibility_bounds_full_horizon_acceleration_and_jerk(self):
    speed = 20.0
    center = 0.24 - 0.004 * MODEL_X + 0.00005 * MODEL_X**2
    output = make_centerline_output(center_y=center, plan_speed=speed)
    controller = LaneCenteringController("absolute")
    controller.filtered_center_y = center.copy()
    lane_plan, join_distance = controller._build_lane_plan(output["plan"], speed, 0.0)
    assert lane_plan is not None
    assert controller.last_lane_path_feasibility == 1.0

    lateral_accel = lane_plan[0, :, Plan.ACCELERATION.start + 1]
    lateral_jerk = np.diff(lateral_accel) / np.diff(TIME_IDXS)
    assert np.max(np.abs(lateral_accel)) <= MAX_LANE_PATH_ACCEL + 1e-8
    assert np.max(np.abs(lateral_jerk)) <= MAX_LANE_PATH_JERK + 1e-8

    path_x = lane_plan[0, :, Plan.POSITION.start]
    after_join = path_x >= join_distance
    reference = controller._reference_coefficients()
    assert reference is not None
    expected_y = reference[0] + reference[1] * path_x + reference[2] * path_x**2
    expected_yaw = np.arctan(reference[1] + 2.0 * reference[2] * path_x)
    assert np.allclose(
      lane_plan[0, after_join, Plan.POSITION.start + 1], expected_y[after_join], atol=1e-8,
    )
    assert np.allclose(
      lane_plan[0, after_join, Plan.T_FROM_CURRENT_EULER.start + 2],
      expected_yaw[after_join],
      atol=1e-8,
    )

  def test_current_curvature_mismatch_abstains_without_switching_join_targets(self):
    speed = 10.0
    output = make_centerline_output(plan_speed=speed)
    output["plan"][0, :, Plan.VELOCITY.start] = speed + 0.5 * TIME_IDXS
    controller = LaneCenteringController("absolute")
    controller.filtered_center_y = np.zeros_like(MODEL_X)
    lane_plan, join_distance = controller._build_lane_plan(output["plan"], speed, 0.03)
    assert lane_plan is None
    assert join_distance == NOMINAL_MERGE_TIME * speed
    assert controller.last_lane_path_feasibility == 0.0

  def test_infeasible_path_clears_authority_and_requires_reacquisition(self):
    speed = 30.0
    output = make_centerline_output(plan_speed=speed)
    controller = LaneCenteringController("absolute")
    activate(controller, output, v_ego=speed)

    selected, status = step(
      controller, output, v_ego=speed, current_curvature=0.0035,
    )
    assert selected is output
    assert status.state == "inactive"
    assert status.reason == "path_infeasible"
    assert status.authority == 0.0
    assert controller.authority == 0.0
    assert controller.last_lane_path_feasibility == 0.0

    _, status = step(controller, output, v_ego=speed, current_curvature=0.0)
    assert status.state == "acquiring"
    assert status.authority == 0.0

  def test_lane_plan_preserves_all_longitudinal_action_inputs(self):
    output = make_centerline_output(center_y=0.25, plan_speed=5.0)
    velocity_x = np.linspace(5.0, -0.5, ModelConstants.IDX_N, dtype=np.float32)
    acceleration_x = np.linspace(-0.7, -1.1, ModelConstants.IDX_N, dtype=np.float32)
    output["plan"][0, :, Plan.VELOCITY.start] = velocity_x
    output["plan"][0, :, Plan.ACCELERATION.start] = acceleration_x
    original_velocity_x = output["plan"][0, :, Plan.VELOCITY.start].copy()
    original_acceleration_x = output["plan"][0, :, Plan.ACCELERATION.start].copy()

    selected, status = activate(LaneCenteringController("absolute"), output, v_ego=5.0)
    assert status.authority == 1.0
    assert np.array_equal(selected["plan"][0, :, Plan.VELOCITY.start], original_velocity_x)
    assert np.array_equal(selected["plan"][0, :, Plan.ACCELERATION.start], original_acceleration_x)

    previous_action = log.ModelDataV2.Action(desiredAcceleration=0.2)
    base_action = modeld_module.get_action_from_model(output, previous_action, ACTION_T, 0.5, 5.0)
    selected_action = modeld_module.get_action_from_model(selected, previous_action, ACTION_T, 0.5, 5.0)
    assert selected_action.desiredAcceleration == base_action.desiredAcceleration
    assert selected_action.shouldStop == base_action.shouldStop

  def test_absolute_full_authority_has_zero_e2e_lateral_contribution(self):
    center = 0.22 - 0.004 * MODEL_X
    first = make_centerline_output(center_y=center)
    second = copy.deepcopy(first)
    second["plan"][0, :, Plan.VELOCITY.start + 1] = 1.5
    second["plan"][0, :, Plan.ACCELERATION.start + 1] = -2.0
    second["plan"][0, :, Plan.T_FROM_CURRENT_EULER.start + 2] = np.linspace(0.0, 0.20, ModelConstants.IDX_N)
    second["plan"][0, :, Plan.ORIENTATION_RATE.start + 2] = 0.08

    first_selected, first_status = activate(LaneCenteringController("absolute"), first)
    second_selected, second_status = activate(LaneCenteringController("absolute"), second)
    assert first_status.authority == second_status.authority == 1.0

    lateral_indices = (
      Plan.POSITION.start + 1,
      Plan.VELOCITY.start + 1,
      Plan.ACCELERATION.start + 1,
      Plan.T_FROM_CURRENT_EULER.start + 2,
      Plan.ORIENTATION_RATE.start + 2,
    )
    for index in lateral_indices:
      assert np.array_equal(first_selected["plan"][0, :, index], second_selected["plan"][0, :, index])
    assert plan_curvature(first_selected) == plan_curvature(second_selected)

  def test_authority_ramp_blends_the_trajectory_not_a_curvature_command(self):
    output = make_centerline_output(center_y=0.25)
    controller = LaneCenteringController("absolute")
    selected, status = step(controller, output, round(ENTRY_TIME / DT_MDL))
    assert status.state == "active"
    assert status.authority == 0.0
    assert selected is output

    selected, status = step(controller, output)
    assert 0.0 < status.authority < 1.0
    lane_plan, _ = controller._build_lane_plan(output["plan"], 20.0, 0.0)
    expected = LaneCenteringController._blend_plans(output["plan"], lane_plan, status.authority)
    assert np.allclose(selected["plan"], expected.astype(selected["plan"].dtype))
    assert controller.last_path_weight == status.authority

  def test_published_action_is_derived_from_the_selected_path(self):
    output = make_centerline_output(center_y=0.30)
    selected, status = activate(LaneCenteringController("absolute"), output)
    assert status.authority == 1.0
    expected = get_curvature_from_plan(
      selected["plan"][0, :, Plan.T_FROM_CURRENT_EULER.start + 2],
      selected["plan"][0, :, Plan.ORIENTATION_RATE.start + 2],
      ModelConstants.T_IDXS,
      20.0,
      ACTION_T,
    )
    previous_action = -0.001
    published_action = smooth_value(expected, previous_action, 0.1)
    assert np.isclose(published_action, smooth_value(plan_curvature(selected), previous_action, 0.1))

  def test_absolute_lane_path_is_independent_of_previous_action_history(self):
    output = make_centerline_output(center_y=0.30)
    selected_plans = []
    for previous_action in (-0.10, 0.10):
      controller = LaneCenteringController("absolute")
      controller.state = "active"
      controller.authority = 1.0
      controller.last_policy_weight = 1.0
      controller.filtered_center_y = np.full_like(MODEL_X, 0.30)
      controller.frame_dt = DT_MDL
      selected_plans.append(controller._select_output(
        output, 20.0, 0.0, ACTION_T, 0.0, previous_action,
      )["plan"])

    assert np.array_equal(selected_plans[0], selected_plans[1])

  def test_path_native_exit_ramp_reaches_the_unmodified_base_plan(self):
    speed = 30.0
    lane_output = make_centerline_output(plan_speed=speed)
    controller = LaneCenteringController("absolute")
    activate(controller, lane_output, v_ego=speed)

    dropout = set_plan_curvature(copy.deepcopy(lane_output), 0.0055, speed)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0
    previous_action = controller._test_previous_selected_curvature
    saw_zero_authority = False
    last_selected = None

    for _ in range(round(2.5 / DT_MDL)):
      selected, status = step(controller, dropout, v_ego=speed)
      action = controller._test_previous_selected_curvature
      jerk = abs(action - previous_action) * speed**2 / DT_MDL
      assert jerk <= MAX_LANE_PATH_JERK + 2e-5
      if status.authority == 0.0:
        saw_zero_authority = True
        assert np.array_equal(selected["plan"], dropout["plan"])
      previous_action = action
      last_selected = selected

    assert saw_zero_authority
    assert last_selected is not None
    assert np.array_equal(last_selected["plan"], dropout["plan"])

  def test_ego_motion_propagation_tracks_a_fixed_world_line(self):
    controller = LaneCenteringController("absolute")
    offset = 0.30
    slope = -0.02
    controller.filtered_center_y = offset + slope * MODEL_X
    controller.frame_dt = DT_MDL
    speed = 20.0
    controller._propagate_centerline(speed, 0.0)

    expected = offset + slope * (MODEL_X + speed * DT_MDL)
    assert np.allclose(controller.filtered_center_y, expected, atol=1e-10)

  def test_invalid_grace_propagates_instead_of_freezing_an_ego_frame_target(self):
    center = 0.25 - 0.012 * MODEL_X
    output = make_centerline_output(center_y=center)
    controller = LaneCenteringController("absolute")
    activate(controller, output)

    dropout = copy.deepcopy(output)
    dropout["lane_lines_prob"][0, 3] = 0.0
    dropout["lane_lines_prob"][0, 5] = 0.0
    before = controller.filtered_center_y.copy()
    selected, status = step(controller, dropout)
    assert status.state == "active"
    assert not np.array_equal(controller.filtered_center_y, before)
    assert not np.array_equal(selected["plan"], output["plan"])

  def test_capped_mode_limits_path_derived_lateral_acceleration_correction(self):
    output = make_centerline_output(center_y=0.68, plan_speed=30.0)
    base_curvature = -0.004
    output["plan"][0, :, Plan.T_FROM_CURRENT_EULER.start + 2] = base_curvature * 30.0 * TIME_IDXS
    output["plan"][0, :, Plan.ORIENTATION_RATE.start + 2] = base_curvature * 30.0
    capped_selected, capped_status = activate(LaneCenteringController("capped"), output, v_ego=30.0)
    absolute_selected, absolute_status = activate(LaneCenteringController("absolute"), output, v_ego=30.0)

    assert abs(capped_status.curvature_correction) * 30.0**2 <= MAX_LATERAL_ACCEL_CORRECTION + 1e-6
    assert abs(absolute_status.curvature_correction) > abs(capped_status.curvature_correction)
    base_action = plan_curvature(output, 30.0)
    assert abs(plan_curvature(absolute_selected, 30.0) - base_action) > abs(plan_curvature(capped_selected, 30.0) - base_action)

  def test_capped_mode_speed_jump_reenters_bound_with_path_weighting(self):
    controller = LaneCenteringController("capped")
    slow = make_centerline_output(center_y=0.68, plan_speed=10.0)
    activate(controller, slow, v_ego=10.0)

    fast = make_centerline_output(center_y=0.68, plan_speed=30.0)
    corrections = []
    for _ in range(round(2.0 / DT_MDL)):
      selected, status = step(controller, fast, v_ego=30.0)
      assert np.all(np.isfinite(selected["plan"]))
      corrections.append(abs(status.curvature_correction))

    assert corrections[-1] * 30.0**2 <= MAX_LATERAL_ACCEL_CORRECTION + 1e-5
    assert corrections[-1] < corrections[0]

  def test_modeld_helper_is_the_runtime_path_and_derives_action_once_from_selected_plan(self):
    main_source = inspect.getsource(modeld_module.main)
    assert "get_lane_centered_action(" in main_source
    assert "lane_centering.update(" not in main_source
    assert "fill_model_msg(drivingdata_send, modelv2_send, selected_model_output, action," in main_source

    output = make_centerline_output(center_y=0.30)
    controller = LaneCenteringController("absolute")
    previous_action = log.ModelDataV2.Action()
    previous_base_action = log.ModelDataV2.Action()
    selected = output
    status = None
    for _ in range(round((ENTRY_TIME + RAMP_IN_TIME + 0.5) / DT_MDL)):
      action_history = previous_action
      selected, action, base_action, status = modeld_module.get_lane_centered_action(
        output, controller, previous_action, previous_base_action,
        20.0, 0.0, ACTION_T, 0.5, DT_MDL,
        True, True, False, False, False,
      )
      expected_action = modeld_module.get_action_from_model(
        selected, action_history, ACTION_T, 0.5, 20.0,
      )
      assert action.desiredCurvature == expected_action.desiredCurvature
      assert action.desiredAcceleration == base_action.desiredAcceleration
      assert action.shouldStop == base_action.shouldStop
      previous_action = action
      previous_base_action = base_action

    assert status is not None
    assert status.authority == 1.0
    assert not np.array_equal(selected["plan"], output["plan"])

  def test_variable_frame_time_advances_hysteresis_by_elapsed_time(self):
    output = make_centerline_output(center_y=0.20)
    controller = LaneCenteringController("absolute")
    _, status = step(controller, output, 3, frame_dt=0.25)
    assert status.state == "active"
    assert status.authority == 0.0

  def test_delayed_closed_loop_stress_grid_settles_without_a_limit_cycle(self):
    scenarios = (
      (10.0, 0.30, 0.12, 0.45, 0.015),
      (17.5, ACTION_T, 0.16, 0.32, 0.0),
      (27.0, 0.55, 0.22, 0.30, -0.010),
      (17.5, 0.60, 0.25, 0.30, 0.0),
      (30.0, ACTION_T, 0.16, -0.30, 0.0),
    )

    for speed, physical_delay, actuator_tau, initial_position, initial_heading in scenarios:
      controller = LaneCenteringController("absolute")
      lateral_position = initial_position
      heading = initial_heading
      actual_curvature = 0.0
      smoothed_action = 0.0
      delay_frames = max(1, round(physical_delay / DT_MDL))
      delayed_actions = deque([0.0] * delay_frames, maxlen=delay_frames)
      errors = []
      actions = []

      for frame in range(round(14.0 / DT_MDL)):
        center = -lateral_position - np.tan(heading) * MODEL_X
        center += 0.02 * np.sin(2.0 * np.pi * 1.7 * frame * DT_MDL)
        output = make_centerline_output(center_y=center, plan_speed=speed)
        if 110 <= frame < 115 or 190 <= frame < 194:
          output["lane_lines_prob"][0, 3] = 0.0
          output["lane_lines_prob"][0, 5] = 0.0

        selected, _ = step(
          controller, output, v_ego=speed, current_curvature=actual_curvature,
        )
        raw_action = plan_curvature(selected, speed)
        smoothed_action = smooth_value(raw_action, smoothed_action, 0.1)
        delayed_actions.append(smoothed_action)
        delayed_action = delayed_actions[0]

        actual_curvature += DT_MDL / actuator_tau * (delayed_action - actual_curvature)
        heading += actual_curvature * speed * DT_MDL
        lateral_position += speed * np.sin(heading) * DT_MDL
        errors.append(lateral_position)
        actions.append(smoothed_action)

      settled = np.asarray(errors)[round(8.0 / DT_MDL):]
      settled_actions = np.asarray(actions)[round(8.0 / DT_MDL):]
      assert abs(np.mean(settled)) < 0.05
      assert np.max(np.abs(settled)) < 0.12
      assert np.std(settled_actions) < 0.0018

  def test_invalid_mode_rejected(self):
    try:
      LaneCenteringController("forced")
    except ValueError:
      pass
    else:
      raise AssertionError("invalid mode was accepted")
