#include "zebu.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const unsigned zebu_shifts[103][16] = {
	[1][2] = 2,
	[1][3] = 3,
	[1][4] = 4,
	[2][5] = 9,
	[4][2] = 10,
	[4][3] = 11,
	[4][4] = 12,
	[4][8] = 13,
	[4][9] = 14,
	[6][6] = 20,
	[6][7] = 21,
	[7][2] = 2,
	[7][3] = 3,
	[7][4] = 4,
	[9][2] = 24,
	[9][3] = 25,
	[9][4] = 26,
	[9][8] = 27,
	[9][9] = 28,
	[12][2] = 10,
	[12][3] = 11,
	[12][4] = 12,
	[12][8] = 13,
	[12][9] = 14,
	[13][2] = 10,
	[13][3] = 11,
	[13][4] = 12,
	[13][8] = 13,
	[13][9] = 14,
	[14][2] = 10,
	[14][3] = 11,
	[14][4] = 12,
	[14][8] = 13,
	[14][9] = 14,
	[15][8] = 38,
	[15][9] = 39,
	[16][11] = 40,
	[17][6] = 42,
	[17][12] = 41,
	[20][2] = 43,
	[20][3] = 3,
	[20][4] = 4,
	[21][2] = 24,
	[21][3] = 25,
	[21][4] = 26,
	[21][8] = 27,
	[21][9] = 28,
	[23][2] = 2,
	[23][3] = 3,
	[23][4] = 4,
	[26][2] = 10,
	[26][3] = 11,
	[26][4] = 12,
	[26][8] = 13,
	[26][9] = 14,
	[27][2] = 24,
	[27][3] = 25,
	[27][4] = 26,
	[27][8] = 27,
	[27][9] = 28,
	[28][2] = 24,
	[28][3] = 25,
	[28][4] = 26,
	[28][8] = 27,
	[28][9] = 28,
	[29][8] = 52,
	[29][9] = 53,
	[30][13] = 54,
	[31][6] = 56,
	[31][12] = 55,
	[34][11] = 57,
	[38][2] = 10,
	[38][3] = 11,
	[38][4] = 12,
	[38][8] = 13,
	[38][9] = 14,
	[39][2] = 10,
	[39][3] = 11,
	[39][4] = 12,
	[39][8] = 13,
	[39][9] = 14,
	[41][2] = 10,
	[41][3] = 11,
	[41][4] = 12,
	[41][8] = 13,
	[41][9] = 14,
	[42][2] = 10,
	[42][3] = 11,
	[42][4] = 12,
	[42][8] = 13,
	[42][9] = 14,
	[44][6] = 64,
	[44][7] = 65,
	[46][13] = 66,
	[48][11] = 67,
	[52][2] = 24,
	[52][3] = 25,
	[52][4] = 26,
	[52][8] = 27,
	[52][9] = 28,
	[53][2] = 24,
	[53][3] = 25,
	[53][4] = 26,
	[53][8] = 27,
	[53][9] = 28,
	[55][2] = 24,
	[55][3] = 25,
	[55][4] = 26,
	[55][8] = 27,
	[55][9] = 28,
	[56][2] = 24,
	[56][3] = 25,
	[56][4] = 26,
	[56][8] = 27,
	[56][9] = 28,
	[59][6] = 42,
	[59][12] = 41,
	[64][2] = 74,
	[64][3] = 75,
	[64][4] = 76,
	[65][2] = 24,
	[65][3] = 25,
	[65][4] = 26,
	[65][8] = 27,
	[65][9] = 28,
	[69][6] = 56,
	[69][12] = 55,
	[76][2] = 10,
	[76][3] = 11,
	[76][4] = 12,
	[76][8] = 13,
	[76][9] = 14,
	[77][7] = 81,
	[77][14] = 80,
	[79][11] = 82,
	[80][2] = 83,
	[80][3] = 84,
	[80][4] = 85,
	[81][2] = 24,
	[81][3] = 25,
	[81][4] = 26,
	[81][8] = 27,
	[81][9] = 28,
	[85][2] = 10,
	[85][3] = 11,
	[85][4] = 12,
	[85][8] = 13,
	[85][9] = 14,
	[86][7] = 90,
	[86][15] = 89,
	[88][11] = 91,
	[89][2] = 83,
	[89][3] = 84,
	[89][4] = 85,
	[90][2] = 24,
	[90][3] = 25,
	[90][4] = 26,
	[90][8] = 27,
	[90][9] = 28,
	[92][7] = 95,
	[92][15] = 94,
	[94][2] = 96,
	[94][3] = 97,
	[94][4] = 98,
	[95][2] = 24,
	[95][3] = 25,
	[95][4] = 26,
	[95][8] = 27,
	[95][9] = 28,
	[98][2] = 10,
	[98][3] = 11,
	[98][4] = 12,
	[98][8] = 13,
	[98][9] = 14,
	[99][7] = 102,
	[101][11] = 103,
	[102][2] = 24,
	[102][3] = 25,
	[102][4] = 26,
	[102][8] = 27,
	[102][9] = 28,
};


const unsigned zebu_reduces[105][16] = {
	[2][6] = 1,
	[2][7] = 1,
	[3][6] = 2,
	[3][7] = 2,
	[5][2] = 3,
	[5][3] = 3,
	[5][4] = 3,
	[5][10] = 3,
	[7][10] = 4,
	[8][2] = 5,
	[8][3] = 5,
	[8][4] = 5,
	[8][10] = 5,
	[10][6] = 1,
	[10][8] = 1,
	[10][9] = 1,
	[10][11] = 1,
	[10][12] = 1,
	[11][6] = 2,
	[11][8] = 2,
	[11][9] = 2,
	[11][11] = 2,
	[11][12] = 2,
	[15][11] = 6,
	[17][8] = 7,
	[17][9] = 7,
	[17][11] = 7,
	[18][6] = 8,
	[18][8] = 8,
	[18][9] = 8,
	[18][11] = 8,
	[18][12] = 8,
	[19][6] = 9,
	[19][8] = 9,
	[19][9] = 9,
	[19][11] = 9,
	[19][12] = 9,
	[22][10] = 10,
	[23][10] = 11,
	[24][6] = 1,
	[24][8] = 1,
	[24][9] = 1,
	[24][12] = 1,
	[24][13] = 1,
	[25][6] = 2,
	[25][8] = 2,
	[25][9] = 2,
	[25][12] = 2,
	[25][13] = 2,
	[29][13] = 6,
	[31][8] = 7,
	[31][9] = 7,
	[31][13] = 7,
	[32][6] = 8,
	[32][8] = 8,
	[32][9] = 8,
	[32][12] = 8,
	[32][13] = 8,
	[33][6] = 9,
	[33][8] = 9,
	[33][9] = 9,
	[33][12] = 9,
	[33][13] = 9,
	[35][6] = 12,
	[35][8] = 12,
	[35][9] = 12,
	[35][11] = 12,
	[35][12] = 12,
	[36][6] = 13,
	[36][8] = 13,
	[36][9] = 13,
	[36][11] = 13,
	[36][12] = 13,
	[37][6] = 14,
	[37][8] = 14,
	[37][9] = 14,
	[37][11] = 14,
	[37][12] = 14,
	[40][6] = 15,
	[40][7] = 15,
	[43][6] = 1,
	[43][7] = 1,
	[45][2] = 16,
	[45][3] = 16,
	[45][4] = 16,
	[45][10] = 16,
	[47][10] = 17,
	[49][6] = 12,
	[49][8] = 12,
	[49][9] = 12,
	[49][12] = 12,
	[49][13] = 12,
	[50][6] = 13,
	[50][8] = 13,
	[50][9] = 13,
	[50][12] = 13,
	[50][13] = 13,
	[51][6] = 14,
	[51][8] = 14,
	[51][9] = 14,
	[51][12] = 14,
	[51][13] = 14,
	[54][2] = 18,
	[54][3] = 18,
	[54][4] = 18,
	[54][10] = 18,
	[57][6] = 15,
	[57][8] = 15,
	[57][9] = 15,
	[57][11] = 15,
	[57][12] = 15,
	[58][8] = 19,
	[58][9] = 19,
	[58][11] = 19,
	[59][8] = 20,
	[59][9] = 20,
	[59][11] = 20,
	[60][8] = 21,
	[60][9] = 21,
	[60][11] = 21,
	[61][6] = 22,
	[61][8] = 22,
	[61][9] = 22,
	[61][11] = 22,
	[61][12] = 22,
	[62][6] = 23,
	[62][8] = 23,
	[62][9] = 23,
	[62][11] = 23,
	[62][12] = 23,
	[63][6] = 24,
	[63][8] = 24,
	[63][9] = 24,
	[63][11] = 24,
	[63][12] = 24,
	[66][2] = 25,
	[66][3] = 25,
	[66][4] = 25,
	[66][10] = 25,
	[67][6] = 15,
	[67][8] = 15,
	[67][9] = 15,
	[67][12] = 15,
	[67][13] = 15,
	[68][8] = 19,
	[68][9] = 19,
	[68][13] = 19,
	[69][8] = 20,
	[69][9] = 20,
	[69][13] = 20,
	[70][8] = 21,
	[70][9] = 21,
	[70][13] = 21,
	[71][6] = 22,
	[71][8] = 22,
	[71][9] = 22,
	[71][12] = 22,
	[71][13] = 22,
	[72][6] = 23,
	[72][8] = 23,
	[72][9] = 23,
	[72][12] = 23,
	[72][13] = 23,
	[73][6] = 24,
	[73][8] = 24,
	[73][9] = 24,
	[73][12] = 24,
	[73][13] = 24,
	[74][7] = 1,
	[74][14] = 1,
	[75][7] = 2,
	[75][14] = 2,
	[78][2] = 26,
	[78][3] = 26,
	[78][4] = 26,
	[78][10] = 26,
	[82][7] = 15,
	[82][14] = 15,
	[83][7] = 1,
	[83][15] = 1,
	[84][7] = 2,
	[84][15] = 2,
	[87][2] = 27,
	[87][3] = 27,
	[87][4] = 27,
	[87][10] = 27,
	[91][7] = 15,
	[91][15] = 15,
	[93][2] = 28,
	[93][3] = 28,
	[93][4] = 28,
	[93][10] = 28,
	[96][7] = 1,
	[97][7] = 2,
	[100][2] = 29,
	[100][3] = 29,
	[100][4] = 29,
	[100][10] = 29,
	[103][7] = 15,
	[104][2] = 30,
	[104][3] = 30,
	[104][4] = 30,
	[104][10] = 30,
};


