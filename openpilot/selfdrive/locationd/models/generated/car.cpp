#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1994063185926291135) {
   out_1994063185926291135[0] = delta_x[0] + nom_x[0];
   out_1994063185926291135[1] = delta_x[1] + nom_x[1];
   out_1994063185926291135[2] = delta_x[2] + nom_x[2];
   out_1994063185926291135[3] = delta_x[3] + nom_x[3];
   out_1994063185926291135[4] = delta_x[4] + nom_x[4];
   out_1994063185926291135[5] = delta_x[5] + nom_x[5];
   out_1994063185926291135[6] = delta_x[6] + nom_x[6];
   out_1994063185926291135[7] = delta_x[7] + nom_x[7];
   out_1994063185926291135[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3556238228317985510) {
   out_3556238228317985510[0] = -nom_x[0] + true_x[0];
   out_3556238228317985510[1] = -nom_x[1] + true_x[1];
   out_3556238228317985510[2] = -nom_x[2] + true_x[2];
   out_3556238228317985510[3] = -nom_x[3] + true_x[3];
   out_3556238228317985510[4] = -nom_x[4] + true_x[4];
   out_3556238228317985510[5] = -nom_x[5] + true_x[5];
   out_3556238228317985510[6] = -nom_x[6] + true_x[6];
   out_3556238228317985510[7] = -nom_x[7] + true_x[7];
   out_3556238228317985510[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_4683309905866058333) {
   out_4683309905866058333[0] = 1.0;
   out_4683309905866058333[1] = 0.0;
   out_4683309905866058333[2] = 0.0;
   out_4683309905866058333[3] = 0.0;
   out_4683309905866058333[4] = 0.0;
   out_4683309905866058333[5] = 0.0;
   out_4683309905866058333[6] = 0.0;
   out_4683309905866058333[7] = 0.0;
   out_4683309905866058333[8] = 0.0;
   out_4683309905866058333[9] = 0.0;
   out_4683309905866058333[10] = 1.0;
   out_4683309905866058333[11] = 0.0;
   out_4683309905866058333[12] = 0.0;
   out_4683309905866058333[13] = 0.0;
   out_4683309905866058333[14] = 0.0;
   out_4683309905866058333[15] = 0.0;
   out_4683309905866058333[16] = 0.0;
   out_4683309905866058333[17] = 0.0;
   out_4683309905866058333[18] = 0.0;
   out_4683309905866058333[19] = 0.0;
   out_4683309905866058333[20] = 1.0;
   out_4683309905866058333[21] = 0.0;
   out_4683309905866058333[22] = 0.0;
   out_4683309905866058333[23] = 0.0;
   out_4683309905866058333[24] = 0.0;
   out_4683309905866058333[25] = 0.0;
   out_4683309905866058333[26] = 0.0;
   out_4683309905866058333[27] = 0.0;
   out_4683309905866058333[28] = 0.0;
   out_4683309905866058333[29] = 0.0;
   out_4683309905866058333[30] = 1.0;
   out_4683309905866058333[31] = 0.0;
   out_4683309905866058333[32] = 0.0;
   out_4683309905866058333[33] = 0.0;
   out_4683309905866058333[34] = 0.0;
   out_4683309905866058333[35] = 0.0;
   out_4683309905866058333[36] = 0.0;
   out_4683309905866058333[37] = 0.0;
   out_4683309905866058333[38] = 0.0;
   out_4683309905866058333[39] = 0.0;
   out_4683309905866058333[40] = 1.0;
   out_4683309905866058333[41] = 0.0;
   out_4683309905866058333[42] = 0.0;
   out_4683309905866058333[43] = 0.0;
   out_4683309905866058333[44] = 0.0;
   out_4683309905866058333[45] = 0.0;
   out_4683309905866058333[46] = 0.0;
   out_4683309905866058333[47] = 0.0;
   out_4683309905866058333[48] = 0.0;
   out_4683309905866058333[49] = 0.0;
   out_4683309905866058333[50] = 1.0;
   out_4683309905866058333[51] = 0.0;
   out_4683309905866058333[52] = 0.0;
   out_4683309905866058333[53] = 0.0;
   out_4683309905866058333[54] = 0.0;
   out_4683309905866058333[55] = 0.0;
   out_4683309905866058333[56] = 0.0;
   out_4683309905866058333[57] = 0.0;
   out_4683309905866058333[58] = 0.0;
   out_4683309905866058333[59] = 0.0;
   out_4683309905866058333[60] = 1.0;
   out_4683309905866058333[61] = 0.0;
   out_4683309905866058333[62] = 0.0;
   out_4683309905866058333[63] = 0.0;
   out_4683309905866058333[64] = 0.0;
   out_4683309905866058333[65] = 0.0;
   out_4683309905866058333[66] = 0.0;
   out_4683309905866058333[67] = 0.0;
   out_4683309905866058333[68] = 0.0;
   out_4683309905866058333[69] = 0.0;
   out_4683309905866058333[70] = 1.0;
   out_4683309905866058333[71] = 0.0;
   out_4683309905866058333[72] = 0.0;
   out_4683309905866058333[73] = 0.0;
   out_4683309905866058333[74] = 0.0;
   out_4683309905866058333[75] = 0.0;
   out_4683309905866058333[76] = 0.0;
   out_4683309905866058333[77] = 0.0;
   out_4683309905866058333[78] = 0.0;
   out_4683309905866058333[79] = 0.0;
   out_4683309905866058333[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5604582280809907208) {
   out_5604582280809907208[0] = state[0];
   out_5604582280809907208[1] = state[1];
   out_5604582280809907208[2] = state[2];
   out_5604582280809907208[3] = state[3];
   out_5604582280809907208[4] = state[4];
   out_5604582280809907208[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5604582280809907208[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5604582280809907208[7] = state[7];
   out_5604582280809907208[8] = state[8];
}
void F_fun(double *state, double dt, double *out_7378124410600008533) {
   out_7378124410600008533[0] = 1;
   out_7378124410600008533[1] = 0;
   out_7378124410600008533[2] = 0;
   out_7378124410600008533[3] = 0;
   out_7378124410600008533[4] = 0;
   out_7378124410600008533[5] = 0;
   out_7378124410600008533[6] = 0;
   out_7378124410600008533[7] = 0;
   out_7378124410600008533[8] = 0;
   out_7378124410600008533[9] = 0;
   out_7378124410600008533[10] = 1;
   out_7378124410600008533[11] = 0;
   out_7378124410600008533[12] = 0;
   out_7378124410600008533[13] = 0;
   out_7378124410600008533[14] = 0;
   out_7378124410600008533[15] = 0;
   out_7378124410600008533[16] = 0;
   out_7378124410600008533[17] = 0;
   out_7378124410600008533[18] = 0;
   out_7378124410600008533[19] = 0;
   out_7378124410600008533[20] = 1;
   out_7378124410600008533[21] = 0;
   out_7378124410600008533[22] = 0;
   out_7378124410600008533[23] = 0;
   out_7378124410600008533[24] = 0;
   out_7378124410600008533[25] = 0;
   out_7378124410600008533[26] = 0;
   out_7378124410600008533[27] = 0;
   out_7378124410600008533[28] = 0;
   out_7378124410600008533[29] = 0;
   out_7378124410600008533[30] = 1;
   out_7378124410600008533[31] = 0;
   out_7378124410600008533[32] = 0;
   out_7378124410600008533[33] = 0;
   out_7378124410600008533[34] = 0;
   out_7378124410600008533[35] = 0;
   out_7378124410600008533[36] = 0;
   out_7378124410600008533[37] = 0;
   out_7378124410600008533[38] = 0;
   out_7378124410600008533[39] = 0;
   out_7378124410600008533[40] = 1;
   out_7378124410600008533[41] = 0;
   out_7378124410600008533[42] = 0;
   out_7378124410600008533[43] = 0;
   out_7378124410600008533[44] = 0;
   out_7378124410600008533[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7378124410600008533[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7378124410600008533[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7378124410600008533[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7378124410600008533[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7378124410600008533[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7378124410600008533[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7378124410600008533[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7378124410600008533[53] = -9.8100000000000005*dt;
   out_7378124410600008533[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7378124410600008533[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7378124410600008533[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7378124410600008533[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7378124410600008533[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7378124410600008533[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7378124410600008533[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7378124410600008533[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7378124410600008533[62] = 0;
   out_7378124410600008533[63] = 0;
   out_7378124410600008533[64] = 0;
   out_7378124410600008533[65] = 0;
   out_7378124410600008533[66] = 0;
   out_7378124410600008533[67] = 0;
   out_7378124410600008533[68] = 0;
   out_7378124410600008533[69] = 0;
   out_7378124410600008533[70] = 1;
   out_7378124410600008533[71] = 0;
   out_7378124410600008533[72] = 0;
   out_7378124410600008533[73] = 0;
   out_7378124410600008533[74] = 0;
   out_7378124410600008533[75] = 0;
   out_7378124410600008533[76] = 0;
   out_7378124410600008533[77] = 0;
   out_7378124410600008533[78] = 0;
   out_7378124410600008533[79] = 0;
   out_7378124410600008533[80] = 1;
}
void h_25(double *state, double *unused, double *out_4144161697862184483) {
   out_4144161697862184483[0] = state[6];
}
void H_25(double *state, double *unused, double *out_8859742342658828443) {
   out_8859742342658828443[0] = 0;
   out_8859742342658828443[1] = 0;
   out_8859742342658828443[2] = 0;
   out_8859742342658828443[3] = 0;
   out_8859742342658828443[4] = 0;
   out_8859742342658828443[5] = 0;
   out_8859742342658828443[6] = 1;
   out_8859742342658828443[7] = 0;
   out_8859742342658828443[8] = 0;
}
void h_24(double *state, double *unused, double *out_3478640049777836153) {
   out_3478640049777836153[0] = state[4];
   out_3478640049777836153[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6682527919051678470) {
   out_6682527919051678470[0] = 0;
   out_6682527919051678470[1] = 0;
   out_6682527919051678470[2] = 0;
   out_6682527919051678470[3] = 0;
   out_6682527919051678470[4] = 1;
   out_6682527919051678470[5] = 0;
   out_6682527919051678470[6] = 0;
   out_6682527919051678470[7] = 0;
   out_6682527919051678470[8] = 0;
   out_6682527919051678470[9] = 0;
   out_6682527919051678470[10] = 0;
   out_6682527919051678470[11] = 0;
   out_6682527919051678470[12] = 0;
   out_6682527919051678470[13] = 0;
   out_6682527919051678470[14] = 1;
   out_6682527919051678470[15] = 0;
   out_6682527919051678470[16] = 0;
   out_6682527919051678470[17] = 0;
}
void h_30(double *state, double *unused, double *out_506712169608162133) {
   out_506712169608162133[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6341409384151579816) {
   out_6341409384151579816[0] = 0;
   out_6341409384151579816[1] = 0;
   out_6341409384151579816[2] = 0;
   out_6341409384151579816[3] = 0;
   out_6341409384151579816[4] = 1;
   out_6341409384151579816[5] = 0;
   out_6341409384151579816[6] = 0;
   out_6341409384151579816[7] = 0;
   out_6341409384151579816[8] = 0;
}
void h_26(double *state, double *unused, double *out_8475257002398678632) {
   out_8475257002398678632[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5845498412176666949) {
   out_5845498412176666949[0] = 0;
   out_5845498412176666949[1] = 0;
   out_5845498412176666949[2] = 0;
   out_5845498412176666949[3] = 0;
   out_5845498412176666949[4] = 0;
   out_5845498412176666949[5] = 0;
   out_5845498412176666949[6] = 0;
   out_5845498412176666949[7] = 1;
   out_5845498412176666949[8] = 0;
}
void h_27(double *state, double *unused, double *out_857511569798661162) {
   out_857511569798661162[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4117815312967636599) {
   out_4117815312967636599[0] = 0;
   out_4117815312967636599[1] = 0;
   out_4117815312967636599[2] = 0;
   out_4117815312967636599[3] = 1;
   out_4117815312967636599[4] = 0;
   out_4117815312967636599[5] = 0;
   out_4117815312967636599[6] = 0;
   out_4117815312967636599[7] = 0;
   out_4117815312967636599[8] = 0;
}
void h_29(double *state, double *unused, double *out_4494961098052683512) {
   out_4494961098052683512[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5831178039837187632) {
   out_5831178039837187632[0] = 0;
   out_5831178039837187632[1] = 1;
   out_5831178039837187632[2] = 0;
   out_5831178039837187632[3] = 0;
   out_5831178039837187632[4] = 0;
   out_5831178039837187632[5] = 0;
   out_5831178039837187632[6] = 0;
   out_5831178039837187632[7] = 0;
   out_5831178039837187632[8] = 0;
}
void h_28(double *state, double *unused, double *out_8871395584042198128) {
   out_8871395584042198128[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7533167016802833410) {
   out_7533167016802833410[0] = 1;
   out_7533167016802833410[1] = 0;
   out_7533167016802833410[2] = 0;
   out_7533167016802833410[3] = 0;
   out_7533167016802833410[4] = 0;
   out_7533167016802833410[5] = 0;
   out_7533167016802833410[6] = 0;
   out_7533167016802833410[7] = 0;
   out_7533167016802833410[8] = 0;
}
void h_31(double *state, double *unused, double *out_7091887688217669444) {
   out_7091887688217669444[0] = state[8];
}
void H_31(double *state, double *unused, double *out_8829096380781868015) {
   out_8829096380781868015[0] = 0;
   out_8829096380781868015[1] = 0;
   out_8829096380781868015[2] = 0;
   out_8829096380781868015[3] = 0;
   out_8829096380781868015[4] = 0;
   out_8829096380781868015[5] = 0;
   out_8829096380781868015[6] = 0;
   out_8829096380781868015[7] = 0;
   out_8829096380781868015[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_1994063185926291135) {
  err_fun(nom_x, delta_x, out_1994063185926291135);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3556238228317985510) {
  inv_err_fun(nom_x, true_x, out_3556238228317985510);
}
void car_H_mod_fun(double *state, double *out_4683309905866058333) {
  H_mod_fun(state, out_4683309905866058333);
}
void car_f_fun(double *state, double dt, double *out_5604582280809907208) {
  f_fun(state,  dt, out_5604582280809907208);
}
void car_F_fun(double *state, double dt, double *out_7378124410600008533) {
  F_fun(state,  dt, out_7378124410600008533);
}
void car_h_25(double *state, double *unused, double *out_4144161697862184483) {
  h_25(state, unused, out_4144161697862184483);
}
void car_H_25(double *state, double *unused, double *out_8859742342658828443) {
  H_25(state, unused, out_8859742342658828443);
}
void car_h_24(double *state, double *unused, double *out_3478640049777836153) {
  h_24(state, unused, out_3478640049777836153);
}
void car_H_24(double *state, double *unused, double *out_6682527919051678470) {
  H_24(state, unused, out_6682527919051678470);
}
void car_h_30(double *state, double *unused, double *out_506712169608162133) {
  h_30(state, unused, out_506712169608162133);
}
void car_H_30(double *state, double *unused, double *out_6341409384151579816) {
  H_30(state, unused, out_6341409384151579816);
}
void car_h_26(double *state, double *unused, double *out_8475257002398678632) {
  h_26(state, unused, out_8475257002398678632);
}
void car_H_26(double *state, double *unused, double *out_5845498412176666949) {
  H_26(state, unused, out_5845498412176666949);
}
void car_h_27(double *state, double *unused, double *out_857511569798661162) {
  h_27(state, unused, out_857511569798661162);
}
void car_H_27(double *state, double *unused, double *out_4117815312967636599) {
  H_27(state, unused, out_4117815312967636599);
}
void car_h_29(double *state, double *unused, double *out_4494961098052683512) {
  h_29(state, unused, out_4494961098052683512);
}
void car_H_29(double *state, double *unused, double *out_5831178039837187632) {
  H_29(state, unused, out_5831178039837187632);
}
void car_h_28(double *state, double *unused, double *out_8871395584042198128) {
  h_28(state, unused, out_8871395584042198128);
}
void car_H_28(double *state, double *unused, double *out_7533167016802833410) {
  H_28(state, unused, out_7533167016802833410);
}
void car_h_31(double *state, double *unused, double *out_7091887688217669444) {
  h_31(state, unused, out_7091887688217669444);
}
void car_H_31(double *state, double *unused, double *out_8829096380781868015) {
  H_31(state, unused, out_8829096380781868015);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
