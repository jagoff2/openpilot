#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_1994063185926291135);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3556238228317985510);
void car_H_mod_fun(double *state, double *out_4683309905866058333);
void car_f_fun(double *state, double dt, double *out_5604582280809907208);
void car_F_fun(double *state, double dt, double *out_7378124410600008533);
void car_h_25(double *state, double *unused, double *out_4144161697862184483);
void car_H_25(double *state, double *unused, double *out_8859742342658828443);
void car_h_24(double *state, double *unused, double *out_3478640049777836153);
void car_H_24(double *state, double *unused, double *out_6682527919051678470);
void car_h_30(double *state, double *unused, double *out_506712169608162133);
void car_H_30(double *state, double *unused, double *out_6341409384151579816);
void car_h_26(double *state, double *unused, double *out_8475257002398678632);
void car_H_26(double *state, double *unused, double *out_5845498412176666949);
void car_h_27(double *state, double *unused, double *out_857511569798661162);
void car_H_27(double *state, double *unused, double *out_4117815312967636599);
void car_h_29(double *state, double *unused, double *out_4494961098052683512);
void car_H_29(double *state, double *unused, double *out_5831178039837187632);
void car_h_28(double *state, double *unused, double *out_8871395584042198128);
void car_H_28(double *state, double *unused, double *out_7533167016802833410);
void car_h_31(double *state, double *unused, double *out_7091887688217669444);
void car_H_31(double *state, double *unused, double *out_8829096380781868015);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}