const unsigned zebu_gotos[103][14] = {
	[1][1] = 5,
	[1][2] = 6,
	[1][3] = 7,
	[1][4] = 8,
	[4][2] = 18,
	[4][5] = 15,
	[4][6] = 16,
	[4][7] = 17,
	[4][8] = 19,
	[7][1] = 5,
	[7][2] = 6,
	[7][3] = 23,
	[7][4] = 8,
	[7][9] = 22,
	[9][2] = 32,
	[9][5] = 29,
	[9][6] = 30,
	[9][7] = 31,
	[9][8] = 33,
	[12][2] = 18,
	[12][5] = 15,
	[12][6] = 34,
	[12][7] = 17,
	[12][8] = 19,
	[13][2] = 18,
	[13][8] = 36,
	[13][10] = 35,
	[14][2] = 18,
	[14][8] = 36,
	[14][10] = 37,
	[20][2] = 44,
	[21][2] = 32,
	[21][5] = 29,
	[21][6] = 46,
	[21][7] = 31,
	[21][8] = 33,
	[21][11] = 45,
	[23][1] = 5,
	[23][2] = 6,
	[23][3] = 23,
	[23][4] = 8,
	[23][9] = 47,
	[26][2] = 18,
	[26][5] = 15,
	[26][6] = 48,
	[26][7] = 17,
	[26][8] = 19,
	[27][2] = 32,
	[27][8] = 50,
	[27][10] = 49,
	[28][2] = 32,
	[28][8] = 50,
	[28][10] = 51,
	[38][2] = 18,
	[38][7] = 59,
	[38][8] = 19,
	[38][12] = 58,
	[39][2] = 18,
	[39][7] = 59,
	[39][8] = 19,
	[39][12] = 60,
	[41][2] = 18,
	[41][8] = 62,
	[41][13] = 61,
	[42][2] = 18,
	[42][8] = 62,
	[42][13] = 63,
	[52][2] = 32,
	[52][7] = 69,
	[52][8] = 33,
	[52][12] = 68,
	[53][2] = 32,
	[53][7] = 69,
	[53][8] = 33,
	[53][12] = 70,
	[55][2] = 32,
	[55][8] = 72,
	[55][13] = 71,
	[56][2] = 32,
	[56][8] = 72,
	[56][13] = 73,
	[64][2] = 77,
	[65][2] = 32,
	[65][5] = 29,
	[65][6] = 46,
	[65][7] = 31,
	[65][8] = 33,
	[65][11] = 78,
	[76][2] = 18,
	[76][5] = 15,
	[76][6] = 79,
	[76][7] = 17,
	[76][8] = 19,
	[80][2] = 86,
	[81][2] = 32,
	[81][5] = 29,
	[81][6] = 46,
	[81][7] = 31,
	[81][8] = 33,
	[81][11] = 87,
	[85][2] = 18,
	[85][5] = 15,
	[85][6] = 88,
	[85][7] = 17,
	[85][8] = 19,
	[89][2] = 92,
	[90][2] = 32,
	[90][5] = 29,
	[90][6] = 46,
	[90][7] = 31,
	[90][8] = 33,
	[90][11] = 93,
	[94][2] = 99,
	[95][2] = 32,
	[95][5] = 29,
	[95][6] = 46,
	[95][7] = 31,
	[95][8] = 33,
	[95][11] = 100,
	[98][2] = 18,
	[98][5] = 15,
	[98][6] = 101,
	[98][7] = 17,
	[98][8] = 19,
	[102][2] = 32,
	[102][5] = 29,
	[102][6] = 46,
	[102][7] = 31,
	[102][8] = 33,
	[102][11] = 104,
};


