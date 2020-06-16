
#include "amici/symbolic_functions.h"
#include "amici/defines.h" //realtype definition
typedef amici::realtype realtype;
#include <cmath> 

using namespace amici;

namespace amici {

namespace model_model_jakstat_adjoint_o2{

void dxdotdp_model_jakstat_adjoint_o2(realtype *dxdotdp, const realtype t, const realtype *x, const realtype *p, const realtype *k, const realtype *h, const int ip, const realtype *w, const realtype *dwdp) {
switch (ip) {
  case 0: {
  dxdotdp[0] = -k[0]*w[0]*x[0]*w[2];
  dxdotdp[1] = w[0]*x[0];
  dxdotdp[9] = -w[0]*x[9];
  dxdotdp[10] = w[0]*x[9];
  dxdotdp[18] = -w[0]*x[18];
  dxdotdp[19] = w[0]*x[18];
  dxdotdp[27] = -w[0]*x[27];
  dxdotdp[28] = w[0]*x[27];
  dxdotdp[36] = -w[0]*x[36];
  dxdotdp[37] = w[0]*x[36];
  dxdotdp[45] = -w[0]*x[45];
  dxdotdp[46] = w[0]*x[45];
  dxdotdp[54] = -x[0]*w[5]-w[0]*x[54];
  dxdotdp[55] = x[0]*w[5]+w[0]*x[54];
  dxdotdp[63] = -x[0]*w[6]-w[0]*x[63];
  dxdotdp[64] = x[0]*w[6]+w[0]*x[63];
  dxdotdp[72] = -x[0]*w[7]-w[0]*x[72];
  dxdotdp[73] = x[0]*w[7]+w[0]*x[72];
  dxdotdp[81] = -x[0]*w[8]-w[0]*x[81];
  dxdotdp[82] = x[0]*w[8]+w[0]*x[81];
  dxdotdp[90] = -x[0]*w[9]-w[0]*x[90];
  dxdotdp[91] = x[0]*w[9]+w[0]*x[90];
  dxdotdp[99] = -w[0]*x[99];
  dxdotdp[100] = w[0]*x[99];
  dxdotdp[108] = -w[0]*x[108];
  dxdotdp[109] = w[0]*x[108];
  dxdotdp[117] = -w[0]*x[117];
  dxdotdp[118] = w[0]*x[117];
  dxdotdp[126] = -w[0]*x[126];
  dxdotdp[127] = w[0]*x[126];
  dxdotdp[135] = -w[0]*x[135];
  dxdotdp[136] = w[0]*x[135];
  dxdotdp[144] = -w[0]*x[144];
  dxdotdp[145] = w[0]*x[144];
  dxdotdp[153] = -w[0]*x[153];
  dxdotdp[154] = w[0]*x[153];

  } break;

  case 1: {
  dxdotdp[1] = w[1]*-2.0;
  dxdotdp[2] = w[1];
  dxdotdp[10] = x[1]*x[10]*-4.0;
  dxdotdp[11] = x[1]*x[10]*2.0;
  dxdotdp[19] = x[1]*x[19]*-4.0;
  dxdotdp[20] = x[1]*x[19]*2.0;
  dxdotdp[28] = x[1]*x[28]*-4.0;
  dxdotdp[29] = x[1]*x[28]*2.0;
  dxdotdp[37] = x[1]*x[37]*-4.0;
  dxdotdp[38] = x[1]*x[37]*2.0;
  dxdotdp[46] = x[1]*x[46]*-4.0;
  dxdotdp[47] = x[1]*x[46]*2.0;
  dxdotdp[55] = x[1]*x[55]*-4.0;
  dxdotdp[56] = x[1]*x[55]*2.0;
  dxdotdp[64] = x[1]*x[64]*-4.0;
  dxdotdp[65] = x[1]*x[64]*2.0;
  dxdotdp[73] = x[1]*x[73]*-4.0;
  dxdotdp[74] = x[1]*x[73]*2.0;
  dxdotdp[82] = x[1]*x[82]*-4.0;
  dxdotdp[83] = x[1]*x[82]*2.0;
  dxdotdp[91] = x[1]*x[91]*-4.0;
  dxdotdp[92] = x[1]*x[91]*2.0;
  dxdotdp[100] = x[1]*x[100]*-4.0;
  dxdotdp[101] = x[1]*x[100]*2.0;
  dxdotdp[109] = x[1]*x[109]*-4.0;
  dxdotdp[110] = x[1]*x[109]*2.0;
  dxdotdp[118] = x[1]*x[118]*-4.0;
  dxdotdp[119] = x[1]*x[118]*2.0;
  dxdotdp[127] = x[1]*x[127]*-4.0;
  dxdotdp[128] = x[1]*x[127]*2.0;
  dxdotdp[136] = x[1]*x[136]*-4.0;
  dxdotdp[137] = x[1]*x[136]*2.0;
  dxdotdp[145] = x[1]*x[145]*-4.0;
  dxdotdp[146] = x[1]*x[145]*2.0;
  dxdotdp[154] = x[1]*x[154]*-4.0;
  dxdotdp[155] = x[1]*x[154]*2.0;

  } break;

  case 2: {
  dxdotdp[2] = -x[2];
  dxdotdp[3] = k[0]*w[3]*x[2];
  dxdotdp[11] = -x[11];
  dxdotdp[12] = k[0]*w[3]*x[11];
  dxdotdp[20] = -x[20];
  dxdotdp[21] = k[0]*w[3]*x[20];
  dxdotdp[29] = -x[29];
  dxdotdp[30] = k[0]*w[3]*x[29];
  dxdotdp[38] = -x[38];
  dxdotdp[39] = k[0]*w[3]*x[38];
  dxdotdp[47] = -x[47];
  dxdotdp[48] = k[0]*w[3]*x[47];
  dxdotdp[56] = -x[56];
  dxdotdp[57] = k[0]*w[3]*x[56];
  dxdotdp[65] = -x[65];
  dxdotdp[66] = k[0]*w[3]*x[65];
  dxdotdp[74] = -x[74];
  dxdotdp[75] = k[0]*w[3]*x[74];
  dxdotdp[83] = -x[83];
  dxdotdp[84] = k[0]*w[3]*x[83];
  dxdotdp[92] = -x[92];
  dxdotdp[93] = k[0]*w[3]*x[92];
  dxdotdp[101] = -x[101];
  dxdotdp[102] = k[0]*w[3]*x[101];
  dxdotdp[110] = -x[110];
  dxdotdp[111] = k[0]*w[3]*x[110];
  dxdotdp[119] = -x[119];
  dxdotdp[120] = k[0]*w[3]*x[119];
  dxdotdp[128] = -x[128];
  dxdotdp[129] = k[0]*w[3]*x[128];
  dxdotdp[137] = -x[137];
  dxdotdp[138] = k[0]*w[3]*x[137];
  dxdotdp[146] = -x[146];
  dxdotdp[147] = k[0]*w[3]*x[146];
  dxdotdp[155] = -x[155];
  dxdotdp[156] = k[0]*w[3]*x[155];

  } break;

  case 3: {
  dxdotdp[0] = k[1]*w[2]*x[8];
  dxdotdp[3] = -k[1]*w[3]*x[3];
  dxdotdp[4] = w[4]-x[4];
  dxdotdp[5] = x[4]-x[5];
  dxdotdp[6] = x[5]-x[6];
  dxdotdp[7] = x[6]-x[7];
  dxdotdp[8] = x[7]-x[8];
  dxdotdp[9] = k[1]*w[2]*x[17];
  dxdotdp[12] = -x[12];
  dxdotdp[13] = x[12]*2.0-x[13];
  dxdotdp[14] = x[13]-x[14];
  dxdotdp[15] = x[14]-x[15];
  dxdotdp[16] = x[15]-x[16];
  dxdotdp[17] = x[16]-x[17];
  dxdotdp[18] = k[1]*w[2]*x[26];
  dxdotdp[21] = -x[21];
  dxdotdp[22] = x[21]*2.0-x[22];
  dxdotdp[23] = x[22]-x[23];
  dxdotdp[24] = x[23]-x[24];
  dxdotdp[25] = x[24]-x[25];
  dxdotdp[26] = x[25]-x[26];
  dxdotdp[27] = k[1]*w[2]*x[35];
  dxdotdp[30] = -x[30];
  dxdotdp[31] = x[30]*2.0-x[31];
  dxdotdp[32] = x[31]-x[32];
  dxdotdp[33] = x[32]-x[33];
  dxdotdp[34] = x[33]-x[34];
  dxdotdp[35] = x[34]-x[35];
  dxdotdp[36] = k[1]*w[2]*x[44];
  dxdotdp[39] = -x[39];
  dxdotdp[40] = x[39]*2.0-x[40];
  dxdotdp[41] = x[40]-x[41];
  dxdotdp[42] = x[41]-x[42];
  dxdotdp[43] = x[42]-x[43];
  dxdotdp[44] = x[43]-x[44];
  dxdotdp[45] = k[1]*w[2]*x[53];
  dxdotdp[48] = -x[48];
  dxdotdp[49] = x[48]*2.0-x[49];
  dxdotdp[50] = x[49]-x[50];
  dxdotdp[51] = x[50]-x[51];
  dxdotdp[52] = x[51]-x[52];
  dxdotdp[53] = x[52]-x[53];
  dxdotdp[54] = k[1]*w[2]*x[62];
  dxdotdp[57] = -x[57];
  dxdotdp[58] = x[57]*2.0-x[58];
  dxdotdp[59] = x[58]-x[59];
  dxdotdp[60] = x[59]-x[60];
  dxdotdp[61] = x[60]-x[61];
  dxdotdp[62] = x[61]-x[62];
  dxdotdp[63] = k[1]*w[2]*x[71];
  dxdotdp[66] = -x[66];
  dxdotdp[67] = x[66]*2.0-x[67];
  dxdotdp[68] = x[67]-x[68];
  dxdotdp[69] = x[68]-x[69];
  dxdotdp[70] = x[69]-x[70];
  dxdotdp[71] = x[70]-x[71];
  dxdotdp[72] = k[1]*w[2]*x[80];
  dxdotdp[75] = -x[75];
  dxdotdp[76] = x[75]*2.0-x[76];
  dxdotdp[77] = x[76]-x[77];
  dxdotdp[78] = x[77]-x[78];
  dxdotdp[79] = x[78]-x[79];
  dxdotdp[80] = x[79]-x[80];
  dxdotdp[81] = k[1]*w[2]*x[89];
  dxdotdp[84] = -x[84];
  dxdotdp[85] = x[84]*2.0-x[85];
  dxdotdp[86] = x[85]-x[86];
  dxdotdp[87] = x[86]-x[87];
  dxdotdp[88] = x[87]-x[88];
  dxdotdp[89] = x[88]-x[89];
  dxdotdp[90] = k[1]*w[2]*x[98];
  dxdotdp[93] = -x[93];
  dxdotdp[94] = x[93]*2.0-x[94];
  dxdotdp[95] = x[94]-x[95];
  dxdotdp[96] = x[95]-x[96];
  dxdotdp[97] = x[96]-x[97];
  dxdotdp[98] = x[97]-x[98];
  dxdotdp[99] = k[1]*w[2]*x[107];
  dxdotdp[102] = -x[102];
  dxdotdp[103] = x[102]*2.0-x[103];
  dxdotdp[104] = x[103]-x[104];
  dxdotdp[105] = x[104]-x[105];
  dxdotdp[106] = x[105]-x[106];
  dxdotdp[107] = x[106]-x[107];
  dxdotdp[108] = k[1]*w[2]*x[116];
  dxdotdp[111] = -x[111];
  dxdotdp[112] = x[111]*2.0-x[112];
  dxdotdp[113] = x[112]-x[113];
  dxdotdp[114] = x[113]-x[114];
  dxdotdp[115] = x[114]-x[115];
  dxdotdp[116] = x[115]-x[116];
  dxdotdp[117] = k[1]*w[2]*x[125];
  dxdotdp[120] = -x[120];
  dxdotdp[121] = x[120]*2.0-x[121];
  dxdotdp[122] = x[121]-x[122];
  dxdotdp[123] = x[122]-x[123];
  dxdotdp[124] = x[123]-x[124];
  dxdotdp[125] = x[124]-x[125];
  dxdotdp[126] = k[1]*w[2]*x[134];
  dxdotdp[129] = -x[129];
  dxdotdp[130] = x[129]*2.0-x[130];
  dxdotdp[131] = x[130]-x[131];
  dxdotdp[132] = x[131]-x[132];
  dxdotdp[133] = x[132]-x[133];
  dxdotdp[134] = x[133]-x[134];
  dxdotdp[135] = k[1]*w[2]*x[143];
  dxdotdp[138] = -x[138];
  dxdotdp[139] = x[138]*2.0-x[139];
  dxdotdp[140] = x[139]-x[140];
  dxdotdp[141] = x[140]-x[141];
  dxdotdp[142] = x[141]-x[142];
  dxdotdp[143] = x[142]-x[143];
  dxdotdp[144] = k[1]*w[2]*x[152];
  dxdotdp[147] = -x[147];
  dxdotdp[148] = x[147]*2.0-x[148];
  dxdotdp[149] = x[148]-x[149];
  dxdotdp[150] = x[149]-x[150];
  dxdotdp[151] = x[150]-x[151];
  dxdotdp[152] = x[151]-x[152];
  dxdotdp[153] = k[1]*w[2]*x[161];
  dxdotdp[156] = -x[156];
  dxdotdp[157] = x[156]*2.0-x[157];
  dxdotdp[158] = x[157]-x[158];
  dxdotdp[159] = x[158]-x[159];
  dxdotdp[160] = x[159]-x[160];
  dxdotdp[161] = x[160]-x[161];

  } break;

  case 5: {
  dxdotdp[0] = -k[0]*p[0]*x[0]*w[2]*dwdp[0];
  dxdotdp[1] = p[0]*x[0]*dwdp[0];
  dxdotdp[9] = -dwdp[0]*(x[0]+p[0]*x[9]);
  dxdotdp[10] = dwdp[0]*(x[0]+p[0]*x[9]);
  dxdotdp[18] = -p[0]*x[18]*dwdp[0];
  dxdotdp[19] = p[0]*x[18]*dwdp[0];
  dxdotdp[27] = -p[0]*x[27]*dwdp[0];
  dxdotdp[28] = p[0]*x[27]*dwdp[0];
  dxdotdp[36] = -p[0]*x[36]*dwdp[0];
  dxdotdp[37] = p[0]*x[36]*dwdp[0];
  dxdotdp[45] = -p[0]*x[45]*dwdp[0];
  dxdotdp[46] = p[0]*x[45]*dwdp[0];
  dxdotdp[54] = -p[0]*x[0]*dwdp[1]-p[0]*x[54]*dwdp[0];
  dxdotdp[55] = p[0]*x[0]*dwdp[1]+p[0]*x[54]*dwdp[0];
  dxdotdp[63] = -p[0]*x[0]*dwdp[2]-p[0]*x[63]*dwdp[0];
  dxdotdp[64] = p[0]*x[0]*dwdp[2]+p[0]*x[63]*dwdp[0];
  dxdotdp[72] = -p[0]*x[0]*dwdp[3]-p[0]*x[72]*dwdp[0];
  dxdotdp[73] = p[0]*x[0]*dwdp[3]+p[0]*x[72]*dwdp[0];
  dxdotdp[81] = -p[0]*x[0]*dwdp[4]-p[0]*x[81]*dwdp[0];
  dxdotdp[82] = p[0]*x[0]*dwdp[4]+p[0]*x[81]*dwdp[0];
  dxdotdp[90] = -p[0]*x[0]*dwdp[5]-p[0]*x[90]*dwdp[0];
  dxdotdp[91] = p[0]*x[0]*dwdp[5]+p[0]*x[90]*dwdp[0];
  dxdotdp[99] = -p[0]*x[99]*dwdp[0];
  dxdotdp[100] = p[0]*x[99]*dwdp[0];
  dxdotdp[108] = -p[0]*x[108]*dwdp[0];
  dxdotdp[109] = p[0]*x[108]*dwdp[0];
  dxdotdp[117] = -p[0]*x[117]*dwdp[0];
  dxdotdp[118] = p[0]*x[117]*dwdp[0];
  dxdotdp[126] = -p[0]*x[126]*dwdp[0];
  dxdotdp[127] = p[0]*x[126]*dwdp[0];
  dxdotdp[135] = -p[0]*x[135]*dwdp[0];
  dxdotdp[136] = p[0]*x[135]*dwdp[0];
  dxdotdp[144] = -p[0]*x[144]*dwdp[0];
  dxdotdp[145] = p[0]*x[144]*dwdp[0];
  dxdotdp[153] = -p[0]*x[153]*dwdp[0];
  dxdotdp[154] = p[0]*x[153]*dwdp[0];

  } break;

  case 6: {
  dxdotdp[0] = -k[0]*p[0]*x[0]*w[2]*dwdp[6];
  dxdotdp[1] = p[0]*x[0]*dwdp[6];
  dxdotdp[9] = -dwdp[6]*(x[0]+p[0]*x[9]);
  dxdotdp[10] = dwdp[6]*(x[0]+p[0]*x[9]);
  dxdotdp[18] = -p[0]*x[18]*dwdp[6];
  dxdotdp[19] = p[0]*x[18]*dwdp[6];
  dxdotdp[27] = -p[0]*x[27]*dwdp[6];
  dxdotdp[28] = p[0]*x[27]*dwdp[6];
  dxdotdp[36] = -p[0]*x[36]*dwdp[6];
  dxdotdp[37] = p[0]*x[36]*dwdp[6];
  dxdotdp[45] = -p[0]*x[45]*dwdp[6];
  dxdotdp[46] = p[0]*x[45]*dwdp[6];
  dxdotdp[54] = -p[0]*x[0]*dwdp[7]-p[0]*x[54]*dwdp[6];
  dxdotdp[55] = p[0]*x[0]*dwdp[7]+p[0]*x[54]*dwdp[6];
  dxdotdp[63] = -p[0]*x[0]*dwdp[8]-p[0]*x[63]*dwdp[6];
  dxdotdp[64] = p[0]*x[0]*dwdp[8]+p[0]*x[63]*dwdp[6];
  dxdotdp[72] = -p[0]*x[0]*dwdp[9]-p[0]*x[72]*dwdp[6];
  dxdotdp[73] = p[0]*x[0]*dwdp[9]+p[0]*x[72]*dwdp[6];
  dxdotdp[81] = -p[0]*x[0]*dwdp[10]-p[0]*x[81]*dwdp[6];
  dxdotdp[82] = p[0]*x[0]*dwdp[10]+p[0]*x[81]*dwdp[6];
  dxdotdp[90] = -p[0]*x[0]*dwdp[11]-p[0]*x[90]*dwdp[6];
  dxdotdp[91] = p[0]*x[0]*dwdp[11]+p[0]*x[90]*dwdp[6];
  dxdotdp[99] = -p[0]*x[99]*dwdp[6];
  dxdotdp[100] = p[0]*x[99]*dwdp[6];
  dxdotdp[108] = -p[0]*x[108]*dwdp[6];
  dxdotdp[109] = p[0]*x[108]*dwdp[6];
  dxdotdp[117] = -p[0]*x[117]*dwdp[6];
  dxdotdp[118] = p[0]*x[117]*dwdp[6];
  dxdotdp[126] = -p[0]*x[126]*dwdp[6];
  dxdotdp[127] = p[0]*x[126]*dwdp[6];
  dxdotdp[135] = -p[0]*x[135]*dwdp[6];
  dxdotdp[136] = p[0]*x[135]*dwdp[6];
  dxdotdp[144] = -p[0]*x[144]*dwdp[6];
  dxdotdp[145] = p[0]*x[144]*dwdp[6];
  dxdotdp[153] = -p[0]*x[153]*dwdp[6];
  dxdotdp[154] = p[0]*x[153]*dwdp[6];

  } break;

  case 7: {
  dxdotdp[0] = -k[0]*p[0]*x[0]*w[2]*dwdp[12];
  dxdotdp[1] = p[0]*x[0]*dwdp[12];
  dxdotdp[9] = -dwdp[12]*(x[0]+p[0]*x[9]);
  dxdotdp[10] = dwdp[12]*(x[0]+p[0]*x[9]);
  dxdotdp[18] = -p[0]*x[18]*dwdp[12];
  dxdotdp[19] = p[0]*x[18]*dwdp[12];
  dxdotdp[27] = -p[0]*x[27]*dwdp[12];
  dxdotdp[28] = p[0]*x[27]*dwdp[12];
  dxdotdp[36] = -p[0]*x[36]*dwdp[12];
  dxdotdp[37] = p[0]*x[36]*dwdp[12];
  dxdotdp[45] = -p[0]*x[45]*dwdp[12];
  dxdotdp[46] = p[0]*x[45]*dwdp[12];
  dxdotdp[54] = -p[0]*x[0]*dwdp[13]-p[0]*x[54]*dwdp[12];
  dxdotdp[55] = p[0]*x[0]*dwdp[13]+p[0]*x[54]*dwdp[12];
  dxdotdp[63] = -p[0]*x[0]*dwdp[14]-p[0]*x[63]*dwdp[12];
  dxdotdp[64] = p[0]*x[0]*dwdp[14]+p[0]*x[63]*dwdp[12];
  dxdotdp[72] = -p[0]*x[0]*dwdp[15]-p[0]*x[72]*dwdp[12];
  dxdotdp[73] = p[0]*x[0]*dwdp[15]+p[0]*x[72]*dwdp[12];
  dxdotdp[81] = -p[0]*x[0]*dwdp[16]-p[0]*x[81]*dwdp[12];
  dxdotdp[82] = p[0]*x[0]*dwdp[16]+p[0]*x[81]*dwdp[12];
  dxdotdp[90] = -p[0]*x[0]*dwdp[17]-p[0]*x[90]*dwdp[12];
  dxdotdp[91] = p[0]*x[0]*dwdp[17]+p[0]*x[90]*dwdp[12];
  dxdotdp[99] = -p[0]*x[99]*dwdp[12];
  dxdotdp[100] = p[0]*x[99]*dwdp[12];
  dxdotdp[108] = -p[0]*x[108]*dwdp[12];
  dxdotdp[109] = p[0]*x[108]*dwdp[12];
  dxdotdp[117] = -p[0]*x[117]*dwdp[12];
  dxdotdp[118] = p[0]*x[117]*dwdp[12];
  dxdotdp[126] = -p[0]*x[126]*dwdp[12];
  dxdotdp[127] = p[0]*x[126]*dwdp[12];
  dxdotdp[135] = -p[0]*x[135]*dwdp[12];
  dxdotdp[136] = p[0]*x[135]*dwdp[12];
  dxdotdp[144] = -p[0]*x[144]*dwdp[12];
  dxdotdp[145] = p[0]*x[144]*dwdp[12];
  dxdotdp[153] = -p[0]*x[153]*dwdp[12];
  dxdotdp[154] = p[0]*x[153]*dwdp[12];

  } break;

  case 8: {
  dxdotdp[0] = -k[0]*p[0]*x[0]*w[2]*dwdp[18];
  dxdotdp[1] = p[0]*x[0]*dwdp[18];
  dxdotdp[9] = -dwdp[18]*(x[0]+p[0]*x[9]);
  dxdotdp[10] = dwdp[18]*(x[0]+p[0]*x[9]);
  dxdotdp[18] = -p[0]*x[18]*dwdp[18];
  dxdotdp[19] = p[0]*x[18]*dwdp[18];
  dxdotdp[27] = -p[0]*x[27]*dwdp[18];
  dxdotdp[28] = p[0]*x[27]*dwdp[18];
  dxdotdp[36] = -p[0]*x[36]*dwdp[18];
  dxdotdp[37] = p[0]*x[36]*dwdp[18];
  dxdotdp[45] = -p[0]*x[45]*dwdp[18];
  dxdotdp[46] = p[0]*x[45]*dwdp[18];
  dxdotdp[54] = -p[0]*x[0]*dwdp[19]-p[0]*x[54]*dwdp[18];
  dxdotdp[55] = p[0]*x[0]*dwdp[19]+p[0]*x[54]*dwdp[18];
  dxdotdp[63] = -p[0]*x[0]*dwdp[20]-p[0]*x[63]*dwdp[18];
  dxdotdp[64] = p[0]*x[0]*dwdp[20]+p[0]*x[63]*dwdp[18];
  dxdotdp[72] = -p[0]*x[0]*dwdp[21]-p[0]*x[72]*dwdp[18];
  dxdotdp[73] = p[0]*x[0]*dwdp[21]+p[0]*x[72]*dwdp[18];
  dxdotdp[81] = -p[0]*x[0]*dwdp[22]-p[0]*x[81]*dwdp[18];
  dxdotdp[82] = p[0]*x[0]*dwdp[22]+p[0]*x[81]*dwdp[18];
  dxdotdp[90] = -p[0]*x[0]*dwdp[23]-p[0]*x[90]*dwdp[18];
  dxdotdp[91] = p[0]*x[0]*dwdp[23]+p[0]*x[90]*dwdp[18];
  dxdotdp[99] = -p[0]*x[99]*dwdp[18];
  dxdotdp[100] = p[0]*x[99]*dwdp[18];
  dxdotdp[108] = -p[0]*x[108]*dwdp[18];
  dxdotdp[109] = p[0]*x[108]*dwdp[18];
  dxdotdp[117] = -p[0]*x[117]*dwdp[18];
  dxdotdp[118] = p[0]*x[117]*dwdp[18];
  dxdotdp[126] = -p[0]*x[126]*dwdp[18];
  dxdotdp[127] = p[0]*x[126]*dwdp[18];
  dxdotdp[135] = -p[0]*x[135]*dwdp[18];
  dxdotdp[136] = p[0]*x[135]*dwdp[18];
  dxdotdp[144] = -p[0]*x[144]*dwdp[18];
  dxdotdp[145] = p[0]*x[144]*dwdp[18];
  dxdotdp[153] = -p[0]*x[153]*dwdp[18];
  dxdotdp[154] = p[0]*x[153]*dwdp[18];

  } break;

  case 9: {
  dxdotdp[0] = -k[0]*p[0]*x[0]*w[2]*dwdp[24];
  dxdotdp[1] = p[0]*x[0]*dwdp[24];
  dxdotdp[9] = -dwdp[24]*(x[0]+p[0]*x[9]);
  dxdotdp[10] = dwdp[24]*(x[0]+p[0]*x[9]);
  dxdotdp[18] = -p[0]*x[18]*dwdp[24];
  dxdotdp[19] = p[0]*x[18]*dwdp[24];
  dxdotdp[27] = -p[0]*x[27]*dwdp[24];
  dxdotdp[28] = p[0]*x[27]*dwdp[24];
  dxdotdp[36] = -p[0]*x[36]*dwdp[24];
  dxdotdp[37] = p[0]*x[36]*dwdp[24];
  dxdotdp[45] = -p[0]*x[45]*dwdp[24];
  dxdotdp[46] = p[0]*x[45]*dwdp[24];
  dxdotdp[54] = -p[0]*x[0]*dwdp[25]-p[0]*x[54]*dwdp[24];
  dxdotdp[55] = p[0]*x[0]*dwdp[25]+p[0]*x[54]*dwdp[24];
  dxdotdp[63] = -p[0]*x[0]*dwdp[26]-p[0]*x[63]*dwdp[24];
  dxdotdp[64] = p[0]*x[0]*dwdp[26]+p[0]*x[63]*dwdp[24];
  dxdotdp[72] = -p[0]*x[0]*dwdp[27]-p[0]*x[72]*dwdp[24];
  dxdotdp[73] = p[0]*x[0]*dwdp[27]+p[0]*x[72]*dwdp[24];
  dxdotdp[81] = -p[0]*x[0]*dwdp[28]-p[0]*x[81]*dwdp[24];
  dxdotdp[82] = p[0]*x[0]*dwdp[28]+p[0]*x[81]*dwdp[24];
  dxdotdp[90] = -p[0]*x[0]*dwdp[29]-p[0]*x[90]*dwdp[24];
  dxdotdp[91] = p[0]*x[0]*dwdp[29]+p[0]*x[90]*dwdp[24];
  dxdotdp[99] = -p[0]*x[99]*dwdp[24];
  dxdotdp[100] = p[0]*x[99]*dwdp[24];
  dxdotdp[108] = -p[0]*x[108]*dwdp[24];
  dxdotdp[109] = p[0]*x[108]*dwdp[24];
  dxdotdp[117] = -p[0]*x[117]*dwdp[24];
  dxdotdp[118] = p[0]*x[117]*dwdp[24];
  dxdotdp[126] = -p[0]*x[126]*dwdp[24];
  dxdotdp[127] = p[0]*x[126]*dwdp[24];
  dxdotdp[135] = -p[0]*x[135]*dwdp[24];
  dxdotdp[136] = p[0]*x[135]*dwdp[24];
  dxdotdp[144] = -p[0]*x[144]*dwdp[24];
  dxdotdp[145] = p[0]*x[144]*dwdp[24];
  dxdotdp[153] = -p[0]*x[153]*dwdp[24];
  dxdotdp[154] = p[0]*x[153]*dwdp[24];

  } break;

}
}

} // namespace model_model_jakstat_adjoint_o2

} // namespace amici

