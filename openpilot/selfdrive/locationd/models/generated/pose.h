#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8408857391730755841);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5129418956577183101);
void pose_H_mod_fun(double *state, double *out_3902216871053882414);
void pose_f_fun(double *state, double dt, double *out_6550423757343157361);
void pose_F_fun(double *state, double dt, double *out_3303653463282877516);
void pose_h_4(double *state, double *unused, double *out_4870156935976554141);
void pose_H_4(double *state, double *unused, double *out_9006241895721244560);
void pose_h_10(double *state, double *unused, double *out_6725094615089616867);
void pose_H_10(double *state, double *unused, double *out_5824813145068267930);
void pose_h_13(double *state, double *unused, double *out_1352228507741542507);
void pose_H_13(double *state, double *unused, double *out_1829870969671606127);
void pose_h_14(double *state, double *unused, double *out_8447259540628755835);
void pose_H_14(double *state, double *unused, double *out_5477261321648822527);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}