const unsigned zebu_lexer[33][256] = {
	[1][9] = 16,
	[1][10] = 16,
	[1][32] = 16,
	[1][35] = 17,
	[1][40] = 18,
	[1][48] = 19,
	[1][49] = 19,
	[1][50] = 19,
	[1][51] = 19,
	[1][52] = 19,
	[1][53] = 19,
	[1][54] = 19,
	[1][55] = 19,
	[1][56] = 19,
	[1][57] = 19,
	[1][65] = 20,
	[1][66] = 20,
	[1][67] = 20,
	[1][68] = 20,
	[1][69] = 20,
	[1][70] = 20,
	[1][71] = 20,
	[1][72] = 20,
	[1][73] = 20,
	[1][74] = 20,
	[1][75] = 20,
	[1][76] = 20,
	[1][77] = 20,
	[1][78] = 20,
	[1][79] = 20,
	[1][80] = 20,
	[1][81] = 20,
	[1][82] = 20,
	[1][83] = 20,
	[1][84] = 20,
	[1][85] = 20,
	[1][86] = 20,
	[1][87] = 20,
	[1][88] = 20,
	[1][89] = 20,
	[1][90] = 20,
	[1][95] = 20,
	[1][97] = 20,
	[1][98] = 20,
	[1][99] = 20,
	[1][100] = 20,
	[1][101] = 20,
	[1][102] = 20,
	[1][103] = 20,
	[1][104] = 20,
	[1][105] = 20,
	[1][106] = 20,
	[1][107] = 20,
	[1][108] = 20,
	[1][109] = 20,
	[1][110] = 20,
	[1][111] = 20,
	[1][112] = 20,
	[1][113] = 20,
	[1][114] = 20,
	[1][115] = 20,
	[1][116] = 20,
	[1][117] = 20,
	[1][118] = 20,
	[1][119] = 20,
	[1][120] = 20,
	[1][121] = 20,
	[1][122] = 20,
	[2][9] = 16,
	[2][10] = 16,
	[2][32] = 16,
	[2][35] = 17,
	[2][47] = 21,
	[2][58] = 22,
	[2][61] = 23,
	[3][9] = 16,
	[3][10] = 16,
	[3][32] = 16,
	[3][35] = 17,
	[3][47] = 21,
	[3][58] = 22,
	[4][9] = 16,
	[4][10] = 16,
	[4][32] = 16,
	[4][35] = 17,
	[4][40] = 18,
	[4][43] = 24,
	[4][45] = 25,
	[4][48] = 19,
	[4][49] = 19,
	[4][50] = 19,
	[4][51] = 19,
	[4][52] = 19,
	[4][53] = 19,
	[4][54] = 19,
	[4][55] = 19,
	[4][56] = 19,
	[4][57] = 19,
	[4][65] = 20,
	[4][66] = 20,
	[4][67] = 20,
	[4][68] = 20,
	[4][69] = 20,
	[4][70] = 20,
	[4][71] = 20,
	[4][72] = 20,
	[4][73] = 20,
	[4][74] = 20,
	[4][75] = 20,
	[4][76] = 20,
	[4][77] = 20,
	[4][78] = 20,
	[4][79] = 20,
	[4][80] = 20,
	[4][81] = 20,
	[4][82] = 20,
	[4][83] = 20,
	[4][84] = 20,
	[4][85] = 20,
	[4][86] = 20,
	[4][87] = 20,
	[4][88] = 20,
	[4][89] = 20,
	[4][90] = 20,
	[4][95] = 20,
	[4][97] = 20,
	[4][98] = 20,
	[4][99] = 20,
	[4][100] = 20,
	[4][101] = 20,
	[4][102] = 20,
	[4][103] = 20,
	[4][104] = 20,
	[4][105] = 20,
	[4][106] = 20,
	[4][107] = 20,
	[4][108] = 20,
	[4][109] = 20,
	[4][110] = 20,
	[4][111] = 20,
	[4][112] = 20,
	[4][113] = 20,
	[4][114] = 20,
	[4][115] = 20,
	[4][116] = 20,
	[4][117] = 20,
	[4][118] = 20,
	[4][119] = 20,
	[4][120] = 20,
	[4][121] = 20,
	[4][122] = 20,
	[5][9] = 16,
	[5][10] = 16,
	[5][32] = 16,
	[5][35] = 17,
	[5][40] = 18,
	[5][48] = 19,
	[5][49] = 19,
	[5][50] = 19,
	[5][51] = 19,
	[5][52] = 19,
	[5][53] = 19,
	[5][54] = 19,
	[5][55] = 19,
	[5][56] = 19,
	[5][57] = 19,
	[5][65] = 20,
	[5][66] = 20,
	[5][67] = 20,
	[5][68] = 20,
	[5][69] = 20,
	[5][70] = 20,
	[5][71] = 20,
	[5][72] = 20,
	[5][73] = 20,
	[5][74] = 20,
	[5][75] = 20,
	[5][76] = 20,
	[5][77] = 20,
	[5][78] = 20,
	[5][79] = 20,
	[5][80] = 20,
	[5][81] = 20,
	[5][82] = 20,
	[5][83] = 20,
	[5][84] = 20,
	[5][85] = 20,
	[5][86] = 20,
	[5][87] = 20,
	[5][88] = 20,
	[5][89] = 20,
	[5][90] = 20,
	[5][95] = 20,
	[5][97] = 20,
	[5][98] = 20,
	[5][99] = 20,
	[5][100] = 20,
	[5][101] = 20,
	[5][102] = 20,
	[5][103] = 20,
	[5][104] = 20,
	[5][105] = 20,
	[5][106] = 20,
	[5][107] = 20,
	[5][108] = 20,
	[5][109] = 20,
	[5][110] = 20,
	[5][111] = 20,
	[5][112] = 20,
	[5][113] = 20,
	[5][114] = 20,
	[5][115] = 20,
	[5][116] = 20,
	[5][117] = 20,
	[5][118] = 20,
	[5][119] = 20,
	[5][120] = 20,
	[5][121] = 20,
	[5][122] = 20,
	[6][9] = 16,
	[6][10] = 16,
	[6][32] = 16,
	[6][35] = 17,
	[6][41] = 27,
	[6][42] = 28,
	[6][43] = 24,
	[6][45] = 25,
	[6][47] = 21,
	[7][9] = 16,
	[7][10] = 16,
	[7][32] = 16,
	[7][35] = 17,
	[7][41] = 27,
	[7][43] = 24,
	[7][45] = 25,
	[8][9] = 16,
	[8][10] = 16,
	[8][32] = 16,
	[8][35] = 17,
	[8][41] = 27,
	[9][9] = 16,
	[9][10] = 16,
	[9][32] = 16,
	[9][35] = 17,
	[10][9] = 16,
	[10][10] = 16,
	[10][32] = 16,
	[10][35] = 17,
	[10][42] = 28,
	[10][43] = 24,
	[10][45] = 25,
	[10][47] = 21,
	[10][59] = 29,
	[11][9] = 16,
	[11][10] = 16,
	[11][32] = 16,
	[11][35] = 17,
	[11][43] = 24,
	[11][45] = 25,
	[11][59] = 29,
	[12][9] = 16,
	[12][10] = 16,
	[12][32] = 16,
	[12][35] = 17,
	[12][59] = 29,
	[13][9] = 16,
	[13][10] = 16,
	[13][32] = 16,
	[13][35] = 17,
	[13][58] = 22,
	[13][64] = 30,
	[14][9] = 16,
	[14][10] = 16,
	[14][32] = 16,
	[14][35] = 17,
	[14][58] = 31,
	[15][9] = 16,
	[15][10] = 16,
	[15][32] = 16,
	[15][35] = 17,
	[15][58] = 22,
	[16][9] = 16,
	[16][10] = 16,
	[16][32] = 16,
	[16][35] = 17,
	[17][0] = 17,
	[17][1] = 17,
	[17][2] = 17,
	[17][3] = 17,
	[17][4] = 17,
	[17][5] = 17,
	[17][6] = 17,
	[17][7] = 17,
	[17][8] = 17,
	[17][9] = 17,
	[17][10] = 16,
	[17][11] = 17,
	[17][12] = 17,
	[17][13] = 17,
	[17][14] = 17,
	[17][15] = 17,
	[17][16] = 17,
	[17][17] = 17,
	[17][18] = 17,
	[17][19] = 17,
	[17][20] = 17,
	[17][21] = 17,
	[17][22] = 17,
	[17][23] = 17,
	[17][24] = 17,
	[17][25] = 17,
	[17][26] = 17,
	[17][27] = 17,
	[17][28] = 17,
	[17][29] = 17,
	[17][30] = 17,
	[17][31] = 17,
	[17][32] = 17,
	[17][33] = 17,
	[17][34] = 17,
	[17][35] = 17,
	[17][36] = 17,
	[17][37] = 17,
	[17][38] = 17,
	[17][39] = 17,
	[17][40] = 17,
	[17][41] = 17,
	[17][42] = 17,
	[17][43] = 17,
	[17][44] = 17,
	[17][45] = 17,
	[17][46] = 17,
	[17][47] = 17,
	[17][48] = 17,
	[17][49] = 17,
	[17][50] = 17,
	[17][51] = 17,
	[17][52] = 17,
	[17][53] = 17,
	[17][54] = 17,
	[17][55] = 17,
	[17][56] = 17,
	[17][57] = 17,
	[17][58] = 17,
	[17][59] = 17,
	[17][60] = 17,
	[17][61] = 17,
	[17][62] = 17,
	[17][63] = 17,
	[17][64] = 17,
	[17][65] = 17,
	[17][66] = 17,
	[17][67] = 17,
	[17][68] = 17,
	[17][69] = 17,
	[17][70] = 17,
	[17][71] = 17,
	[17][72] = 17,
	[17][73] = 17,
	[17][74] = 17,
	[17][75] = 17,
	[17][76] = 17,
	[17][77] = 17,
	[17][78] = 17,
	[17][79] = 17,
	[17][80] = 17,
	[17][81] = 17,
	[17][82] = 17,
	[17][83] = 17,
	[17][84] = 17,
	[17][85] = 17,
	[17][86] = 17,
	[17][87] = 17,
	[17][88] = 17,
	[17][89] = 17,
	[17][90] = 17,
	[17][91] = 17,
	[17][92] = 17,
	[17][93] = 17,
	[17][94] = 17,
	[17][95] = 17,
	[17][96] = 17,
	[17][97] = 17,
	[17][98] = 17,
	[17][99] = 17,
	[17][100] = 17,
	[17][101] = 17,
	[17][102] = 17,
	[17][103] = 17,
	[17][104] = 17,
	[17][105] = 17,
	[17][106] = 17,
	[17][107] = 17,
	[17][108] = 17,
	[17][109] = 17,
	[17][110] = 17,
	[17][111] = 17,
	[17][112] = 17,
	[17][113] = 17,
	[17][114] = 17,
	[17][115] = 17,
	[17][116] = 17,
	[17][117] = 17,
	[17][118] = 17,
	[17][119] = 17,
	[17][120] = 17,
	[17][121] = 17,
	[17][122] = 17,
	[17][123] = 17,
	[17][124] = 17,
	[17][125] = 17,
	[17][126] = 17,
	[17][127] = 17,
	[17][128] = 17,
	[17][129] = 17,
	[17][130] = 17,
	[17][131] = 17,
	[17][132] = 17,
	[17][133] = 17,
	[17][134] = 17,
	[17][135] = 17,
	[17][136] = 17,
	[17][137] = 17,
	[17][138] = 17,
	[17][139] = 17,
	[17][140] = 17,
	[17][141] = 17,
	[17][142] = 17,
	[17][143] = 17,
	[17][144] = 17,
	[17][145] = 17,
	[17][146] = 17,
	[17][147] = 17,
	[17][148] = 17,
	[17][149] = 17,
	[17][150] = 17,
	[17][151] = 17,
	[17][152] = 17,
	[17][153] = 17,
	[17][154] = 17,
	[17][155] = 17,
	[17][156] = 17,
	[17][157] = 17,
	[17][158] = 17,
	[17][159] = 17,
	[17][160] = 17,
	[17][161] = 17,
	[17][162] = 17,
	[17][163] = 17,
	[17][164] = 17,
	[17][165] = 17,
	[17][166] = 17,
	[17][167] = 17,
	[17][168] = 17,
	[17][169] = 17,
	[17][170] = 17,
	[17][171] = 17,
	[17][172] = 17,
	[17][173] = 17,
	[17][174] = 17,
	[17][175] = 17,
	[17][176] = 17,
	[17][177] = 17,
	[17][178] = 17,
	[17][179] = 17,
	[17][180] = 17,
	[17][181] = 17,
	[17][182] = 17,
	[17][183] = 17,
	[17][184] = 17,
	[17][185] = 17,
	[17][186] = 17,
	[17][187] = 17,
	[17][188] = 17,
	[17][189] = 17,
	[17][190] = 17,
	[17][191] = 17,
	[17][192] = 17,
	[17][193] = 17,
	[17][194] = 17,
	[17][195] = 17,
	[17][196] = 17,
	[17][197] = 17,
	[17][198] = 17,
	[17][199] = 17,
	[17][200] = 17,
	[17][201] = 17,
	[17][202] = 17,
	[17][203] = 17,
	[17][204] = 17,
	[17][205] = 17,
	[17][206] = 17,
	[17][207] = 17,
	[17][208] = 17,
	[17][209] = 17,
	[17][210] = 17,
	[17][211] = 17,
	[17][212] = 17,
	[17][213] = 17,
	[17][214] = 17,
	[17][215] = 17,
	[17][216] = 17,
	[17][217] = 17,
	[17][218] = 17,
	[17][219] = 17,
	[17][220] = 17,
	[17][221] = 17,
	[17][222] = 17,
	[17][223] = 17,
	[17][224] = 17,
	[17][225] = 17,
	[17][226] = 17,
	[17][227] = 17,
	[17][228] = 17,
	[17][229] = 17,
	[17][230] = 17,
	[17][231] = 17,
	[17][232] = 17,
	[17][233] = 17,
	[17][234] = 17,
	[17][235] = 17,
	[17][236] = 17,
	[17][237] = 17,
	[17][238] = 17,
	[17][239] = 17,
	[17][240] = 17,
	[17][241] = 17,
	[17][242] = 17,
	[17][243] = 17,
	[17][244] = 17,
	[17][245] = 17,
	[17][246] = 17,
	[17][247] = 17,
	[17][248] = 17,
	[17][249] = 17,
	[17][250] = 17,
	[17][251] = 17,
	[17][252] = 17,
	[17][253] = 17,
	[17][254] = 17,
	[17][255] = 17,
	[19][46] = 32,
	[19][48] = 19,
	[19][49] = 19,
	[19][50] = 19,
	[19][51] = 19,
	[19][52] = 19,
	[19][53] = 19,
	[19][54] = 19,
	[19][55] = 19,
	[19][56] = 19,
	[19][57] = 19,
	[20][48] = 20,
	[20][49] = 20,
	[20][50] = 20,
	[20][51] = 20,
	[20][52] = 20,
	[20][53] = 20,
	[20][54] = 20,
	[20][55] = 20,
	[20][56] = 20,
	[20][57] = 20,
	[20][65] = 20,
	[20][66] = 20,
	[20][67] = 20,
	[20][68] = 20,
	[20][69] = 20,
	[20][70] = 20,
	[20][71] = 20,
	[20][72] = 20,
	[20][73] = 20,
	[20][74] = 20,
	[20][75] = 20,
	[20][76] = 20,
	[20][77] = 20,
	[20][78] = 20,
	[20][79] = 20,
	[20][80] = 20,
	[20][81] = 20,
	[20][82] = 20,
	[20][83] = 20,
	[20][84] = 20,
	[20][85] = 20,
	[20][86] = 20,
	[20][87] = 20,
	[20][88] = 20,
	[20][89] = 20,
	[20][90] = 20,
	[20][95] = 20,
	[20][97] = 20,
	[20][98] = 20,
	[20][99] = 20,
	[20][100] = 20,
	[20][101] = 20,
	[20][102] = 20,
	[20][103] = 20,
	[20][104] = 20,
	[20][105] = 20,
	[20][106] = 20,
	[20][107] = 20,
	[20][108] = 20,
	[20][109] = 20,
	[20][110] = 20,
	[20][111] = 20,
	[20][112] = 20,
	[20][113] = 20,
	[20][114] = 20,
	[20][115] = 20,
	[20][116] = 20,
	[20][117] = 20,
	[20][118] = 20,
	[20][119] = 20,
	[20][120] = 20,
	[20][121] = 20,
	[20][122] = 20,
	[22][32] = 33,
	[31][32] = 33,
	[32][48] = 32,
	[32][49] = 32,
	[32][50] = 32,
	[32][51] = 32,
	[32][52] = 32,
	[32][53] = 32,
	[32][54] = 32,
	[32][55] = 32,
	[32][56] = 32,
	[32][57] = 32,
};


