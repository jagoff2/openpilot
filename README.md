# C3X active-install snapshot
## THIS BRANCH WILL LIKELY CAUSE COMMA TO BAN YOUR DEVICE FROM UPLOADS (COMMA CONNECT) IF YOU UPLOAD ANY ROUTES WHICH ARE DRIVEN ON THIS BRANCH.
## YOU HAVE BEEN WARNED. NOT MY RESPONSIBILITY. 
This branch captures the active, reproducible openpilot source/runtime state from:

- Device: `comma@192.168.1.95`
- Checkout: `/data/openpilot`
- Device branch: `master-dev`
- Base commit: `025a930ce8f4cbf1650ac5bfc12dd7cdd194fd37`
- Capture date: 2026-07-28

## Included

- The complete tracked source tree and device commit history at the base commit.
- All eight modified tracked files from the live root checkout.
- The active untracked lane-centering implementation and its tests.
- The active untracked NNLC sigmoid-map tuner imported by the live controller.
- Both modified ONNX model assets through Git LFS:
  - policy: `78477124cbf3ffe30fa951ebada8410b43c4242c6054584d656f1d329b067e15`
  - vision: `ee29ee5bce84d1ce23e9ff381280de9b4e4d96d2934cd751740354884e112c66`
- The dirty `opendbc_repo` working state. It is vendored as ordinary files so the
  five live Hyundai interface, lateral-limit, and panda-safety edits cannot be
  lost behind the old clean submodule pointer.
- The non-secret runtime NNLC map loaded outside the repository, stored at
  `c3x_device_snapshot/runtime_params/d_tmp/sigmoid_maps/HYUNDAI_KONA_EV_nnlc.json`.
  Its SHA-256 is
  `83f5796acbb70a8a7d1c6f3fecf6709a903e9834f522663f4e2c5cc4b868b45d`.
- All other clean submodules as their original pinned Git links.

## Deliberately excluded

This is a complete active-source snapshot, not a byte-for-byte image of generated
storage. The following do not affect the active install and are excluded:

- `.git` object storage and submodule Git metadata.
- Ignored SCons products, object files, shared libraries, native build outputs,
  Python caches, and generated Tinygrad PKLs. These regenerate from tracked
  source and model inputs.
- Twenty-one historical `*.bak` recovery copies.
- Stale untracked `logcatd`, `proclogd`, and `ubloxd` build products.
- The inactive supercombo experiment, offline sigmoid-map analysis scripts, and
  the stale untracked `opendbc/safety/modes/safety.h` duplicate.
- Route logs, user databases, credentials, and the rest of `/data/params`.

## Runtime parameter restore

For behavior identical to the captured controller, copy the preserved NNLC map
to:

`/data/params/d_tmp/sigmoid_maps/HYUNDAI_KONA_EV_nnlc.json`

The tracked source-tree map is not a substitute; the live controller reads the
path above.
