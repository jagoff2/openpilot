#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_8408857391730755841) {
   out_8408857391730755841[0] = delta_x[0] + nom_x[0];
   out_8408857391730755841[1] = delta_x[1] + nom_x[1];
   out_8408857391730755841[2] = delta_x[2] + nom_x[2];
   out_8408857391730755841[3] = delta_x[3] + nom_x[3];
   out_8408857391730755841[4] = delta_x[4] + nom_x[4];
   out_8408857391730755841[5] = delta_x[5] + nom_x[5];
   out_8408857391730755841[6] = delta_x[6] + nom_x[6];
   out_8408857391730755841[7] = delta_x[7] + nom_x[7];
   out_8408857391730755841[8] = delta_x[8] + nom_x[8];
   out_8408857391730755841[9] = delta_x[9] + nom_x[9];
   out_8408857391730755841[10] = delta_x[10] + nom_x[10];
   out_8408857391730755841[11] = delta_x[11] + nom_x[11];
   out_8408857391730755841[12] = delta_x[12] + nom_x[12];
   out_8408857391730755841[13] = delta_x[13] + nom_x[13];
   out_8408857391730755841[14] = delta_x[14] + nom_x[14];
   out_8408857391730755841[15] = delta_x[15] + nom_x[15];
   out_8408857391730755841[16] = delta_x[16] + nom_x[16];
   out_8408857391730755841[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5129418956577183101) {
   out_5129418956577183101[0] = -nom_x[0] + true_x[0];
   out_5129418956577183101[1] = -nom_x[1] + true_x[1];
   out_5129418956577183101[2] = -nom_x[2] + true_x[2];
   out_5129418956577183101[3] = -nom_x[3] + true_x[3];
   out_5129418956577183101[4] = -nom_x[4] + true_x[4];
   out_5129418956577183101[5] = -nom_x[5] + true_x[5];
   out_5129418956577183101[6] = -nom_x[6] + true_x[6];
   out_5129418956577183101[7] = -nom_x[7] + true_x[7];
   out_5129418956577183101[8] = -nom_x[8] + true_x[8];
   out_5129418956577183101[9] = -nom_x[9] + true_x[9];
   out_5129418956577183101[10] = -nom_x[10] + true_x[10];
   out_5129418956577183101[11] = -nom_x[11] + true_x[11];
   out_5129418956577183101[12] = -nom_x[12] + true_x[12];
   out_5129418956577183101[13] = -nom_x[13] + true_x[13];
   out_5129418956577183101[14] = -nom_x[14] + true_x[14];
   out_5129418956577183101[15] = -nom_x[15] + true_x[15];
   out_5129418956577183101[16] = -nom_x[16] + true_x[16];
   out_5129418956577183101[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_3902216871053882414) {
   out_3902216871053882414[0] = 1.0;
   out_3902216871053882414[1] = 0.0;
   out_3902216871053882414[2] = 0.0;
   out_3902216871053882414[3] = 0.0;
   out_3902216871053882414[4] = 0.0;
   out_3902216871053882414[5] = 0.0;
   out_3902216871053882414[6] = 0.0;
   out_3902216871053882414[7] = 0.0;
   out_3902216871053882414[8] = 0.0;
   out_3902216871053882414[9] = 0.0;
   out_3902216871053882414[10] = 0.0;
   out_3902216871053882414[11] = 0.0;
   out_3902216871053882414[12] = 0.0;
   out_3902216871053882414[13] = 0.0;
   out_3902216871053882414[14] = 0.0;
   out_3902216871053882414[15] = 0.0;
   out_3902216871053882414[16] = 0.0;
   out_3902216871053882414[17] = 0.0;
   out_3902216871053882414[18] = 0.0;
   out_3902216871053882414[19] = 1.0;
   out_3902216871053882414[20] = 0.0;
   out_3902216871053882414[21] = 0.0;
   out_3902216871053882414[22] = 0.0;
   out_3902216871053882414[23] = 0.0;
   out_3902216871053882414[24] = 0.0;
   out_3902216871053882414[25] = 0.0;
   out_3902216871053882414[26] = 0.0;
   out_3902216871053882414[27] = 0.0;
   out_3902216871053882414[28] = 0.0;
   out_3902216871053882414[29] = 0.0;
   out_3902216871053882414[30] = 0.0;
   out_3902216871053882414[31] = 0.0;
   out_3902216871053882414[32] = 0.0;
   out_3902216871053882414[33] = 0.0;
   out_3902216871053882414[34] = 0.0;
   out_3902216871053882414[35] = 0.0;
   out_3902216871053882414[36] = 0.0;
   out_3902216871053882414[37] = 0.0;
   out_3902216871053882414[38] = 1.0;
   out_3902216871053882414[39] = 0.0;
   out_3902216871053882414[40] = 0.0;
   out_3902216871053882414[41] = 0.0;
   out_3902216871053882414[42] = 0.0;
   out_3902216871053882414[43] = 0.0;
   out_3902216871053882414[44] = 0.0;
   out_3902216871053882414[45] = 0.0;
   out_3902216871053882414[46] = 0.0;
   out_3902216871053882414[47] = 0.0;
   out_3902216871053882414[48] = 0.0;
   out_3902216871053882414[49] = 0.0;
   out_3902216871053882414[50] = 0.0;
   out_3902216871053882414[51] = 0.0;
   out_3902216871053882414[52] = 0.0;
   out_3902216871053882414[53] = 0.0;
   out_3902216871053882414[54] = 0.0;
   out_3902216871053882414[55] = 0.0;
   out_3902216871053882414[56] = 0.0;
   out_3902216871053882414[57] = 1.0;
   out_3902216871053882414[58] = 0.0;
   out_3902216871053882414[59] = 0.0;
   out_3902216871053882414[60] = 0.0;
   out_3902216871053882414[61] = 0.0;
   out_3902216871053882414[62] = 0.0;
   out_3902216871053882414[63] = 0.0;
   out_3902216871053882414[64] = 0.0;
   out_3902216871053882414[65] = 0.0;
   out_3902216871053882414[66] = 0.0;
   out_3902216871053882414[67] = 0.0;
   out_3902216871053882414[68] = 0.0;
   out_3902216871053882414[69] = 0.0;
   out_3902216871053882414[70] = 0.0;
   out_3902216871053882414[71] = 0.0;
   out_3902216871053882414[72] = 0.0;
   out_3902216871053882414[73] = 0.0;
   out_3902216871053882414[74] = 0.0;
   out_3902216871053882414[75] = 0.0;
   out_3902216871053882414[76] = 1.0;
   out_3902216871053882414[77] = 0.0;
   out_3902216871053882414[78] = 0.0;
   out_3902216871053882414[79] = 0.0;
   out_3902216871053882414[80] = 0.0;
   out_3902216871053882414[81] = 0.0;
   out_3902216871053882414[82] = 0.0;
   out_3902216871053882414[83] = 0.0;
   out_3902216871053882414[84] = 0.0;
   out_3902216871053882414[85] = 0.0;
   out_3902216871053882414[86] = 0.0;
   out_3902216871053882414[87] = 0.0;
   out_3902216871053882414[88] = 0.0;
   out_3902216871053882414[89] = 0.0;
   out_3902216871053882414[90] = 0.0;
   out_3902216871053882414[91] = 0.0;
   out_3902216871053882414[92] = 0.0;
   out_3902216871053882414[93] = 0.0;
   out_3902216871053882414[94] = 0.0;
   out_3902216871053882414[95] = 1.0;
   out_3902216871053882414[96] = 0.0;
   out_3902216871053882414[97] = 0.0;
   out_3902216871053882414[98] = 0.0;
   out_3902216871053882414[99] = 0.0;
   out_3902216871053882414[100] = 0.0;
   out_3902216871053882414[101] = 0.0;
   out_3902216871053882414[102] = 0.0;
   out_3902216871053882414[103] = 0.0;
   out_3902216871053882414[104] = 0.0;
   out_3902216871053882414[105] = 0.0;
   out_3902216871053882414[106] = 0.0;
   out_3902216871053882414[107] = 0.0;
   out_3902216871053882414[108] = 0.0;
   out_3902216871053882414[109] = 0.0;
   out_3902216871053882414[110] = 0.0;
   out_3902216871053882414[111] = 0.0;
   out_3902216871053882414[112] = 0.0;
   out_3902216871053882414[113] = 0.0;
   out_3902216871053882414[114] = 1.0;
   out_3902216871053882414[115] = 0.0;
   out_3902216871053882414[116] = 0.0;
   out_3902216871053882414[117] = 0.0;
   out_3902216871053882414[118] = 0.0;
   out_3902216871053882414[119] = 0.0;
   out_3902216871053882414[120] = 0.0;
   out_3902216871053882414[121] = 0.0;
   out_3902216871053882414[122] = 0.0;
   out_3902216871053882414[123] = 0.0;
   out_3902216871053882414[124] = 0.0;
   out_3902216871053882414[125] = 0.0;
   out_3902216871053882414[126] = 0.0;
   out_3902216871053882414[127] = 0.0;
   out_3902216871053882414[128] = 0.0;
   out_3902216871053882414[129] = 0.0;
   out_3902216871053882414[130] = 0.0;
   out_3902216871053882414[131] = 0.0;
   out_3902216871053882414[132] = 0.0;
   out_3902216871053882414[133] = 1.0;
   out_3902216871053882414[134] = 0.0;
   out_3902216871053882414[135] = 0.0;
   out_3902216871053882414[136] = 0.0;
   out_3902216871053882414[137] = 0.0;
   out_3902216871053882414[138] = 0.0;
   out_3902216871053882414[139] = 0.0;
   out_3902216871053882414[140] = 0.0;
   out_3902216871053882414[141] = 0.0;
   out_3902216871053882414[142] = 0.0;
   out_3902216871053882414[143] = 0.0;
   out_3902216871053882414[144] = 0.0;
   out_3902216871053882414[145] = 0.0;
   out_3902216871053882414[146] = 0.0;
   out_3902216871053882414[147] = 0.0;
   out_3902216871053882414[148] = 0.0;
   out_3902216871053882414[149] = 0.0;
   out_3902216871053882414[150] = 0.0;
   out_3902216871053882414[151] = 0.0;
   out_3902216871053882414[152] = 1.0;
   out_3902216871053882414[153] = 0.0;
   out_3902216871053882414[154] = 0.0;
   out_3902216871053882414[155] = 0.0;
   out_3902216871053882414[156] = 0.0;
   out_3902216871053882414[157] = 0.0;
   out_3902216871053882414[158] = 0.0;
   out_3902216871053882414[159] = 0.0;
   out_3902216871053882414[160] = 0.0;
   out_3902216871053882414[161] = 0.0;
   out_3902216871053882414[162] = 0.0;
   out_3902216871053882414[163] = 0.0;
   out_3902216871053882414[164] = 0.0;
   out_3902216871053882414[165] = 0.0;
   out_3902216871053882414[166] = 0.0;
   out_3902216871053882414[167] = 0.0;
   out_3902216871053882414[168] = 0.0;
   out_3902216871053882414[169] = 0.0;
   out_3902216871053882414[170] = 0.0;
   out_3902216871053882414[171] = 1.0;
   out_3902216871053882414[172] = 0.0;
   out_3902216871053882414[173] = 0.0;
   out_3902216871053882414[174] = 0.0;
   out_3902216871053882414[175] = 0.0;
   out_3902216871053882414[176] = 0.0;
   out_3902216871053882414[177] = 0.0;
   out_3902216871053882414[178] = 0.0;
   out_3902216871053882414[179] = 0.0;
   out_3902216871053882414[180] = 0.0;
   out_3902216871053882414[181] = 0.0;
   out_3902216871053882414[182] = 0.0;
   out_3902216871053882414[183] = 0.0;
   out_3902216871053882414[184] = 0.0;
   out_3902216871053882414[185] = 0.0;
   out_3902216871053882414[186] = 0.0;
   out_3902216871053882414[187] = 0.0;
   out_3902216871053882414[188] = 0.0;
   out_3902216871053882414[189] = 0.0;
   out_3902216871053882414[190] = 1.0;
   out_3902216871053882414[191] = 0.0;
   out_3902216871053882414[192] = 0.0;
   out_3902216871053882414[193] = 0.0;
   out_3902216871053882414[194] = 0.0;
   out_3902216871053882414[195] = 0.0;
   out_3902216871053882414[196] = 0.0;
   out_3902216871053882414[197] = 0.0;
   out_3902216871053882414[198] = 0.0;
   out_3902216871053882414[199] = 0.0;
   out_3902216871053882414[200] = 0.0;
   out_3902216871053882414[201] = 0.0;
   out_3902216871053882414[202] = 0.0;
   out_3902216871053882414[203] = 0.0;
   out_3902216871053882414[204] = 0.0;
   out_3902216871053882414[205] = 0.0;
   out_3902216871053882414[206] = 0.0;
   out_3902216871053882414[207] = 0.0;
   out_3902216871053882414[208] = 0.0;
   out_3902216871053882414[209] = 1.0;
   out_3902216871053882414[210] = 0.0;
   out_3902216871053882414[211] = 0.0;
   out_3902216871053882414[212] = 0.0;
   out_3902216871053882414[213] = 0.0;
   out_3902216871053882414[214] = 0.0;
   out_3902216871053882414[215] = 0.0;
   out_3902216871053882414[216] = 0.0;
   out_3902216871053882414[217] = 0.0;
   out_3902216871053882414[218] = 0.0;
   out_3902216871053882414[219] = 0.0;
   out_3902216871053882414[220] = 0.0;
   out_3902216871053882414[221] = 0.0;
   out_3902216871053882414[222] = 0.0;
   out_3902216871053882414[223] = 0.0;
   out_3902216871053882414[224] = 0.0;
   out_3902216871053882414[225] = 0.0;
   out_3902216871053882414[226] = 0.0;
   out_3902216871053882414[227] = 0.0;
   out_3902216871053882414[228] = 1.0;
   out_3902216871053882414[229] = 0.0;
   out_3902216871053882414[230] = 0.0;
   out_3902216871053882414[231] = 0.0;
   out_3902216871053882414[232] = 0.0;
   out_3902216871053882414[233] = 0.0;
   out_3902216871053882414[234] = 0.0;
   out_3902216871053882414[235] = 0.0;
   out_3902216871053882414[236] = 0.0;
   out_3902216871053882414[237] = 0.0;
   out_3902216871053882414[238] = 0.0;
   out_3902216871053882414[239] = 0.0;
   out_3902216871053882414[240] = 0.0;
   out_3902216871053882414[241] = 0.0;
   out_3902216871053882414[242] = 0.0;
   out_3902216871053882414[243] = 0.0;
   out_3902216871053882414[244] = 0.0;
   out_3902216871053882414[245] = 0.0;
   out_3902216871053882414[246] = 0.0;
   out_3902216871053882414[247] = 1.0;
   out_3902216871053882414[248] = 0.0;
   out_3902216871053882414[249] = 0.0;
   out_3902216871053882414[250] = 0.0;
   out_3902216871053882414[251] = 0.0;
   out_3902216871053882414[252] = 0.0;
   out_3902216871053882414[253] = 0.0;
   out_3902216871053882414[254] = 0.0;
   out_3902216871053882414[255] = 0.0;
   out_3902216871053882414[256] = 0.0;
   out_3902216871053882414[257] = 0.0;
   out_3902216871053882414[258] = 0.0;
   out_3902216871053882414[259] = 0.0;
   out_3902216871053882414[260] = 0.0;
   out_3902216871053882414[261] = 0.0;
   out_3902216871053882414[262] = 0.0;
   out_3902216871053882414[263] = 0.0;
   out_3902216871053882414[264] = 0.0;
   out_3902216871053882414[265] = 0.0;
   out_3902216871053882414[266] = 1.0;
   out_3902216871053882414[267] = 0.0;
   out_3902216871053882414[268] = 0.0;
   out_3902216871053882414[269] = 0.0;
   out_3902216871053882414[270] = 0.0;
   out_3902216871053882414[271] = 0.0;
   out_3902216871053882414[272] = 0.0;
   out_3902216871053882414[273] = 0.0;
   out_3902216871053882414[274] = 0.0;
   out_3902216871053882414[275] = 0.0;
   out_3902216871053882414[276] = 0.0;
   out_3902216871053882414[277] = 0.0;
   out_3902216871053882414[278] = 0.0;
   out_3902216871053882414[279] = 0.0;
   out_3902216871053882414[280] = 0.0;
   out_3902216871053882414[281] = 0.0;
   out_3902216871053882414[282] = 0.0;
   out_3902216871053882414[283] = 0.0;
   out_3902216871053882414[284] = 0.0;
   out_3902216871053882414[285] = 1.0;
   out_3902216871053882414[286] = 0.0;
   out_3902216871053882414[287] = 0.0;
   out_3902216871053882414[288] = 0.0;
   out_3902216871053882414[289] = 0.0;
   out_3902216871053882414[290] = 0.0;
   out_3902216871053882414[291] = 0.0;
   out_3902216871053882414[292] = 0.0;
   out_3902216871053882414[293] = 0.0;
   out_3902216871053882414[294] = 0.0;
   out_3902216871053882414[295] = 0.0;
   out_3902216871053882414[296] = 0.0;
   out_3902216871053882414[297] = 0.0;
   out_3902216871053882414[298] = 0.0;
   out_3902216871053882414[299] = 0.0;
   out_3902216871053882414[300] = 0.0;
   out_3902216871053882414[301] = 0.0;
   out_3902216871053882414[302] = 0.0;
   out_3902216871053882414[303] = 0.0;
   out_3902216871053882414[304] = 1.0;
   out_3902216871053882414[305] = 0.0;
   out_3902216871053882414[306] = 0.0;
   out_3902216871053882414[307] = 0.0;
   out_3902216871053882414[308] = 0.0;
   out_3902216871053882414[309] = 0.0;
   out_3902216871053882414[310] = 0.0;
   out_3902216871053882414[311] = 0.0;
   out_3902216871053882414[312] = 0.0;
   out_3902216871053882414[313] = 0.0;
   out_3902216871053882414[314] = 0.0;
   out_3902216871053882414[315] = 0.0;
   out_3902216871053882414[316] = 0.0;
   out_3902216871053882414[317] = 0.0;
   out_3902216871053882414[318] = 0.0;
   out_3902216871053882414[319] = 0.0;
   out_3902216871053882414[320] = 0.0;
   out_3902216871053882414[321] = 0.0;
   out_3902216871053882414[322] = 0.0;
   out_3902216871053882414[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_6550423757343157361) {
   out_6550423757343157361[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_6550423757343157361[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_6550423757343157361[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_6550423757343157361[3] = dt*state[12] + state[3];
   out_6550423757343157361[4] = dt*state[13] + state[4];
   out_6550423757343157361[5] = dt*state[14] + state[5];
   out_6550423757343157361[6] = state[6];
   out_6550423757343157361[7] = state[7];
   out_6550423757343157361[8] = state[8];
   out_6550423757343157361[9] = state[9];
   out_6550423757343157361[10] = state[10];
   out_6550423757343157361[11] = state[11];
   out_6550423757343157361[12] = state[12];
   out_6550423757343157361[13] = state[13];
   out_6550423757343157361[14] = state[14];
   out_6550423757343157361[15] = state[15];
   out_6550423757343157361[16] = state[16];
   out_6550423757343157361[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3303653463282877516) {
   out_3303653463282877516[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3303653463282877516[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3303653463282877516[2] = 0;
   out_3303653463282877516[3] = 0;
   out_3303653463282877516[4] = 0;
   out_3303653463282877516[5] = 0;
   out_3303653463282877516[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3303653463282877516[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3303653463282877516[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3303653463282877516[9] = 0;
   out_3303653463282877516[10] = 0;
   out_3303653463282877516[11] = 0;
   out_3303653463282877516[12] = 0;
   out_3303653463282877516[13] = 0;
   out_3303653463282877516[14] = 0;
   out_3303653463282877516[15] = 0;
   out_3303653463282877516[16] = 0;
   out_3303653463282877516[17] = 0;
   out_3303653463282877516[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3303653463282877516[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3303653463282877516[20] = 0;
   out_3303653463282877516[21] = 0;
   out_3303653463282877516[22] = 0;
   out_3303653463282877516[23] = 0;
   out_3303653463282877516[24] = 0;
   out_3303653463282877516[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3303653463282877516[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3303653463282877516[27] = 0;
   out_3303653463282877516[28] = 0;
   out_3303653463282877516[29] = 0;
   out_3303653463282877516[30] = 0;
   out_3303653463282877516[31] = 0;
   out_3303653463282877516[32] = 0;
   out_3303653463282877516[33] = 0;
   out_3303653463282877516[34] = 0;
   out_3303653463282877516[35] = 0;
   out_3303653463282877516[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3303653463282877516[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3303653463282877516[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3303653463282877516[39] = 0;
   out_3303653463282877516[40] = 0;
   out_3303653463282877516[41] = 0;
   out_3303653463282877516[42] = 0;
   out_3303653463282877516[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3303653463282877516[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3303653463282877516[45] = 0;
   out_3303653463282877516[46] = 0;
   out_3303653463282877516[47] = 0;
   out_3303653463282877516[48] = 0;
   out_3303653463282877516[49] = 0;
   out_3303653463282877516[50] = 0;
   out_3303653463282877516[51] = 0;
   out_3303653463282877516[52] = 0;
   out_3303653463282877516[53] = 0;
   out_3303653463282877516[54] = 0;
   out_3303653463282877516[55] = 0;
   out_3303653463282877516[56] = 0;
   out_3303653463282877516[57] = 1;
   out_3303653463282877516[58] = 0;
   out_3303653463282877516[59] = 0;
   out_3303653463282877516[60] = 0;
   out_3303653463282877516[61] = 0;
   out_3303653463282877516[62] = 0;
   out_3303653463282877516[63] = 0;
   out_3303653463282877516[64] = 0;
   out_3303653463282877516[65] = 0;
   out_3303653463282877516[66] = dt;
   out_3303653463282877516[67] = 0;
   out_3303653463282877516[68] = 0;
   out_3303653463282877516[69] = 0;
   out_3303653463282877516[70] = 0;
   out_3303653463282877516[71] = 0;
   out_3303653463282877516[72] = 0;
   out_3303653463282877516[73] = 0;
   out_3303653463282877516[74] = 0;
   out_3303653463282877516[75] = 0;
   out_3303653463282877516[76] = 1;
   out_3303653463282877516[77] = 0;
   out_3303653463282877516[78] = 0;
   out_3303653463282877516[79] = 0;
   out_3303653463282877516[80] = 0;
   out_3303653463282877516[81] = 0;
   out_3303653463282877516[82] = 0;
   out_3303653463282877516[83] = 0;
   out_3303653463282877516[84] = 0;
   out_3303653463282877516[85] = dt;
   out_3303653463282877516[86] = 0;
   out_3303653463282877516[87] = 0;
   out_3303653463282877516[88] = 0;
   out_3303653463282877516[89] = 0;
   out_3303653463282877516[90] = 0;
   out_3303653463282877516[91] = 0;
   out_3303653463282877516[92] = 0;
   out_3303653463282877516[93] = 0;
   out_3303653463282877516[94] = 0;
   out_3303653463282877516[95] = 1;
   out_3303653463282877516[96] = 0;
   out_3303653463282877516[97] = 0;
   out_3303653463282877516[98] = 0;
   out_3303653463282877516[99] = 0;
   out_3303653463282877516[100] = 0;
   out_3303653463282877516[101] = 0;
   out_3303653463282877516[102] = 0;
   out_3303653463282877516[103] = 0;
   out_3303653463282877516[104] = dt;
   out_3303653463282877516[105] = 0;
   out_3303653463282877516[106] = 0;
   out_3303653463282877516[107] = 0;
   out_3303653463282877516[108] = 0;
   out_3303653463282877516[109] = 0;
   out_3303653463282877516[110] = 0;
   out_3303653463282877516[111] = 0;
   out_3303653463282877516[112] = 0;
   out_3303653463282877516[113] = 0;
   out_3303653463282877516[114] = 1;
   out_3303653463282877516[115] = 0;
   out_3303653463282877516[116] = 0;
   out_3303653463282877516[117] = 0;
   out_3303653463282877516[118] = 0;
   out_3303653463282877516[119] = 0;
   out_3303653463282877516[120] = 0;
   out_3303653463282877516[121] = 0;
   out_3303653463282877516[122] = 0;
   out_3303653463282877516[123] = 0;
   out_3303653463282877516[124] = 0;
   out_3303653463282877516[125] = 0;
   out_3303653463282877516[126] = 0;
   out_3303653463282877516[127] = 0;
   out_3303653463282877516[128] = 0;
   out_3303653463282877516[129] = 0;
   out_3303653463282877516[130] = 0;
   out_3303653463282877516[131] = 0;
   out_3303653463282877516[132] = 0;
   out_3303653463282877516[133] = 1;
   out_3303653463282877516[134] = 0;
   out_3303653463282877516[135] = 0;
   out_3303653463282877516[136] = 0;
   out_3303653463282877516[137] = 0;
   out_3303653463282877516[138] = 0;
   out_3303653463282877516[139] = 0;
   out_3303653463282877516[140] = 0;
   out_3303653463282877516[141] = 0;
   out_3303653463282877516[142] = 0;
   out_3303653463282877516[143] = 0;
   out_3303653463282877516[144] = 0;
   out_3303653463282877516[145] = 0;
   out_3303653463282877516[146] = 0;
   out_3303653463282877516[147] = 0;
   out_3303653463282877516[148] = 0;
   out_3303653463282877516[149] = 0;
   out_3303653463282877516[150] = 0;
   out_3303653463282877516[151] = 0;
   out_3303653463282877516[152] = 1;
   out_3303653463282877516[153] = 0;
   out_3303653463282877516[154] = 0;
   out_3303653463282877516[155] = 0;
   out_3303653463282877516[156] = 0;
   out_3303653463282877516[157] = 0;
   out_3303653463282877516[158] = 0;
   out_3303653463282877516[159] = 0;
   out_3303653463282877516[160] = 0;
   out_3303653463282877516[161] = 0;
   out_3303653463282877516[162] = 0;
   out_3303653463282877516[163] = 0;
   out_3303653463282877516[164] = 0;
   out_3303653463282877516[165] = 0;
   out_3303653463282877516[166] = 0;
   out_3303653463282877516[167] = 0;
   out_3303653463282877516[168] = 0;
   out_3303653463282877516[169] = 0;
   out_3303653463282877516[170] = 0;
   out_3303653463282877516[171] = 1;
   out_3303653463282877516[172] = 0;
   out_3303653463282877516[173] = 0;
   out_3303653463282877516[174] = 0;
   out_3303653463282877516[175] = 0;
   out_3303653463282877516[176] = 0;
   out_3303653463282877516[177] = 0;
   out_3303653463282877516[178] = 0;
   out_3303653463282877516[179] = 0;
   out_3303653463282877516[180] = 0;
   out_3303653463282877516[181] = 0;
   out_3303653463282877516[182] = 0;
   out_3303653463282877516[183] = 0;
   out_3303653463282877516[184] = 0;
   out_3303653463282877516[185] = 0;
   out_3303653463282877516[186] = 0;
   out_3303653463282877516[187] = 0;
   out_3303653463282877516[188] = 0;
   out_3303653463282877516[189] = 0;
   out_3303653463282877516[190] = 1;
   out_3303653463282877516[191] = 0;
   out_3303653463282877516[192] = 0;
   out_3303653463282877516[193] = 0;
   out_3303653463282877516[194] = 0;
   out_3303653463282877516[195] = 0;
   out_3303653463282877516[196] = 0;
   out_3303653463282877516[197] = 0;
   out_3303653463282877516[198] = 0;
   out_3303653463282877516[199] = 0;
   out_3303653463282877516[200] = 0;
   out_3303653463282877516[201] = 0;
   out_3303653463282877516[202] = 0;
   out_3303653463282877516[203] = 0;
   out_3303653463282877516[204] = 0;
   out_3303653463282877516[205] = 0;
   out_3303653463282877516[206] = 0;
   out_3303653463282877516[207] = 0;
   out_3303653463282877516[208] = 0;
   out_3303653463282877516[209] = 1;
   out_3303653463282877516[210] = 0;
   out_3303653463282877516[211] = 0;
   out_3303653463282877516[212] = 0;
   out_3303653463282877516[213] = 0;
   out_3303653463282877516[214] = 0;
   out_3303653463282877516[215] = 0;
   out_3303653463282877516[216] = 0;
   out_3303653463282877516[217] = 0;
   out_3303653463282877516[218] = 0;
   out_3303653463282877516[219] = 0;
   out_3303653463282877516[220] = 0;
   out_3303653463282877516[221] = 0;
   out_3303653463282877516[222] = 0;
   out_3303653463282877516[223] = 0;
   out_3303653463282877516[224] = 0;
   out_3303653463282877516[225] = 0;
   out_3303653463282877516[226] = 0;
   out_3303653463282877516[227] = 0;
   out_3303653463282877516[228] = 1;
   out_3303653463282877516[229] = 0;
   out_3303653463282877516[230] = 0;
   out_3303653463282877516[231] = 0;
   out_3303653463282877516[232] = 0;
   out_3303653463282877516[233] = 0;
   out_3303653463282877516[234] = 0;
   out_3303653463282877516[235] = 0;
   out_3303653463282877516[236] = 0;
   out_3303653463282877516[237] = 0;
   out_3303653463282877516[238] = 0;
   out_3303653463282877516[239] = 0;
   out_3303653463282877516[240] = 0;
   out_3303653463282877516[241] = 0;
   out_3303653463282877516[242] = 0;
   out_3303653463282877516[243] = 0;
   out_3303653463282877516[244] = 0;
   out_3303653463282877516[245] = 0;
   out_3303653463282877516[246] = 0;
   out_3303653463282877516[247] = 1;
   out_3303653463282877516[248] = 0;
   out_3303653463282877516[249] = 0;
   out_3303653463282877516[250] = 0;
   out_3303653463282877516[251] = 0;
   out_3303653463282877516[252] = 0;
   out_3303653463282877516[253] = 0;
   out_3303653463282877516[254] = 0;
   out_3303653463282877516[255] = 0;
   out_3303653463282877516[256] = 0;
   out_3303653463282877516[257] = 0;
   out_3303653463282877516[258] = 0;
   out_3303653463282877516[259] = 0;
   out_3303653463282877516[260] = 0;
   out_3303653463282877516[261] = 0;
   out_3303653463282877516[262] = 0;
   out_3303653463282877516[263] = 0;
   out_3303653463282877516[264] = 0;
   out_3303653463282877516[265] = 0;
   out_3303653463282877516[266] = 1;
   out_3303653463282877516[267] = 0;
   out_3303653463282877516[268] = 0;
   out_3303653463282877516[269] = 0;
   out_3303653463282877516[270] = 0;
   out_3303653463282877516[271] = 0;
   out_3303653463282877516[272] = 0;
   out_3303653463282877516[273] = 0;
   out_3303653463282877516[274] = 0;
   out_3303653463282877516[275] = 0;
   out_3303653463282877516[276] = 0;
   out_3303653463282877516[277] = 0;
   out_3303653463282877516[278] = 0;
   out_3303653463282877516[279] = 0;
   out_3303653463282877516[280] = 0;
   out_3303653463282877516[281] = 0;
   out_3303653463282877516[282] = 0;
   out_3303653463282877516[283] = 0;
   out_3303653463282877516[284] = 0;
   out_3303653463282877516[285] = 1;
   out_3303653463282877516[286] = 0;
   out_3303653463282877516[287] = 0;
   out_3303653463282877516[288] = 0;
   out_3303653463282877516[289] = 0;
   out_3303653463282877516[290] = 0;
   out_3303653463282877516[291] = 0;
   out_3303653463282877516[292] = 0;
   out_3303653463282877516[293] = 0;
   out_3303653463282877516[294] = 0;
   out_3303653463282877516[295] = 0;
   out_3303653463282877516[296] = 0;
   out_3303653463282877516[297] = 0;
   out_3303653463282877516[298] = 0;
   out_3303653463282877516[299] = 0;
   out_3303653463282877516[300] = 0;
   out_3303653463282877516[301] = 0;
   out_3303653463282877516[302] = 0;
   out_3303653463282877516[303] = 0;
   out_3303653463282877516[304] = 1;
   out_3303653463282877516[305] = 0;
   out_3303653463282877516[306] = 0;
   out_3303653463282877516[307] = 0;
   out_3303653463282877516[308] = 0;
   out_3303653463282877516[309] = 0;
   out_3303653463282877516[310] = 0;
   out_3303653463282877516[311] = 0;
   out_3303653463282877516[312] = 0;
   out_3303653463282877516[313] = 0;
   out_3303653463282877516[314] = 0;
   out_3303653463282877516[315] = 0;
   out_3303653463282877516[316] = 0;
   out_3303653463282877516[317] = 0;
   out_3303653463282877516[318] = 0;
   out_3303653463282877516[319] = 0;
   out_3303653463282877516[320] = 0;
   out_3303653463282877516[321] = 0;
   out_3303653463282877516[322] = 0;
   out_3303653463282877516[323] = 1;
}
void h_4(double *state, double *unused, double *out_4870156935976554141) {
   out_4870156935976554141[0] = state[6] + state[9];
   out_4870156935976554141[1] = state[7] + state[10];
   out_4870156935976554141[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_9006241895721244560) {
   out_9006241895721244560[0] = 0;
   out_9006241895721244560[1] = 0;
   out_9006241895721244560[2] = 0;
   out_9006241895721244560[3] = 0;
   out_9006241895721244560[4] = 0;
   out_9006241895721244560[5] = 0;
   out_9006241895721244560[6] = 1;
   out_9006241895721244560[7] = 0;
   out_9006241895721244560[8] = 0;
   out_9006241895721244560[9] = 1;
   out_9006241895721244560[10] = 0;
   out_9006241895721244560[11] = 0;
   out_9006241895721244560[12] = 0;
   out_9006241895721244560[13] = 0;
   out_9006241895721244560[14] = 0;
   out_9006241895721244560[15] = 0;
   out_9006241895721244560[16] = 0;
   out_9006241895721244560[17] = 0;
   out_9006241895721244560[18] = 0;
   out_9006241895721244560[19] = 0;
   out_9006241895721244560[20] = 0;
   out_9006241895721244560[21] = 0;
   out_9006241895721244560[22] = 0;
   out_9006241895721244560[23] = 0;
   out_9006241895721244560[24] = 0;
   out_9006241895721244560[25] = 1;
   out_9006241895721244560[26] = 0;
   out_9006241895721244560[27] = 0;
   out_9006241895721244560[28] = 1;
   out_9006241895721244560[29] = 0;
   out_9006241895721244560[30] = 0;
   out_9006241895721244560[31] = 0;
   out_9006241895721244560[32] = 0;
   out_9006241895721244560[33] = 0;
   out_9006241895721244560[34] = 0;
   out_9006241895721244560[35] = 0;
   out_9006241895721244560[36] = 0;
   out_9006241895721244560[37] = 0;
   out_9006241895721244560[38] = 0;
   out_9006241895721244560[39] = 0;
   out_9006241895721244560[40] = 0;
   out_9006241895721244560[41] = 0;
   out_9006241895721244560[42] = 0;
   out_9006241895721244560[43] = 0;
   out_9006241895721244560[44] = 1;
   out_9006241895721244560[45] = 0;
   out_9006241895721244560[46] = 0;
   out_9006241895721244560[47] = 1;
   out_9006241895721244560[48] = 0;
   out_9006241895721244560[49] = 0;
   out_9006241895721244560[50] = 0;
   out_9006241895721244560[51] = 0;
   out_9006241895721244560[52] = 0;
   out_9006241895721244560[53] = 0;
}
void h_10(double *state, double *unused, double *out_6725094615089616867) {
   out_6725094615089616867[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_6725094615089616867[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_6725094615089616867[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5824813145068267930) {
   out_5824813145068267930[0] = 0;
   out_5824813145068267930[1] = 9.8100000000000005*cos(state[1]);
   out_5824813145068267930[2] = 0;
   out_5824813145068267930[3] = 0;
   out_5824813145068267930[4] = -state[8];
   out_5824813145068267930[5] = state[7];
   out_5824813145068267930[6] = 0;
   out_5824813145068267930[7] = state[5];
   out_5824813145068267930[8] = -state[4];
   out_5824813145068267930[9] = 0;
   out_5824813145068267930[10] = 0;
   out_5824813145068267930[11] = 0;
   out_5824813145068267930[12] = 1;
   out_5824813145068267930[13] = 0;
   out_5824813145068267930[14] = 0;
   out_5824813145068267930[15] = 1;
   out_5824813145068267930[16] = 0;
   out_5824813145068267930[17] = 0;
   out_5824813145068267930[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5824813145068267930[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5824813145068267930[20] = 0;
   out_5824813145068267930[21] = state[8];
   out_5824813145068267930[22] = 0;
   out_5824813145068267930[23] = -state[6];
   out_5824813145068267930[24] = -state[5];
   out_5824813145068267930[25] = 0;
   out_5824813145068267930[26] = state[3];
   out_5824813145068267930[27] = 0;
   out_5824813145068267930[28] = 0;
   out_5824813145068267930[29] = 0;
   out_5824813145068267930[30] = 0;
   out_5824813145068267930[31] = 1;
   out_5824813145068267930[32] = 0;
   out_5824813145068267930[33] = 0;
   out_5824813145068267930[34] = 1;
   out_5824813145068267930[35] = 0;
   out_5824813145068267930[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5824813145068267930[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5824813145068267930[38] = 0;
   out_5824813145068267930[39] = -state[7];
   out_5824813145068267930[40] = state[6];
   out_5824813145068267930[41] = 0;
   out_5824813145068267930[42] = state[4];
   out_5824813145068267930[43] = -state[3];
   out_5824813145068267930[44] = 0;
   out_5824813145068267930[45] = 0;
   out_5824813145068267930[46] = 0;
   out_5824813145068267930[47] = 0;
   out_5824813145068267930[48] = 0;
   out_5824813145068267930[49] = 0;
   out_5824813145068267930[50] = 1;
   out_5824813145068267930[51] = 0;
   out_5824813145068267930[52] = 0;
   out_5824813145068267930[53] = 1;
}
void h_13(double *state, double *unused, double *out_1352228507741542507) {
   out_1352228507741542507[0] = state[3];
   out_1352228507741542507[1] = state[4];
   out_1352228507741542507[2] = state[5];
}
void H_13(double *state, double *unused, double *out_1829870969671606127) {
   out_1829870969671606127[0] = 0;
   out_1829870969671606127[1] = 0;
   out_1829870969671606127[2] = 0;
   out_1829870969671606127[3] = 1;
   out_1829870969671606127[4] = 0;
   out_1829870969671606127[5] = 0;
   out_1829870969671606127[6] = 0;
   out_1829870969671606127[7] = 0;
   out_1829870969671606127[8] = 0;
   out_1829870969671606127[9] = 0;
   out_1829870969671606127[10] = 0;
   out_1829870969671606127[11] = 0;
   out_1829870969671606127[12] = 0;
   out_1829870969671606127[13] = 0;
   out_1829870969671606127[14] = 0;
   out_1829870969671606127[15] = 0;
   out_1829870969671606127[16] = 0;
   out_1829870969671606127[17] = 0;
   out_1829870969671606127[18] = 0;
   out_1829870969671606127[19] = 0;
   out_1829870969671606127[20] = 0;
   out_1829870969671606127[21] = 0;
   out_1829870969671606127[22] = 1;
   out_1829870969671606127[23] = 0;
   out_1829870969671606127[24] = 0;
   out_1829870969671606127[25] = 0;
   out_1829870969671606127[26] = 0;
   out_1829870969671606127[27] = 0;
   out_1829870969671606127[28] = 0;
   out_1829870969671606127[29] = 0;
   out_1829870969671606127[30] = 0;
   out_1829870969671606127[31] = 0;
   out_1829870969671606127[32] = 0;
   out_1829870969671606127[33] = 0;
   out_1829870969671606127[34] = 0;
   out_1829870969671606127[35] = 0;
   out_1829870969671606127[36] = 0;
   out_1829870969671606127[37] = 0;
   out_1829870969671606127[38] = 0;
   out_1829870969671606127[39] = 0;
   out_1829870969671606127[40] = 0;
   out_1829870969671606127[41] = 1;
   out_1829870969671606127[42] = 0;
   out_1829870969671606127[43] = 0;
   out_1829870969671606127[44] = 0;
   out_1829870969671606127[45] = 0;
   out_1829870969671606127[46] = 0;
   out_1829870969671606127[47] = 0;
   out_1829870969671606127[48] = 0;
   out_1829870969671606127[49] = 0;
   out_1829870969671606127[50] = 0;
   out_1829870969671606127[51] = 0;
   out_1829870969671606127[52] = 0;
   out_1829870969671606127[53] = 0;
}
void h_14(double *state, double *unused, double *out_8447259540628755835) {
   out_8447259540628755835[0] = state[6];
   out_8447259540628755835[1] = state[7];
   out_8447259540628755835[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5477261321648822527) {
   out_5477261321648822527[0] = 0;
   out_5477261321648822527[1] = 0;
   out_5477261321648822527[2] = 0;
   out_5477261321648822527[3] = 0;
   out_5477261321648822527[4] = 0;
   out_5477261321648822527[5] = 0;
   out_5477261321648822527[6] = 1;
   out_5477261321648822527[7] = 0;
   out_5477261321648822527[8] = 0;
   out_5477261321648822527[9] = 0;
   out_5477261321648822527[10] = 0;
   out_5477261321648822527[11] = 0;
   out_5477261321648822527[12] = 0;
   out_5477261321648822527[13] = 0;
   out_5477261321648822527[14] = 0;
   out_5477261321648822527[15] = 0;
   out_5477261321648822527[16] = 0;
   out_5477261321648822527[17] = 0;
   out_5477261321648822527[18] = 0;
   out_5477261321648822527[19] = 0;
   out_5477261321648822527[20] = 0;
   out_5477261321648822527[21] = 0;
   out_5477261321648822527[22] = 0;
   out_5477261321648822527[23] = 0;
   out_5477261321648822527[24] = 0;
   out_5477261321648822527[25] = 1;
   out_5477261321648822527[26] = 0;
   out_5477261321648822527[27] = 0;
   out_5477261321648822527[28] = 0;
   out_5477261321648822527[29] = 0;
   out_5477261321648822527[30] = 0;
   out_5477261321648822527[31] = 0;
   out_5477261321648822527[32] = 0;
   out_5477261321648822527[33] = 0;
   out_5477261321648822527[34] = 0;
   out_5477261321648822527[35] = 0;
   out_5477261321648822527[36] = 0;
   out_5477261321648822527[37] = 0;
   out_5477261321648822527[38] = 0;
   out_5477261321648822527[39] = 0;
   out_5477261321648822527[40] = 0;
   out_5477261321648822527[41] = 0;
   out_5477261321648822527[42] = 0;
   out_5477261321648822527[43] = 0;
   out_5477261321648822527[44] = 1;
   out_5477261321648822527[45] = 0;
   out_5477261321648822527[46] = 0;
   out_5477261321648822527[47] = 0;
   out_5477261321648822527[48] = 0;
   out_5477261321648822527[49] = 0;
   out_5477261321648822527[50] = 0;
   out_5477261321648822527[51] = 0;
   out_5477261321648822527[52] = 0;
   out_5477261321648822527[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_8408857391730755841) {
  err_fun(nom_x, delta_x, out_8408857391730755841);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5129418956577183101) {
  inv_err_fun(nom_x, true_x, out_5129418956577183101);
}
void pose_H_mod_fun(double *state, double *out_3902216871053882414) {
  H_mod_fun(state, out_3902216871053882414);
}
void pose_f_fun(double *state, double dt, double *out_6550423757343157361) {
  f_fun(state,  dt, out_6550423757343157361);
}
void pose_F_fun(double *state, double dt, double *out_3303653463282877516) {
  F_fun(state,  dt, out_3303653463282877516);
}
void pose_h_4(double *state, double *unused, double *out_4870156935976554141) {
  h_4(state, unused, out_4870156935976554141);
}
void pose_H_4(double *state, double *unused, double *out_9006241895721244560) {
  H_4(state, unused, out_9006241895721244560);
}
void pose_h_10(double *state, double *unused, double *out_6725094615089616867) {
  h_10(state, unused, out_6725094615089616867);
}
void pose_H_10(double *state, double *unused, double *out_5824813145068267930) {
  H_10(state, unused, out_5824813145068267930);
}
void pose_h_13(double *state, double *unused, double *out_1352228507741542507) {
  h_13(state, unused, out_1352228507741542507);
}
void pose_H_13(double *state, double *unused, double *out_1829870969671606127) {
  H_13(state, unused, out_1829870969671606127);
}
void pose_h_14(double *state, double *unused, double *out_8447259540628755835) {
  h_14(state, unused, out_8447259540628755835);
}
void pose_H_14(double *state, double *unused, double *out_5477261321648822527) {
  H_14(state, unused, out_5477261321648822527);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