const unsigned zebu_lexer_starts[105] = {
	[1] = 1,
	[2] = 2,
	[3] = 3,
	[4] = 4,
	[5] = 5,
	[6] = 3,
	[7] = 5,
	[8] = 5,
	[9] = 4,
	[10] = 6,
	[11] = 6,
	[12] = 4,
	[13] = 4,
	[14] = 4,
	[15] = 7,
	[16] = 8,
	[17] = 6,
	[18] = 6,
	[19] = 6,
	[20] = 1,
	[21] = 4,
	[22] = 9,
	[23] = 5,
	[24] = 10,
	[25] = 10,
	[26] = 4,
	[27] = 4,
	[28] = 4,
	[29] = 11,
	[30] = 12,
	[31] = 10,
	[32] = 10,
	[33] = 10,
	[34] = 8,
	[35] = 6,
	[36] = 6,
	[37] = 6,
	[38] = 4,
	[39] = 4,
	[40] = 3,
	[41] = 4,
	[42] = 4,
	[43] = 3,
	[44] = 3,
	[45] = 5,
	[46] = 12,
	[47] = 9,
	[48] = 8,
	[49] = 10,
	[50] = 10,
	[51] = 10,
	[52] = 4,
	[53] = 4,
	[54] = 5,
	[55] = 4,
	[56] = 4,
	[57] = 6,
	[58] = 7,
	[59] = 6,
	[60] = 7,
	[61] = 6,
	[62] = 6,
	[63] = 6,
	[64] = 1,
	[65] = 4,
	[66] = 5,
	[67] = 10,
	[68] = 11,
	[69] = 10,
	[70] = 11,
	[71] = 10,
	[72] = 10,
	[73] = 10,
	[74] = 13,
	[75] = 13,
	[76] = 4,
	[77] = 13,
	[78] = 5,
	[79] = 8,
	[80] = 1,
	[81] = 4,
	[82] = 13,
	[83] = 14,
	[84] = 14,
	[85] = 4,
	[86] = 14,
	[87] = 5,
	[88] = 8,
	[89] = 1,
	[90] = 4,
	[91] = 14,
	[92] = 14,
	[93] = 5,
	[94] = 1,
	[95] = 4,
	[96] = 15,
	[97] = 15,
	[98] = 4,
	[99] = 15,
	[100] = 5,
	[101] = 8,
	[102] = 4,
	[103] = 15,
	[104] = 5,
};


const unsigned zebu_lexer_accepts[34] = {
	[16] = 1,
	[18] = 4,
	[19] = 3,
	[20] = 2,
	[21] = 6,
	[23] = 5,
	[24] = 8,
	[25] = 9,
	[26] = 10,
	[27] = 11,
	[28] = 12,
	[29] = 13,
	[30] = 14,
	[31] = 15,
	[32] = 3,
	[33] = 7,
};


const unsigned zebu_lexer_EOFs[10] = {
	[5] = 26,
	[9] = 26,
};


#ifdef ZEBU_DEBUG
#include <stdbool.h>

struct link
{
	bool is_last;
	struct link* prev;
};

static void print_links(struct link* link)
{
	if (!link) return;
	
	print_links(link->prev);
	
	if (link->is_last)
		fputs("    ", stdout);
	else
		fputs("│   ", stdout);
}

static void print_tree_escape(char *out, char in)
{
	switch (in)
	{
		case ' ':
		case '~':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '-':
		case '+':
		case '=':
		case '|':
		case '<': case '>':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case ':': case ';':
		case ',': case '.':
		case '_':
		case '/':
		case '`':
		case '0' ... '9':
		case 'a' ... 'z':
		case 'A' ... 'Z':
			*out++ = in;
			*out = 0;
			break;
		
		case '\\': *out++ = '\\', *out++ = '\\', *out = 0; break;
		
		case '\'': *out++ = '\\', *out++ = '\'', *out = 0; break;
		
		case '\"': *out++ = '\\', *out++ = '\"', *out = 0; break;
		
		case '\t': *out++ = '\\', *out++ = 't', *out = 0; break;
		
		case '\n': *out++ = '\\', *out++ = 'n', *out = 0; break;
		
		default:
			sprintf(out, "\\x%02X", in);
			break;
	}
}

enum prefix
{
	p_root,
	p_not_last_child,
	p_last_child,
};

void print_token_leaf(struct link* links, enum prefix p, const char* name, struct zebu_token* token)
{
	print_links(links);
	switch (p)
	{
		case p_root: break;
		case p_not_last_child: fputs("├── ", stdout); break;
		case p_last_child: fputs("└── ", stdout); break;
	}
	printf("\e[32m%s\e[0m (\e[35m\"", name);
	char escape[10];
	for (unsigned i = 0, n = token->len; i < n; i++)
		print_tree_escape(escape, token->data[i]), fputs(escape, stdout);
	printf("\"\e[0m) on line %u\n", token->line);
}

void print_scanf_leaf(struct link* links, enum prefix p, const char* name, const char* format, ...)
{
	va_list ap;
	print_links(links);
	switch (p)
	{
		case p_root: break;
		case p_not_last_child: fputs("├── ", stdout); break;
		case p_last_child: fputs("└── ", stdout); break;
	}
	printf("\e[32m%s\e[0m (\e[35m%s\e[m: \e[35m", name, format);
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("\e[0m)\n");
}

void print_empty_leaf(struct link* links, enum prefix p, const char* type, const char* name)
{
	print_links(links);
	switch (p)
	{
		case p_root: break;
		case p_not_last_child: fputs("├── ", stdout); break;
		case p_last_child: fputs("└── ", stdout); break;
	}
	printf("\e[31m%s\e[0m (\e[36m%s\e[0m)\n", name, type);
}
void print_zebu_$start(struct link* links, enum prefix p, const char* name, struct zebu_$start* ptree);

void print_zebu_additive_expression(struct link* links, enum prefix p, const char* name, struct zebu_additive_expression* ptree);

void print_zebu_assignment(struct link* links, enum prefix p, const char* name, struct zebu_assignment* ptree);

void print_zebu_expression(struct link* links, enum prefix p, const char* name, struct zebu_expression* ptree);

void print_zebu_multiplicative_expression(struct link* links, enum prefix p, const char* name, struct zebu_multiplicative_expression* ptree);

void print_zebu_postfix_expression(struct link* links, enum prefix p, const char* name, struct zebu_postfix_expression* ptree);

void print_zebu_prefix_expression(struct link* links, enum prefix p, const char* name, struct zebu_prefix_expression* ptree);

void print_zebu_statement(struct link* links, enum prefix p, const char* name, struct zebu_statement* ptree);

void print_zebu_transaction(struct link* links, enum prefix p, const char* name, struct zebu_transaction* ptree);

void print_zebu_$start(struct link* links, enum prefix p, const char* name, struct zebu_$start* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36m$start\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->statements.n)
	{
		for (unsigned i = 0, n = ptree->statements.n; i < n; i++)
		{
			char label[10 + 30];
			snprintf(label, sizeof(label), "statements[%u]", i);
			print_zebu_statement(new ?: links, i + 1 < n ? p_not_last_child : p_last_child, label, ptree->statements.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_last_child, "statement[]", "statements");
	}
	free(new);
}
void print_zebu_additive_expression(struct link* links, enum prefix p, const char* name, struct zebu_additive_expression* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36madditive_expression\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->inner)
		print_zebu_multiplicative_expression(new ?: links, p_not_last_child, "inner", ptree->inner);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "multiplicative_expression", "inner");
	if (ptree->left)
		print_zebu_additive_expression(new ?: links, p_not_last_child, "left", ptree->left);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "additive_expression", "left");
	if (ptree->minus)
		print_token_leaf(new ?: links, p_not_last_child, "minus", ptree->minus);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "minus");
	if (ptree->plus)
		print_token_leaf(new ?: links, p_not_last_child, "plus", ptree->plus);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "plus");
	if (ptree->right)
		print_zebu_multiplicative_expression(new ?: links, p_last_child, "right", ptree->right);
	else
		print_empty_leaf(new ?: links, p_last_child, "multiplicative_expression", "right");
	free(new);
}
void print_zebu_assignment(struct link* links, enum prefix p, const char* name, struct zebu_assignment* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36massignment\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->expression)
		print_zebu_expression(new ?: links, p_not_last_child, "expression", ptree->expression);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "expression", "expression");
	if (ptree->name)
		print_token_leaf(new ?: links, p_last_child, "name", ptree->name);
	else
		print_empty_leaf(new ?: links, p_last_child, "token", "name");
	free(new);
}
void print_zebu_expression(struct link* links, enum prefix p, const char* name, struct zebu_expression* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mexpression\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->sub)
		print_zebu_additive_expression(new ?: links, p_last_child, "sub", ptree->sub);
	else
		print_empty_leaf(new ?: links, p_last_child, "additive_expression", "sub");
	free(new);
}
void print_zebu_multiplicative_expression(struct link* links, enum prefix p, const char* name, struct zebu_multiplicative_expression* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mmultiplicative_expression\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->divide)
		print_token_leaf(new ?: links, p_not_last_child, "divide", ptree->divide);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "divide");
	if (ptree->inner)
		print_zebu_prefix_expression(new ?: links, p_not_last_child, "inner", ptree->inner);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "prefix_expression", "inner");
	if (ptree->left)
		print_zebu_multiplicative_expression(new ?: links, p_not_last_child, "left", ptree->left);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "multiplicative_expression", "left");
	if (ptree->right)
		print_zebu_prefix_expression(new ?: links, p_not_last_child, "right", ptree->right);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "prefix_expression", "right");
	if (ptree->times)
		print_token_leaf(new ?: links, p_last_child, "times", ptree->times);
	else
		print_empty_leaf(new ?: links, p_last_child, "token", "times");
	free(new);
}
void print_zebu_postfix_expression(struct link* links, enum prefix p, const char* name, struct zebu_postfix_expression* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mpostfix_expression\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->identifier)
		print_token_leaf(new ?: links, p_not_last_child, "identifier", ptree->identifier);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "identifier");
	if (ptree->literal)
		print_token_leaf(new ?: links, p_not_last_child, "literal", ptree->literal);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "literal");
	if (ptree->subexpression)
		print_zebu_expression(new ?: links, p_last_child, "subexpression", ptree->subexpression);
	else
		print_empty_leaf(new ?: links, p_last_child, "expression", "subexpression");
	free(new);
}
void print_zebu_prefix_expression(struct link* links, enum prefix p, const char* name, struct zebu_prefix_expression* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mprefix_expression\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->inner)
		print_zebu_postfix_expression(new ?: links, p_not_last_child, "inner", ptree->inner);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "postfix_expression", "inner");
	if (ptree->minus)
		print_token_leaf(new ?: links, p_not_last_child, "minus", ptree->minus);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "minus");
	if (ptree->plus)
		print_token_leaf(new ?: links, p_not_last_child, "plus", ptree->plus);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "plus");
	if (ptree->sub)
		print_zebu_prefix_expression(new ?: links, p_last_child, "sub", ptree->sub);
	else
		print_empty_leaf(new ?: links, p_last_child, "prefix_expression", "sub");
	free(new);
}
void print_zebu_statement(struct link* links, enum prefix p, const char* name, struct zebu_statement* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mstatement\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->assignment)
		print_zebu_assignment(new ?: links, p_not_last_child, "assignment", ptree->assignment);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "assignment", "assignment");
	if (ptree->transaction)
		print_zebu_transaction(new ?: links, p_last_child, "transaction", ptree->transaction);
	else
		print_empty_leaf(new ?: links, p_last_child, "transaction", "transaction");
	free(new);
}
void print_zebu_transaction(struct link* links, enum prefix p, const char* name, struct zebu_transaction* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mtransaction\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->day)
		print_zebu_postfix_expression(new ?: links, p_not_last_child, "day", ptree->day);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "postfix_expression", "day");
	if (ptree->delta)
		print_zebu_expression(new ?: links, p_not_last_child, "delta", ptree->delta);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "expression", "delta");
	if (ptree->hour)
		print_zebu_postfix_expression(new ?: links, p_not_last_child, "hour", ptree->hour);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "postfix_expression", "hour");
	if (ptree->minute)
		print_zebu_postfix_expression(new ?: links, p_not_last_child, "minute", ptree->minute);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "postfix_expression", "minute");
	if (ptree->month)
		print_zebu_postfix_expression(new ?: links, p_not_last_child, "month", ptree->month);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "postfix_expression", "month");
	if (ptree->second)
		print_zebu_postfix_expression(new ?: links, p_not_last_child, "second", ptree->second);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "postfix_expression", "second");
	if (ptree->year)
		print_zebu_postfix_expression(new ?: links, p_last_child, "year", ptree->year);
	else
		print_empty_leaf(new ?: links, p_last_child, "postfix_expression", "year");
	free(new);
}

#endif

struct zebu_token* inc_zebu_token(struct zebu_token* token)
{
	if (token) token->refcount++;
	return token;
}
struct zebu_$start* inc_zebu_$start(struct zebu_$start* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_additive_expression* inc_zebu_additive_expression(struct zebu_additive_expression* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_assignment* inc_zebu_assignment(struct zebu_assignment* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_expression* inc_zebu_expression(struct zebu_expression* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_multiplicative_expression* inc_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_postfix_expression* inc_zebu_postfix_expression(struct zebu_postfix_expression* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_prefix_expression* inc_zebu_prefix_expression(struct zebu_prefix_expression* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_statement* inc_zebu_statement(struct zebu_statement* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_transaction* inc_zebu_transaction(struct zebu_transaction* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}



extern void free_zebu_token(struct zebu_token* token);
extern void free_zebu_$start(struct zebu_$start* ptree);

extern void free_zebu_additive_expression(struct zebu_additive_expression* ptree);

extern void free_zebu_assignment(struct zebu_assignment* ptree);

extern void free_zebu_expression(struct zebu_expression* ptree);

extern void free_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree);

extern void free_zebu_postfix_expression(struct zebu_postfix_expression* ptree);

extern void free_zebu_prefix_expression(struct zebu_prefix_expression* ptree);

extern void free_zebu_statement(struct zebu_statement* ptree);

extern void free_zebu_transaction(struct zebu_transaction* ptree);

void free_zebu_token(struct zebu_token* token)
{
	if (token && !--token->refcount)
	{
		free(token->data);
		free(token);
	}
}
void free_zebu_$start(struct zebu_$start* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		for (unsigned i = 0, n = ptree->statements.n; i < n; i++)
			free_zebu_statement(ptree->statements.data[i]);
		free(ptree->statements.data);
		free(ptree);
	}
}

void free_zebu_additive_expression(struct zebu_additive_expression* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_multiplicative_expression(ptree->inner);
		free_zebu_additive_expression(ptree->left);
		free_zebu_token(ptree->minus);
		free_zebu_token(ptree->plus);
		free_zebu_multiplicative_expression(ptree->right);
		free(ptree);
	}
}

void free_zebu_assignment(struct zebu_assignment* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_expression(ptree->expression);
		free_zebu_token(ptree->name);
		free(ptree);
	}
}

void free_zebu_expression(struct zebu_expression* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_additive_expression(ptree->sub);
		free(ptree);
	}
}

void free_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_token(ptree->divide);
		free_zebu_prefix_expression(ptree->inner);
		free_zebu_multiplicative_expression(ptree->left);
		free_zebu_prefix_expression(ptree->right);
		free_zebu_token(ptree->times);
		free(ptree);
	}
}

void free_zebu_postfix_expression(struct zebu_postfix_expression* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_token(ptree->identifier);
		free_zebu_token(ptree->literal);
		free_zebu_expression(ptree->subexpression);
		free(ptree);
	}
}

void free_zebu_prefix_expression(struct zebu_prefix_expression* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_postfix_expression(ptree->inner);
		free_zebu_token(ptree->minus);
		free_zebu_token(ptree->plus);
		free_zebu_prefix_expression(ptree->sub);
		free(ptree);
	}
}

void free_zebu_statement(struct zebu_statement* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_assignment(ptree->assignment);
		free_zebu_transaction(ptree->transaction);
		free(ptree);
	}
}

void free_zebu_transaction(struct zebu_transaction* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_postfix_expression(ptree->day);
		free_zebu_expression(ptree->delta);
		free_zebu_postfix_expression(ptree->hour);
		free_zebu_postfix_expression(ptree->minute);
		free_zebu_postfix_expression(ptree->month);
		free_zebu_postfix_expression(ptree->second);
		free_zebu_postfix_expression(ptree->year);
		free(ptree);
	}
}



#define argv0 (program_invocation_name)

#define N(array) (sizeof(array) / sizeof(*array))

#ifdef ZEBU_DEBUG
static void escape(char *out, unsigned char in)
{
	switch (in)
	{
		case ' ':
		case '~':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '-':
		case '+':
		case '=':
		case '|':
		case '/':
		case '<': case '>':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case ':': case ';':
		case ',': case '.':
		case '_':
		case '0' ... '9':
		case 'a' ... 'z':
		case 'A' ... 'Z':
			*out++ = in;
			*out = 0;
			break;
		
		case '\\': *out++ = '\\', *out++ = '\\', *out = 0; break;
		
		case '\'': *out++ = '\\', *out++ = '\'', *out = 0; break;
		
		case '\"': *out++ = '\\', *out++ = '\"', *out = 0; break;
		
		case '\t': *out++ = '\\', *out++ = 't', *out = 0; break;
		
		case '\n': *out++ = '\\', *out++ = 'n', *out = 0; break;
		
		default:
			sprintf(out, "\\x%02hhX", in);
			break;
	}
}
#endif

struct zebu_$start* zebu_parse(FILE* stream)
{
	void* root;
	struct { unsigned* data, n, cap; } yacc = {};
	struct { void** data; unsigned n, cap; } data = {};
	struct { unsigned char* data; unsigned n, cap; unsigned line; } lexer = {
		.line = 1,
	};
	
	void push_state(unsigned y)
	{
		if (yacc.n + 1 >= yacc.cap)
		{
			yacc.cap = yacc.cap << 1 ?: 1;
			yacc.data = realloc(yacc.data, sizeof(*yacc.data) * yacc.cap);
		}
		yacc.data[yacc.n++] = y;
	}
	
	void push_data(void* d)
	{
		if (data.n + 1 >= data.cap)
		{
			data.cap = data.cap << 1 ?: 1;
			data.data = realloc(data.data, sizeof(*data.data) * data.cap);
		}
		data.data[data.n++] = d;
	}
	
	#ifdef ZEBU_DEBUG
	void ddprintf(const char* fmt, ...)
	{
		for (unsigned i = 0, n = yacc.n; i < n; i++)
			printf("%u ", yacc.data[i]);
		
		printf("| ");
		
		va_list va;
		va_start(va, fmt);
		vprintf(fmt, va);
		va_end(va);
	}
	#endif
	
	void push_char(unsigned char c)
	{
		while (lexer.n + 1 >= lexer.cap)
		{
			lexer.cap = lexer.cap << 1 ?: 1;
			#ifdef ZEBU_DEBUG
			ddprintf("lexer.cap == %u\n", lexer.cap);
			#endif
			lexer.data = realloc(lexer.data, lexer.cap);
		}
		
		lexer.data[lexer.n++] = c;
	}
	
	unsigned y, t, s, r;
	void* td;
	
	void read_token(unsigned l)
	{
		unsigned original_l = l, i = 0, a, b, c, f = 0;
		
		unsigned line = lexer.line;
		
		t = 0;
		
		#ifdef ZEBU_DEBUG
		ddprintf("lexer: \"%.*s\": l = %u\n", lexer.n, lexer.data, l);
		#endif
		
		while (1)
		{
			if (i < lexer.n)
			{
				c = lexer.data[i];
				
				#ifdef ZEBU_DEBUG
				char escaped[10];
				
				escape(escaped, c);
				
				ddprintf("lexer: c = '%s' (0x%X) (from cache)\n", escaped, c);
				#endif
				
				a = l < N(zebu_lexer) && c < N(*zebu_lexer) ? zebu_lexer[l][c] : 0;
			}
			else if ((c = getc(stream)) != EOF)
			{
				push_char(c);
				
				#ifdef ZEBU_DEBUG
				char escaped[10];
				
				escape(escaped, c);
				
				ddprintf("lexer: c = '%s' (0x%X)\n", escaped, c);
				#endif
				
				a = l < N(zebu_lexer) && c < N(*zebu_lexer) ? zebu_lexer[l][c] : 0;
			}
			else
			{
				c = EOF;
				
				#ifdef ZEBU_DEBUG
				ddprintf("lexer: c = <EOF>\n");
				#endif
				
				a = l < N(zebu_lexer_EOFs) ? zebu_lexer_EOFs[l] : 0;
			}
			
			b = l < N(zebu_lexer_accepts) ? zebu_lexer_accepts[l] : 0;
			
			#ifdef ZEBU_DEBUG
			ddprintf("lexer: \"%.*s\" (%u): a = %u, b = %u\n", lexer.n, lexer.data, i, a, b);
			#endif
			
			if (a)
			{
				if (b)
				{
					l = a, t = b, f = i++, lexer.line = line;
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: l = %u\n", l);
					#endif
				}
				else
				{
					l = a, i++;
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: l = %u\n", l);
					#endif
				}
				
				if (c == '\n')
				{
					line++;
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: line: %u\n", line);
					#endif
				}
			}
			else if (b)
			{
				#ifdef ZEBU_DEBUG
				ddprintf("lexer: token: \"%.*s\", line: %u\n", i, lexer.data, line);
				#endif
				
				if (!lexer.n)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: EOF.\n");
					#endif
					t = b, td = NULL;
					break;
				}
				else if (b == 1)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: whitespace: \"%.*s\"\n", i, lexer.data);
					#endif
					
					l = original_l, t = 0, lexer.line = line;
					memmove(lexer.data, lexer.data + i, lexer.n - i), lexer.n -= i, i = 0;
				}
				else
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: i = %u\n", i);
					#endif
					
					struct zebu_token* token = malloc(sizeof(*token));
					token->refcount = 1;
					token->line = line;
					token->data = memcpy(malloc(i + 1), lexer.data, i);
					token->data[i] = 0;
					token->len = i;
					t = b, td = token;
					
					lexer.line = line;
					memmove(lexer.data, lexer.data + i, lexer.n - i), lexer.n -= i;
					break;
				}
			}
			else if (t)
			{
				if (t == 1)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: falling back to whitespace: \"%.*s\"\n", f, lexer.data);
					#endif
					
					l = original_l, t = 0, line = lexer.line;
					memmove(lexer.data, lexer.data + f, lexer.n - f), lexer.n -= f, f = 0, i = 0;
				}
				else
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: falling back to token: \"%.*s\"\n", f, lexer.data);
					#endif
					
					struct zebu_token* token = malloc(sizeof(*token));
					token->refcount = 1;
					token->line = lexer.line;
					token->data = memcpy(malloc(f + 1), lexer.data, f);
					token->data[f] = 0;
					token->len = f;
					td = token;
					
					memmove(lexer.data, lexer.data + f, lexer.n - f), lexer.n -= f, f = 0;
					break;
				}
			}
			else
			{
				if (i != 0)
				{
					if (c == (unsigned) EOF)
						fprintf(stderr, "%s: unexpected '<EOF>' when reading '%.*s' on line %u!\n", argv0, i, lexer.data, line);
					else
						fprintf(stderr, "%s: unexpected '%c' when reading '%.*s' on line %u!\n", argv0, c, i, lexer.data, line);
				}
				else
				{
					if (c == (unsigned) EOF)
						fprintf(stderr, "%s: unexpected '<EOF>' on line %u!\n", argv0, line);
					else
						fprintf(stderr, "%s: unexpected '%c' on line %u!\n", argv0, c, line);
				}
				exit(1);
			}
		}
	}
	
	push_state(1), y = 1, read_token(1);
	
	#ifdef ZEBU_DEBUG
	ddprintf("y = %u, t = %u\n", y, t);
	#endif
	
	while (yacc.n)
	{
		if (y < N(zebu_shifts) && t < N(*zebu_shifts) && (s = zebu_shifts[y][t]))
		{
			#ifdef ZEBU_DEBUG
			ddprintf("s == %u\n", s);
			#endif
			
			y = s, push_state(y), push_data(td);
			
			read_token(zebu_lexer_starts[y]);
			
			#ifdef ZEBU_DEBUG
			ddprintf("t = %u\n", t);
			#endif
		}
		else if (y < N( zebu_reduces) && t < N(*zebu_reduces) && (r = zebu_reduces[y][t]))
		{
			#ifdef ZEBU_DEBUG
			ddprintf("r == %u\n", r);
			#endif
			
			unsigned g;
			void* d;
			
			switch (r)
{
	case 4:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_statement* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->statements.n == value->statements.cap)
{
value->statements.cap = value->statements.cap << 1 ?: 1;
value->statements.data = realloc(value->statements.data, sizeof(*value->statements.data) * value->statements.cap);
}
memmove(value->statements.data + 1, value->statements.data, sizeof(*value->statements.data) * value->statements.n);
value->statements.data[0] = inc_zebu_statement(subgrammar), value->statements.n++;
free_zebu_statement(subgrammar);
}
		d = value, g = 14;
		break;
	}
	case 10:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_$start* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->statements.n)
{
while (value->statements.n + trie->statements.n > value->statements.cap)
{
value->statements.cap = value->statements.cap << 1 ?: 1;
value->statements.data = realloc(value->statements.data, sizeof(*value->statements.data) * value->statements.cap);
}
memmove(value->statements.data + trie->statements.n, value->statements.data, sizeof(*value->statements.data) * value->statements.n);
for (unsigned i = 0, n = trie->statements.n; i < n; i++)
value->statements.data[i] = inc_zebu_statement(trie->statements.data[i]);
value->statements.n += trie->statements.n;
}
free_zebu_$start(trie);
}
{
struct zebu_statement* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->statements.n == value->statements.cap)
{
value->statements.cap = value->statements.cap << 1 ?: 1;
value->statements.data = realloc(value->statements.data, sizeof(*value->statements.data) * value->statements.cap);
}
memmove(value->statements.data + 1, value->statements.data, sizeof(*value->statements.data) * value->statements.n);
value->statements.data[0] = inc_zebu_statement(subgrammar), value->statements.n++;
free_zebu_statement(subgrammar);
}
		d = value, g = 14;
		break;
	}
	case 17:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_$start* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->statements.n)
{
while (value->statements.n + trie->statements.n > value->statements.cap)
{
value->statements.cap = value->statements.cap << 1 ?: 1;
value->statements.data = realloc(value->statements.data, sizeof(*value->statements.data) * value->statements.cap);
}
memmove(value->statements.data + trie->statements.n, value->statements.data, sizeof(*value->statements.data) * value->statements.n);
for (unsigned i = 0, n = trie->statements.n; i < n; i++)
value->statements.data[i] = inc_zebu_statement(trie->statements.data[i]);
value->statements.n += trie->statements.n;
}
free_zebu_$start(trie);
}
{
struct zebu_statement* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->statements.n == value->statements.cap)
{
value->statements.cap = value->statements.cap << 1 ?: 1;
value->statements.data = realloc(value->statements.data, sizeof(*value->statements.data) * value->statements.cap);
}
memmove(value->statements.data + 1, value->statements.data, sizeof(*value->statements.data) * value->statements.n);
value->statements.data[0] = inc_zebu_statement(subgrammar), value->statements.n++;
free_zebu_statement(subgrammar);
}
		d = value, g = 9;
		break;
	}
	case 11:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_statement* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->statements.n == value->statements.cap)
{
value->statements.cap = value->statements.cap << 1 ?: 1;
value->statements.data = realloc(value->statements.data, sizeof(*value->statements.data) * value->statements.cap);
}
memmove(value->statements.data + 1, value->statements.data, sizeof(*value->statements.data) * value->statements.n);
value->statements.data[0] = inc_zebu_statement(subgrammar), value->statements.n++;
free_zebu_statement(subgrammar);
}
		d = value, g = 9;
		break;
	}
	case 20:
	{
		struct zebu_additive_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_multiplicative_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_multiplicative_expression(value->right), value->right = inc_zebu_multiplicative_expression(subgrammar);
free_zebu_multiplicative_expression(subgrammar);
}
		d = value, g = 12;
		break;
	}
	case 23:
	{
		struct zebu_multiplicative_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_prefix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_prefix_expression(value->right), value->right = inc_zebu_prefix_expression(subgrammar);
free_zebu_prefix_expression(subgrammar);
}
		d = value, g = 13;
		break;
	}
	case 13:
	{
		struct zebu_prefix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_prefix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_prefix_expression(value->sub), value->sub = inc_zebu_prefix_expression(subgrammar);
free_zebu_prefix_expression(subgrammar);
}
		d = value, g = 10;
		break;
	}
	case 25:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_expression(value->delta), value->delta = inc_zebu_expression(subgrammar);
free_zebu_expression(subgrammar);
}
		d = value, g = 11;
		break;
	}
	case 19:
	{
		struct zebu_additive_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_additive_expression* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->inner) { free_zebu_multiplicative_expression(value->inner); value->inner = inc_zebu_multiplicative_expression(trie->inner); }
if (trie->left) { free_zebu_additive_expression(value->left); value->left = inc_zebu_additive_expression(trie->left); }
if (trie->minus) { free_zebu_token(value->minus); value->minus = inc_zebu_token(trie->minus); }
if (trie->plus) { free_zebu_token(value->plus); value->plus = inc_zebu_token(trie->plus); }
if (trie->right) { free_zebu_multiplicative_expression(value->right); value->right = inc_zebu_multiplicative_expression(trie->right); }
free_zebu_additive_expression(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->plus), value->plus = inc_zebu_token(token);
free_zebu_token(token);
}
{
struct zebu_additive_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_additive_expression(value->left), value->left = inc_zebu_additive_expression(subgrammar);
free_zebu_additive_expression(subgrammar);
}
		d = value, g = 5;
		break;
	}
	case 21:
	{
		struct zebu_additive_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_additive_expression* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->inner) { free_zebu_multiplicative_expression(value->inner); value->inner = inc_zebu_multiplicative_expression(trie->inner); }
if (trie->left) { free_zebu_additive_expression(value->left); value->left = inc_zebu_additive_expression(trie->left); }
if (trie->minus) { free_zebu_token(value->minus); value->minus = inc_zebu_token(trie->minus); }
if (trie->plus) { free_zebu_token(value->plus); value->plus = inc_zebu_token(trie->plus); }
if (trie->right) { free_zebu_multiplicative_expression(value->right); value->right = inc_zebu_multiplicative_expression(trie->right); }
free_zebu_additive_expression(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->minus), value->minus = inc_zebu_token(token);
free_zebu_token(token);
}
{
struct zebu_additive_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_additive_expression(value->left), value->left = inc_zebu_additive_expression(subgrammar);
free_zebu_additive_expression(subgrammar);
}
		d = value, g = 5;
		break;
	}
	case 7:
	{
		struct zebu_additive_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_multiplicative_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_multiplicative_expression(value->inner), value->inner = inc_zebu_multiplicative_expression(subgrammar);
free_zebu_multiplicative_expression(subgrammar);
}
		d = value, g = 5;
		break;
	}
	case 18:
	{
		struct zebu_assignment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_expression(value->expression), value->expression = inc_zebu_expression(subgrammar);
free_zebu_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->name), value->name = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 1;
		break;
	}
	case 6:
	{
		struct zebu_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_additive_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_additive_expression(value->sub), value->sub = inc_zebu_additive_expression(subgrammar);
free_zebu_additive_expression(subgrammar);
}
		d = value, g = 6;
		break;
	}
	case 22:
	{
		struct zebu_multiplicative_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_multiplicative_expression* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->divide) { free_zebu_token(value->divide); value->divide = inc_zebu_token(trie->divide); }
if (trie->inner) { free_zebu_prefix_expression(value->inner); value->inner = inc_zebu_prefix_expression(trie->inner); }
if (trie->left) { free_zebu_multiplicative_expression(value->left); value->left = inc_zebu_multiplicative_expression(trie->left); }
if (trie->right) { free_zebu_prefix_expression(value->right); value->right = inc_zebu_prefix_expression(trie->right); }
if (trie->times) { free_zebu_token(value->times); value->times = inc_zebu_token(trie->times); }
free_zebu_multiplicative_expression(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->times), value->times = inc_zebu_token(token);
free_zebu_token(token);
}
{
struct zebu_multiplicative_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_multiplicative_expression(value->left), value->left = inc_zebu_multiplicative_expression(subgrammar);
free_zebu_multiplicative_expression(subgrammar);
}
		d = value, g = 7;
		break;
	}
	case 24:
	{
		struct zebu_multiplicative_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_multiplicative_expression* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->divide) { free_zebu_token(value->divide); value->divide = inc_zebu_token(trie->divide); }
if (trie->inner) { free_zebu_prefix_expression(value->inner); value->inner = inc_zebu_prefix_expression(trie->inner); }
if (trie->left) { free_zebu_multiplicative_expression(value->left); value->left = inc_zebu_multiplicative_expression(trie->left); }
if (trie->right) { free_zebu_prefix_expression(value->right); value->right = inc_zebu_prefix_expression(trie->right); }
if (trie->times) { free_zebu_token(value->times); value->times = inc_zebu_token(trie->times); }
free_zebu_multiplicative_expression(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->divide), value->divide = inc_zebu_token(token);
free_zebu_token(token);
}
{
struct zebu_multiplicative_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_multiplicative_expression(value->left), value->left = inc_zebu_multiplicative_expression(subgrammar);
free_zebu_multiplicative_expression(subgrammar);
}
		d = value, g = 7;
		break;
	}
	case 9:
	{
		struct zebu_multiplicative_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_prefix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_prefix_expression(value->inner), value->inner = inc_zebu_prefix_expression(subgrammar);
free_zebu_prefix_expression(subgrammar);
}
		d = value, g = 7;
		break;
	}
	case 1:
	{
		struct zebu_postfix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->identifier), value->identifier = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 2;
		break;
	}
	case 2:
	{
		struct zebu_postfix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->literal), value->literal = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 2;
		break;
	}
	case 15:
	{
		struct zebu_postfix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_expression(value->subexpression), value->subexpression = inc_zebu_expression(subgrammar);
free_zebu_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 2;
		break;
	}
	case 12:
	{
		struct zebu_prefix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_prefix_expression* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->inner) { free_zebu_postfix_expression(value->inner); value->inner = inc_zebu_postfix_expression(trie->inner); }
if (trie->minus) { free_zebu_token(value->minus); value->minus = inc_zebu_token(trie->minus); }
if (trie->plus) { free_zebu_token(value->plus); value->plus = inc_zebu_token(trie->plus); }
if (trie->sub) { free_zebu_prefix_expression(value->sub); value->sub = inc_zebu_prefix_expression(trie->sub); }
free_zebu_prefix_expression(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->plus), value->plus = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 8;
		break;
	}
	case 14:
	{
		struct zebu_prefix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_prefix_expression* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->inner) { free_zebu_postfix_expression(value->inner); value->inner = inc_zebu_postfix_expression(trie->inner); }
if (trie->minus) { free_zebu_token(value->minus); value->minus = inc_zebu_token(trie->minus); }
if (trie->plus) { free_zebu_token(value->plus); value->plus = inc_zebu_token(trie->plus); }
if (trie->sub) { free_zebu_prefix_expression(value->sub); value->sub = inc_zebu_prefix_expression(trie->sub); }
free_zebu_prefix_expression(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->minus), value->minus = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 8;
		break;
	}
	case 8:
	{
		struct zebu_prefix_expression* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->inner), value->inner = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 8;
		break;
	}
	case 3:
	{
		struct zebu_statement* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_assignment* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_assignment(value->assignment), value->assignment = inc_zebu_assignment(subgrammar);
free_zebu_assignment(subgrammar);
}
		d = value, g = 3;
		break;
	}
	case 5:
	{
		struct zebu_statement* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_transaction(value->transaction), value->transaction = inc_zebu_transaction(subgrammar);
free_zebu_transaction(subgrammar);
}
		d = value, g = 3;
		break;
	}
	case 30:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->day) { free_zebu_postfix_expression(value->day); value->day = inc_zebu_postfix_expression(trie->day); }
if (trie->delta) { free_zebu_expression(value->delta); value->delta = inc_zebu_expression(trie->delta); }
if (trie->hour) { free_zebu_postfix_expression(value->hour); value->hour = inc_zebu_postfix_expression(trie->hour); }
if (trie->minute) { free_zebu_postfix_expression(value->minute); value->minute = inc_zebu_postfix_expression(trie->minute); }
if (trie->month) { free_zebu_postfix_expression(value->month); value->month = inc_zebu_postfix_expression(trie->month); }
if (trie->second) { free_zebu_postfix_expression(value->second); value->second = inc_zebu_postfix_expression(trie->second); }
if (trie->year) { free_zebu_postfix_expression(value->year); value->year = inc_zebu_postfix_expression(trie->year); }
free_zebu_transaction(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->second), value->second = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->minute), value->minute = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->hour), value->hour = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->day), value->day = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->month), value->month = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->year), value->year = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 4;
		break;
	}
	case 29:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->day) { free_zebu_postfix_expression(value->day); value->day = inc_zebu_postfix_expression(trie->day); }
if (trie->delta) { free_zebu_expression(value->delta); value->delta = inc_zebu_expression(trie->delta); }
if (trie->hour) { free_zebu_postfix_expression(value->hour); value->hour = inc_zebu_postfix_expression(trie->hour); }
if (trie->minute) { free_zebu_postfix_expression(value->minute); value->minute = inc_zebu_postfix_expression(trie->minute); }
if (trie->month) { free_zebu_postfix_expression(value->month); value->month = inc_zebu_postfix_expression(trie->month); }
if (trie->second) { free_zebu_postfix_expression(value->second); value->second = inc_zebu_postfix_expression(trie->second); }
if (trie->year) { free_zebu_postfix_expression(value->year); value->year = inc_zebu_postfix_expression(trie->year); }
free_zebu_transaction(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->minute), value->minute = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->hour), value->hour = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->day), value->day = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->month), value->month = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->year), value->year = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 4;
		break;
	}
	case 28:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->day) { free_zebu_postfix_expression(value->day); value->day = inc_zebu_postfix_expression(trie->day); }
if (trie->delta) { free_zebu_expression(value->delta); value->delta = inc_zebu_expression(trie->delta); }
if (trie->hour) { free_zebu_postfix_expression(value->hour); value->hour = inc_zebu_postfix_expression(trie->hour); }
if (trie->minute) { free_zebu_postfix_expression(value->minute); value->minute = inc_zebu_postfix_expression(trie->minute); }
if (trie->month) { free_zebu_postfix_expression(value->month); value->month = inc_zebu_postfix_expression(trie->month); }
if (trie->second) { free_zebu_postfix_expression(value->second); value->second = inc_zebu_postfix_expression(trie->second); }
if (trie->year) { free_zebu_postfix_expression(value->year); value->year = inc_zebu_postfix_expression(trie->year); }
free_zebu_transaction(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->hour), value->hour = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->day), value->day = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->month), value->month = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->year), value->year = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 4;
		break;
	}
	case 27:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->day) { free_zebu_postfix_expression(value->day); value->day = inc_zebu_postfix_expression(trie->day); }
if (trie->delta) { free_zebu_expression(value->delta); value->delta = inc_zebu_expression(trie->delta); }
if (trie->hour) { free_zebu_postfix_expression(value->hour); value->hour = inc_zebu_postfix_expression(trie->hour); }
if (trie->minute) { free_zebu_postfix_expression(value->minute); value->minute = inc_zebu_postfix_expression(trie->minute); }
if (trie->month) { free_zebu_postfix_expression(value->month); value->month = inc_zebu_postfix_expression(trie->month); }
if (trie->second) { free_zebu_postfix_expression(value->second); value->second = inc_zebu_postfix_expression(trie->second); }
if (trie->year) { free_zebu_postfix_expression(value->year); value->year = inc_zebu_postfix_expression(trie->year); }
free_zebu_transaction(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->day), value->day = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->month), value->month = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->year), value->year = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 4;
		break;
	}
	case 26:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->day) { free_zebu_postfix_expression(value->day); value->day = inc_zebu_postfix_expression(trie->day); }
if (trie->delta) { free_zebu_expression(value->delta); value->delta = inc_zebu_expression(trie->delta); }
if (trie->hour) { free_zebu_postfix_expression(value->hour); value->hour = inc_zebu_postfix_expression(trie->hour); }
if (trie->minute) { free_zebu_postfix_expression(value->minute); value->minute = inc_zebu_postfix_expression(trie->minute); }
if (trie->month) { free_zebu_postfix_expression(value->month); value->month = inc_zebu_postfix_expression(trie->month); }
if (trie->second) { free_zebu_postfix_expression(value->second); value->second = inc_zebu_postfix_expression(trie->second); }
if (trie->year) { free_zebu_postfix_expression(value->year); value->year = inc_zebu_postfix_expression(trie->year); }
free_zebu_transaction(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->month), value->month = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->year), value->year = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 4;
		break;
	}
	case 16:
	{
		struct zebu_transaction* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_transaction* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->day) { free_zebu_postfix_expression(value->day); value->day = inc_zebu_postfix_expression(trie->day); }
if (trie->delta) { free_zebu_expression(value->delta); value->delta = inc_zebu_expression(trie->delta); }
if (trie->hour) { free_zebu_postfix_expression(value->hour); value->hour = inc_zebu_postfix_expression(trie->hour); }
if (trie->minute) { free_zebu_postfix_expression(value->minute); value->minute = inc_zebu_postfix_expression(trie->minute); }
if (trie->month) { free_zebu_postfix_expression(value->month); value->month = inc_zebu_postfix_expression(trie->month); }
if (trie->second) { free_zebu_postfix_expression(value->second); value->second = inc_zebu_postfix_expression(trie->second); }
if (trie->year) { free_zebu_postfix_expression(value->year); value->year = inc_zebu_postfix_expression(trie->year); }
free_zebu_transaction(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_postfix_expression* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_postfix_expression(value->year), value->year = inc_zebu_postfix_expression(subgrammar);
free_zebu_postfix_expression(subgrammar);
}
		d = value, g = 4;
		break;
	}
}
			
			if (g == 14)
			{
				free_zebu_token(td);
				yacc.n = 0, root = d;
			}
			else
			{
				y = yacc.data[yacc.n - 1];
				
				#ifdef ZEBU_DEBUG
				ddprintf("y = %u\n", y);
				#endif
				
				assert(y < N(zebu_gotos) && g < N(*zebu_gotos));
				
				s = zebu_gotos[y][g];
				
				#ifdef ZEBU_DEBUG
				ddprintf("s = %u\n", s);
				#endif
				
				y = s, push_state(y), push_data(d);
			}
		}
		else
		{
			struct zebu_token* token = td;
			
			fprintf(stderr, "zebu: unexpected token '%.*s'!\n", token->len, token->data);
			
			exit(1);
		}
	}
	
	#ifdef ZEBU_DEBUG
	print_zebu_$start(NULL, p_root, "start", root);
	#endif
	
	free(yacc.data);
	free(data.data);
	free(lexer.data);
	
	return root;
}

















