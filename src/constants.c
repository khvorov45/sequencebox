#include "mltaln.h"

#define DEFAULTGOP_B -1530
#define DEFAULTGEP_B -00
#define DEFAULTOFS_B -123

#define DEFAULTGOP_N -1530
#define DEFAULTGEP_N 0
#define DEFAULTOFS_N -369
#define DEFAULTPAMN 200

#define DEFAULTRNAGOP_N -1530
#define DEFAULTRNAGEP_N 0
#define DEFAULTRNATHR_N 0
#define DEFAULTGOP_J -1530
#define DEFAULTGEP_J -00
#define DEFAULTOFS_J -123 /* +10 -- -50  teido ka ? */
#define DEFAULTPAMN 200

int  locpenaltym = -1440;
int  exgpm = +0;
char locaminom[] = "ARNDCQEGHILKMFPSTWYVBZX.-J";
char locgrpm[] = {0, 3, 2, 2, 5, 2, 2, 0, 3, 1, 1, 3, 1, 4, 0, 0, 0, 4, 4, 1, 2, 2, 6, 6, 6, 1};

// clang-format off
int locn_dism[26][26] = 
    {
	{
  600, -235,   91,  -78,  202,   51, -103,  340,  -21, -169,
 -189, -246,  -92, -323,  582,  454,  342, -400, -309,   71,
    7,  -26,  -15, -400,    0,-1400,
	},

	{
 -235,  600,   17,  -69, -275,  277,  185, -400,  365, -112,
 -149,  485,  -55, -106, -229, -183,   20, -178,   22,  -95,
  -26,  231,  -15, -400,    0,-1400,
	},

	{
   91,   17,  600,  414, -209,  317,  357,   39,  231, -363,
 -398,   74, -280, -400,   85,  225,  200, -400, -378, -189,
  507,  337,  -15, -400,    0,-1400,
	},

	{
  -78,  -69,  414,  600, -395,  179,  342,  -78,  108, -400,
 -400,   14, -400, -400,  -86,   65,   14, -400, -400, -372,
  507,  261,  -15, -400,    0,-1400,
	},

	{
  202, -275, -209, -395,  600, -109, -332,  -35, -132,  134,
  128, -335,  182,  -40,  220,   74,  185, -355,  -81,  354,
 -302, -220,  -15, -400,    0,-1400,
	},

	{
   51,  277,  317,  179, -109,  600,  360, -109,  508, -135,
 -172,  297,  -58, -203,   51,  128,  280, -378, -109,   -9,
  248,  480,  -15, -400,    0,-1400,
	},

	{
 -103,  185,  357,  342, -332,  360,  600, -195,  325, -369,
 -400,  274, -295, -400, -109,   11,   77, -400, -321, -249,
  350,  480,  -15, -400,    0,-1400,
	},

	{
  340, -400,   39,  -78,  -35, -109, -195,  600, -195, -400,
 -400, -400, -355, -400,  322,  357,  114, -400, -400, -189,
  -19, -152,  -15, -400,    0,-1400,
	},

	{
  -21,  365,  231,  108, -132,  508,  325, -195,  600, -100,
 -141,  374,  -26, -152,  -15,   45,  222, -303,  -49,   -3,
  169,  417,  -15, -400,    0,-1400,
	},

	{
 -169, -112, -363, -400,  134, -135, -369, -400, -100,  600,
  560, -212,  517,  425, -149, -243,  -12,  108,  354,  357,
 -400, -252,  -15, -400,    0,-1400,
	},

	{
 -189, -149, -398, -400,  128, -172, -400, -400, -141,  560,
  600, -252,  482,  420, -172, -269,  -43,  105,  331,  340,
 -400, -290,  -15, -400,    0,-1400,
	},

	{
 -246,  485,   74,   14, -335,  297,  274, -400,  374, -212,
 -252,  600, -152, -215, -240, -175,   -1, -289,  -92, -172,
   44,  285,  -15, -400,    0,-1400,
	},

	{
  -92,  -55, -280, -400,  182,  -58, -295, -355,  -26,  517,
  482, -152,  600,  365,  -75, -163,   68,   59,  334,  422,
 -368, -176,  -15, -400,    0,-1400,
	},

	{
 -323, -106, -400, -400,  -40, -203, -400, -400, -152,  425,
  420, -215,  365,  600, -306, -386, -143,  282,  462,  191,
 -400, -315,  -15, -400,    0,-1400,
	},

	{
  582, -229,   85,  -86,  220,   51, -109,  322,  -15, -149,
 -172, -240,  -75, -306,  600,  440,  351, -400, -292,   88,
    0,  -29,  -15, -400,    0,-1400,
	},

	{
  454, -183,  225,   65,   74,  128,   11,  357,   45, -243,
 -269, -175, -163, -386,  440,  600,  345, -400, -352,  -15,
  145,   70,  -15, -400,    0,-1400,
	},

	{
  342,   20,  200,   14,  185,  280,   77,  114,  222,  -12,
  -43,   -1,   68, -143,  351,  345,  600, -400, -100,  194,
  107,  178,  -15, -400,    0,-1400,
	},

	{
 -400, -178, -400, -400, -355, -378, -400, -400, -303,  108,
  105, -289,   59,  282, -400, -400, -400,  600,  297, -118,
 -400, -400,  -15, -400,    0,-1400,
	},

	{
 -309,   22, -378, -400,  -81, -109, -321, -400,  -49,  354,
  331,  -92,  334,  462, -292, -352, -100,  297,  600,  165,
 -400, -215,  -15, -400,    0,-1400,
	},

	{
   71,  -95, -189, -372,  354,   -9, -249, -189,   -3,  357,
  340, -172,  422,  191,   88,  -15,  194, -118,  165,  600,
 -280, -129,  -15, -400,    0,-1400,
	},

	{
    7,  -26,  507,  507, -302,  248,  350,  -19,  169, -400,
 -400,   44, -368, -400,    0,  145,  107, -400, -400, -280,
  507,  299, -400, -400,    0,-1400,
	},

	{
  -26,  231,  337,  261, -220,  480,  480, -152,  417, -252,
 -290,  285, -176, -315,  -29,   70,  178, -400, -215, -129,
  299,  480, -400, -400,    0,-1400,
	},

	{
  -15,  -15,  -15,  -15,  -15,  -15,  -15,  -15,  -15,  -15,
  -15,  -15,  -15,  -15,  -15,  -15,  -15,  -15,  -15,  -15,
 -400, -400, -400, -400,    0,-1400,
	},

	{
 -400, -400, -400, -400, -400, -400, -400, -400, -400, -400,
 -400, -400, -400, -400, -400, -400, -400, -400, -400, -400,
 -400, -400, -400, -400,    0,-1400,
	},

	{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
	},

	{
-1400,-1400,-1400,-1400,-1400,-1400,-1400,-1400,-1400,-1400,
-1400,-1400,-1400,-1400,-1400,-1400,-1400,-1400,-1400,-1400,
-1400,-1400,-1400,-1400,    0, 1600,
	},
};
// clang-format on

double polarity_[] = {
    8.1, /* A */
    10.5, /* R */
    11.6, /* N */
    13.0, /* D */
    5.5, /* C */
    10.5, /* Q */
    12.3, /* E */
    9.0, /* G */
    10.4, /* H */
    5.2, /* I */
    4.9, /* L */
    11.3, /* K */
    5.7, /* M */
    5.2, /* F */
    8.0, /* P */
    9.2, /* S */
    8.6, /* T */
    5.4, /* W */
    6.2, /* Y */
    5.9, /* V */
};

double volume_[] = {
    31.0, /* A */
    124.0, /* R */
    56.0, /* N */
    54.0, /* D */
    55.0, /* C */
    85.0, /* Q */
    83.0, /* E */
    3.0, /* G */
    96.0, /* H */
    111.0, /* I */
    111.0, /* L */
    119.0, /* K */
    105.0, /* M */
    132.0, /* F */
    32.5, /* P */
    32.0, /* S */
    61.0, /* T */
    170.0, /* W */
    136.0, /* Y */
    84.0, /* V */
};

int  locpenaltyn = -1750;
char locaminon[] = "agctuAGCTUnNbdhkmnrsvwyx-O";
char locgrpn[] = {0, 1, 2, 3, 3, 0, 1, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int  exgpn = +00;

// clang-format off
double ribosum4[4][4] = {
//   a       g       c       t     
{    2.22,  -1.46,  -1.86,  -1.39, }, // a
{   -1.46,   1.03,  -2.48,  -1.74, }, // g
{   -1.86,  -2.48,   1.16,  -1.05, }, // c
{   -1.39,  -1.74,  -1.05,   1.65, }, // t
};

double ribosum16[16][16] = {
//   aa      ag      ac      at      ga      gg      gc      gt      ca      cg      cc      ct      ta      tg      tc      tt    
{   -2.49,  -8.24,  -7.04,  -4.32,  -6.86,  -8.39,  -5.03,  -5.84,  -8.84,  -4.68, -14.37, -12.64,  -4.01,  -6.16, -11.32,  -9.05, }, // aa
{   -8.24,  -0.80,  -8.89,  -5.13,  -8.61,  -5.38,  -5.77,  -6.60, -10.41,  -4.57, -14.53, -10.14,  -5.43,  -5.94,  -8.87, -11.07, }, // ag
{   -7.04,  -8.89,  -2.11,  -2.04,  -9.73, -11.05,  -3.81,  -4.72,  -9.37,  -5.86,  -9.08, -10.45,  -5.33,  -6.93,  -8.67,  -7.83, }, // ac
{   -4.32,  -5.13,  -2.04,   4.49,  -5.33,  -5.61,   2.70,   0.59,  -5.56,   1.67,  -6.71,  -5.17,   1.61,  -0.51,  -4.81,  -2.98, }, // at
{   -6.86,  -8.61,  -9.73,  -5.33,  -1.05,  -8.67,  -4.88,  -6.10,  -7.98,  -6.00, -12.43,  -7.71,  -5.85,  -7.55,  -6.63, -11.54, }, // ga
{   -8.39,  -5.38, -11.05,  -5.61,  -8.67,  -1.98,  -4.13,  -5.77, -11.36,  -4.66, -12.58, -13.69,  -5.75,  -4.27, -12.01, -10.79, }, // gg
{   -5.03,  -5.77,  -3.81,   2.70,  -4.88,  -4.13,   5.62,   1.21,  -5.95,   2.11,  -3.70,  -5.84,   1.60,  -0.08,  -4.49,  -3.90, }, // gc
{   -5.84,  -6.60,  -4.72,   0.59,  -6.10,  -5.77,   1.21,   3.47,  -7.93,  -0.27,  -7.88,  -5.61,  -0.57,  -2.09,  -5.30,  -4.45, }, // gt
{   -8.84, -10.41,  -9.37,  -5.56,  -7.98, -11.36,  -5.95,  -7.93,  -5.13,  -3.57, -10.45,  -8.49,  -2.42,  -5.63,  -7.08,  -8.39, }, // ca
{   -4.68,  -4.57,  -5.86,   1.67,  -6.00,  -4.66,   2.11,  -0.27,  -3.57,   5.36,  -5.71,  -4.96,   2.75,   1.32,  -4.91,  -3.67, }, // cg
{  -14.37, -14.53,  -9.08,  -6.71, -12.43, -12.58,  -3.70,  -7.88, -10.45,  -5.71,  -3.59,  -5.77,  -6.88,  -8.41,  -7.40,  -5.41, }, // cc
{  -12.64, -10.14, -10.45,  -5.17,  -7.71, -13.69,  -5.84,  -5.61,  -8.49,  -4.96,  -5.77,  -2.28,  -4.72,  -7.36,  -3.83,  -5.21, }, // ct
{   -4.01,  -5.43,  -5.33,   1.61,  -5.85,  -5.75,   1.60,  -0.57,  -2.42,   2.75,  -6.88,  -4.72,   4.97,   1.14,  -2.98,  -3.39, }, // ta
{   -6.16,  -5.94,  -6.93,  -0.51,  -7.55,  -4.27,  -0.08,  -2.09,  -5.63,   1.32,  -8.41,  -7.36,   1.14,   3.36,  -4.76,  -4.28, }, // tg
{  -11.32,  -8.87,  -8.67,  -4.81,  -6.63, -12.01,  -4.49,  -5.30,  -7.08,  -4.91,  -7.40,  -3.83,  -2.98,  -4.76,  -3.21,  -5.97, }, // tc
{   -9.05, -11.07,  -7.83,  -2.98, -11.54, -10.79,  -3.90,  -4.45,  -8.39,  -3.67,  -5.41,  -5.21,  -3.39,  -4.28,  -5.97,  -0.02, }, // tt
};

int locn_disn[26][26] = {
{
  1000,   600,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
},

{
   600,  1000,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
},

		{
     0,     0,  1000,   600,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,   600,  1000,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,   500,   500,     0,     0,     0,   500,   500,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  -500,
		},

		{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -500,
		},

		{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -500,
		},

		{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -500,
		},

		{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -500,
		},

		{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
		},

		{
 -500, -500, -500, -500, -500, -500, -500, -500, -500, -500,
 -500, -500, -500, -500, -500, -500, -500, -500, -500, -500,
 -500, -500, -500, -500,    0,  500,
		},
     };
//clang-format on

static void
BLOSUMmtx(int n, double** matrix, double* freq, unsigned char* amino, char* amino_grp) {
    char   locaminod[] = "ARNDCQEGHILKMFPSTWYVBZX.-J";
    char   locgrpd[] = {0, 3, 2, 2, 5, 2, 2, 0, 3, 1, 1, 3, 1, 4, 0, 0, 0, 4, 4, 1, 2, 2, 6, 6, 6, 1};
    double freqd[20] = {0.077, 0.051, 0.043, 0.052, 0.020, 0.041, 0.062, 0.074, 0.023, 0.052, 0.091, 0.059, 0.024, 0.040, 0.051, 0.069, 0.059, 0.014, 0.032, 0.066};

    // clang-format off
	double tmpmtx30[] = {
    4,
   -1,     8,
    0,    -2,     8,
    0,    -1,     1,     9,
   -3,    -2,    -1,    -3,    17,
    1,     3,    -1,    -1,    -2,     8,
    0,    -1,    -1,     1,     1,     2,     6,
    0,    -2,     0,    -1,    -4,    -2,    -2,     8,
   -2,    -1,    -1,    -2,    -5,     0,     0,    -3,    14,
    0,    -3,     0,    -4,    -2,    -2,    -3,    -1,    -2,     6,
   -1,    -2,    -2,    -1,     0,    -2,    -1,    -2,    -1,     2,     4,
    0,     1,     0,     0,    -3,     0,     2,    -1,    -2,    -2,    -2,     4,
    1,     0,     0,    -3,    -2,    -1,    -1,    -2,     2,     1,     2,     2,     6,
   -2,    -1,    -1,    -5,    -3,    -3,    -4,    -3,    -3,     0,     2,    -1,    -2,    10,
   -1,    -1,    -3,    -1,    -3,     0,     1,    -1,     1,    -3,    -3,     1,    -4,    -4,    11,
    1,    -1,     0,     0,    -2,    -1,     0,     0,    -1,    -1,    -2,     0,    -2,    -1,    -1,     4,
    1,    -3,     1,    -1,    -2,     0,    -2,    -2,    -2,     0,     0,    -1,     0,    -2,     0,     2,     5,
   -5,     0,    -7,    -4,    -2,    -1,    -1,     1,    -5,    -3,    -2,    -2,    -3,     1,    -3,    -3,    -5,    20,
   -4,     0,    -4,    -1,    -6,    -1,    -2,    -3,     0,    -1,     3,    -1,    -1,     3,    -2,    -2,    -1,     5,     9,
    1,    -1,    -2,    -2,    -2,    -3,    -3,    -3,    -3,     4,     1,    -2,     0,     1,    -4,    -1,     1,    -3,     1,     5,
    0,    -2,     4,     5,    -2,    -1,     0,     0,    -2,    -2,    -1,     0,    -2,    -3,    -2,     0,     0,    -5,    -3,    -2,     5,
    0,     0,    -1,     0,     0,     4,     5,    -2,     0,    -3,    -1,     1,    -1,    -4,     0,    -1,    -1,    -1,    -2,    -3,     0,     4,
    0,    -1,     0,    -1,    -2,     0,    -1,    -1,    -1,     0,     0,     0,     0,    -1,    -1,     0,     0,    -2,    -1,     0,    -1,     0,    -1};
	
	double tmpmtx45[] = {
    5,
   -2,      7,
   -1,      0,      6,
   -2,     -1,      2,      7,
   -1,     -3,     -2,     -3,     12,
   -1,      1,      0,      0,     -3,      6,
   -1,      0,      0,      2,     -3,      2,      6,
    0,     -2,      0,     -1,     -3,     -2,     -2,      7,
   -2,      0,      1,      0,     -3,      1,      0,     -2,     10,
   -1,     -3,     -2,     -4,     -3,     -2,     -3,     -4,     -3,      5,
   -1,     -2,     -3,     -3,     -2,     -2,     -2,     -3,     -2,      2,      5,
   -1,      3,      0,      0,     -3,      1,      1,     -2,     -1,     -3,     -3,      5,
   -1,     -1,     -2,     -3,     -2,      0,     -2,     -2,      0,      2,      2,     -1,      6,
   -2,     -2,     -2,     -4,     -2,     -4,     -3,     -3,     -2,      0,      1,     -3,      0,      8,
   -1,     -2,     -2,     -1,     -4,     -1,      0,     -2,     -2,     -2,     -3,     -1,     -2,     -3,      9,
    1,     -1,      1,      0,     -1,      0,      0,      0,     -1,     -2,     -3,     -1,     -2,     -2,     -1,      4,
    0,     -1,      0,     -1,     -1,     -1,     -1,     -2,     -2,     -1,     -1,     -1,     -1,     -1,     -1,      2,      5,
   -2,     -2,     -4,     -4,     -5,     -2,     -3,     -2,     -3,     -2,     -2,     -2,     -2,      1,     -3,     -4,     -3,     15,
   -2,     -1,     -2,     -2,     -3,     -1,     -2,     -3,      2,      0,      0,     -1,      0,      3,     -3,     -2,     -1,      3,      8,
    0,     -2,     -3,     -3,     -1,     -3,     -3,     -3,     -3,      3,      1,     -2,      1,      0,     -3,     -1,      0,     -3,     -1,      5};

    double tmpmtx50[] = {
    5,
   -2,      7,
   -1,     -1,      7,
   -2,     -2,      2,      8,
   -1,     -4,     -2,     -4,     13,
   -1,      1,      0,      0,     -3,      7,
   -1,      0,      0,      2,     -3,      2,      6,
    0,     -3,      0,     -1,     -3,     -2,     -3,      8,
   -2,      0,      1,     -1,     -3,      1,      0,     -2,     10,
   -1,     -4,     -3,     -4,     -2,     -3,     -4,     -4,     -4,      5,
   -2,     -3,     -4,     -4,     -2,     -2,     -3,     -4,     -3,      2,      5,
   -1,      3,      0,     -1,     -3,      2,      1,     -2,      0,     -3,     -3,      6,
   -1,     -2,     -2,     -4,     -2,      0,     -2,     -3,     -1,      2,      3,     -2,      7,
   -3,     -3,     -4,     -5,     -2,     -4,     -3,     -4,     -1,      0,      1,     -4,      0,      8,
   -1,     -3,     -2,     -1,     -4,     -1,     -1,     -2,     -2,     -3,     -4,     -1,     -3,     -4,     10,
    1,     -1,      1,      0,     -1,      0,     -1,      0,     -1,     -3,     -3,      0,     -2,     -3,     -1,      5,
    0,     -1,      0,     -1,     -1,     -1,     -1,     -2,     -2,     -1,     -1,     -1,     -1,     -2,     -1,      2,      5,
   -3,     -3,     -4,     -5,     -5,     -1,     -3,     -3,     -3,     -3,     -2,     -3,     -1,      1,     -4,     -4,     -3,     15,
   -2,     -1,     -2,     -3,     -3,     -1,     -2,     -3,      2,     -1,     -1,     -2,      0,      4,     -3,     -2,     -2,      2,      8,
    0,     -3,     -3,     -4,     -1,     -3,     -3,     -4,     -4,      4,      1,     -3,      1,     -1,     -3,     -2,      0,     -3,     -1,      5};

	double tmpmtx62[] = { 
    5.893685,
   -2.120252,  8.210189,
   -2.296072, -0.659672,  8.479856,
   -2.630151, -2.408668,  1.907550,  8.661363,
   -0.612761, -5.083814, -3.989626, -5.189966, 12.873172,
   -1.206025,  1.474162,  0.002529, -0.470069, -4.352838,  7.927704,
   -1.295821, -0.173087, -0.402015,  2.265459, -5.418729,  2.781955,  7.354247,
    0.239392, -3.456163, -0.634136, -1.970281, -3.750621, -2.677743, -3.165266,  8.344902,
   -2.437724, -0.374792,  0.867735, -1.678363, -4.481724,  0.672051, -0.176497, -3.061315, 11.266586,
   -1.982718, -4.485360, -4.825558, -4.681732, -1.841495, -4.154454, -4.791538, -5.587336, -4.847345,  5.997760,
   -2.196882, -3.231860, -5.068375, -5.408471, -1.916207, -3.200863, -4.269723, -5.440437, -4.180099,  2.282412,  5.774148,
   -1.101017,  3.163105, -0.268534, -1.052724, -4.554510,  1.908859,  1.163010, -2.291924, -1.081539, -4.005209, -3.670219,  6.756827,
   -1.402897, -2.050705, -3.226290, -4.587785, -2.129758, -0.631437, -2.997038, -4.014898, -2.326896,  1.690191,  2.987638, -2.032119,  8.088951,
   -3.315080, -4.179521, -4.491005, -5.225795, -3.563219, -4.746598, -4.788639, -4.661029, -1.851231, -0.241317,  0.622170, -4.618016,  0.018880,  9.069126,
   -1.221394, -3.162863, -3.000581, -2.220163, -4.192770, -1.922917, -1.674258, -3.200320, -3.241363, -4.135001, -4.290107, -1.520445, -3.714633, -5.395930, 11.046892,
    1.673639, -1.147170,  0.901353, -0.391548, -1.312485, -0.151708, -0.220375, -0.438748, -1.322366, -3.522266, -3.663923, -0.305170, -2.221304, -3.553533, -1.213470,  5.826527,
   -0.068042, -1.683495, -0.069138, -1.576054, -1.299983, -1.012997, -1.294878, -2.363065, -2.528844, -1.076382, -1.796229, -1.004336, -0.999449, -3.161436, -1.612919,  2.071710,  6.817956,
   -3.790328, -4.019108, -5.543911, -6.321502, -3.456164, -2.919725, -4.253197, -3.737232, -3.513238, -3.870811, -2.447829, -4.434676, -2.137255,  1.376341, -5.481260, -4.127804, -3.643382, 15.756041,
   -2.646022, -2.540799, -3.122641, -4.597428, -3.610671, -2.131601, -3.030688, -4.559647,  2.538948, -1.997058, -1.593097, -2.730047, -1.492308,  4.408690, -4.379667, -2.528713, -2.408996,  3.231335,  9.892544,
   -0.284140, -3.753871, -4.314525, -4.713963, -1.211518, -3.297575, -3.663425, -4.708118, -4.676220,  3.820569,  1.182672, -3.393535,  1.030861, -1.273542, -3.523054, -2.469318, -0.083276, -4.251392, -1.811267,  5.653391};

	double tmpmtx80[] = {
    7,
   -3,      9,
   -3,     -1,      9,
   -3,     -3,      2,     10,
   -1,     -6,     -5,     -7,     13,
   -2,      1,      0,     -1,     -5,      9,
   -2,     -1,     -1,      2,     -7,      3,      8,
    0,     -4,     -1,     -3,     -6,     -4,     -4,      9,
   -3,      0,      1,     -2,     -7,      1,      0,     -4,     12,
   -3,     -5,     -6,     -7,     -2,     -5,     -6,     -7,     -6,      7,
   -3,     -4,     -6,     -7,     -3,     -4,     -6,     -7,     -5,      2,      6,
   -1,      3,      0,     -2,     -6,      2,      1,     -3,     -1,     -5,     -4,      8,
   -2,     -3,     -4,     -6,     -3,     -1,     -4,     -5,     -4,      2,      3,     -3,      9,
   -4,     -5,     -6,     -6,     -4,     -5,     -6,     -6,     -2,     -1,      0,     -5,      0,     10,
   -1,     -3,     -4,     -3,     -6,     -3,     -2,     -5,     -4,     -5,     -5,     -2,     -4,     -6,     12,
    2,     -2,      1,     -1,     -2,     -1,     -1,     -1,     -2,     -4,     -4,     -1,     -3,     -4,     -2,      7,
    0,     -2,      0,     -2,     -2,     -1,     -2,     -3,     -3,     -2,     -3,     -1,     -1,     -4,     -3,      2,      8,
   -5,     -5,     -7,     -8,     -5,     -4,     -6,     -6,     -4,     -5,     -4,     -6,     -3,      0,     -7,     -6,     -5,     16,
   -4,     -4,     -4,     -6,     -5,     -3,     -5,     -6,      3,     -3,     -2,     -4,     -3,      4,     -6,     -3,     -3,      3,     11,
   -1,     -4,     -5,     -6,     -2,     -4,     -4,     -6,     -5,      4,      1,     -4,      1,     -2,     -4,     -3,      0,     -5,     -3,      7};

	double tmpmtx90[] = {
    5,
   -2,  6,
   -2, -1,  7,
   -3, -3,  1,  7,
   -1, -5, -4, -5,  9,
   -1,  1,  0, -1, -4,  7,
   -1, -1, -1,  1, -6,  2,  6,
    0, -3, -1, -2, -4, -3, -3,  6,
   -2,  0,  0, -2, -5,  1, -1, -3,  8,
   -2, -4, -4, -5, -2, -4, -4, -5, -4,  5,
   -2, -3, -4, -5, -2, -3, -4, -5, -4,  1,  5,
   -1,  2,  0, -1, -4,  1,  0, -2, -1, -4, -3,  6,
   -2, -2, -3, -4, -2,  0, -3, -4, -3,  1,  2, -2,  7,
   -3, -4, -4, -5, -3, -4, -5, -5, -2, -1,  0, -4, -1,  7,
   -1, -3, -3, -3, -4, -2, -2, -3, -3, -4, -4, -2, -3, -4,  8,
    1, -1,  0, -1, -2, -1, -1, -1, -2, -3, -3, -1, -2, -3, -2,  5,
    0, -2,  0, -2, -2, -1, -1, -3, -2, -1, -2, -1, -1, -3, -2,  1,  6,
   -4, -4, -5, -6, -4, -3, -5, -4, -3, -4, -3, -5, -2,  0, -5, -4, -4, 11,
   -3, -3, -3, -4, -4, -3, -4, -5,  1, -2, -2, -3, -2,  3, -4, -3, -2,  2,  8,
   -1, -3, -4, -5, -2, -3, -3, -5, -4,  3,  0, -3,  0, -2, -3, -2, -1, -3, -3,  5};

	double tmpmtx100[] = {
    8,
   -3,10,
   -4,-2,11,
   -5,-5, 1,10,
   -2,-8,-5,-8,14,
   -2, 0,-1,-2,-7,11,
   -3,-2,-2, 2,-9, 2,10,
   -1,-6,-2,-4,-7,-5,-6, 9,
   -4,-1, 0,-3,-8, 1,-2,-6,13,
   -4,-7,-7,-8,-3,-6,-7,-9,-7, 8,
   -4,-6,-7,-8,-5,-5,-7,-8,-6, 2, 8,
   -2, 3,-1,-3,-8, 2, 0,-5,-3,-6,-6,10,
   -3,-4,-5,-8,-4,-2,-5,-7,-5, 1, 3,-4,12,
   -5,-6,-7,-8,-4,-6,-8,-8,-4,-2, 0,-6,-1,11,
   -2,-5,-5,-5,-8,-4,-4,-6,-5,-7,-7,-3,-5,-7,12,
    1,-3, 0,-2,-3,-2,-2,-2,-3,-5,-6,-2,-4,-5,-3, 9,
   -1,-3,-1,-4,-3,-3,-3,-5,-4,-3,-4,-3,-2,-5,-4, 2, 9,
   -6,-7,-8,-10,-7,-5,-8,-7,-5,-6,-5,-8,-4, 0,-8,-7,-7,17,
   -5,-5,-5,-7,-6,-4,-7,-8, 1,-4,-4,-5,-5, 4,-7,-5,-5, 2,12,
   -2,-6,-7,-8,-3,-5,-5,-8,-7, 4, 0,-5, 0,-3,-6,-4,-1,-5,-5, 8};

	double tmpmtx0[] = {
    2.4,
   -0.6,    4.7,
   -0.3,    0.3,    3.8,
   -0.3,   -0.3,    2.2,    4.7,
    0.5,   -2.2,   -1.8,   -3.2,   11.5,
   -0.2,    1.5,    0.7,    0.9,   -2.4,    2.7,
    0.0,    0.4,    0.9,    2.7,   -3.0,    1.7,    3.6,
    0.5,   -1.0,    0.4,    0.1,   -2.0,   -1.0,   -0.8,    6.6,
   -0.8,    0.6,    1.2,    0.4,   -1.3,    1.2,    0.4,   -1.4,    6.0,
   -0.8,   -2.4,   -2.8,   -3.8,   -1.1,   -1.9,   -2.7,   -4.5,   -2.2,    4.0,
   -1.2,   -2.2,   -3.0,   -4.0,   -1.5,   -1.6,   -2.8,   -4.4,   -1.9,    2.8,    4.0,
   -0.4,    2.7,    0.8,    0.5,   -2.8,    1.5,    1.2,   -1.1,    0.6,   -2.1,   -2.1,    3.2,
   -0.7,   -1.7,   -2.2,   -3.0,   -0.9,   -1.0,   -2.0,   -3.5,   -1.3,    2.5,    2.8,   -1.4,    4.3,
   -2.3,   -3.2,   -3.1,   -4.5,   -0.8,   -2.6,   -3.9,   -5.2,   -0.1,    1.0,    2.0,   -3.3,    1.6,    7.0,
    0.3,   -0.9,   -0.9,   -0.7,   -3.1,   -0.2,   -0.5,   -1.6,   -1.1,   -2.6,   -2.3,   -0.6,   -2.4,   -3.8,    7.6,
    1.1,   -0.2,    0.9,    0.5,    0.1,    0.2,    0.2,    0.4,   -0.2,   -1.8,   -2.1,    0.1,   -1.4,   -2.8,    0.4,    2.2,
    0.6,   -0.2,    0.5,    0.0,   -0.5,    0.0,   -0.1,   -1.1,   -0.3,   -0.6,   -1.3,    0.1,   -0.6,   -2.2,    0.1,    1.5,    2.5,
   -3.6,   -1.6,   -3.6,   -5.2,   -1.0,   -2.7,   -4.3,   -4.0,   -0.8,   -1.8,   -0.7,   -3.5,   -1.0,    3.6,   -5.0,   -3.3,   -3.5,   14.2,
   -2.2,   -1.8,   -1.4,   -2.8,   -0.5,   -1.7,   -2.7,   -4.0,    2.2,   -0.7,    0.0,   -2.1,   -0.2,    5.1,   -3.1,   -1.9,   -1.9,    4.1,    7.8,
    0.1,   -2.0,   -2.2,   -2.9,    0.0,   -1.5,   -1.9,   -3.3,   -2.0,    3.1,    1.8,   -1.7,    1.6,    0.1,   -1.8,   -1.0,    0.0,   -2.6,   -1.1,    3.4};
    // clang-format on

    int     count;
    double* tmpmtx;

    switch (n) {
        case 30: tmpmtx = tmpmtx30; break;
        case 45: tmpmtx = tmpmtx45; break;
        case 50: tmpmtx = tmpmtx50; break;
        case 62: tmpmtx = tmpmtx62; break;
        case 80: tmpmtx = tmpmtx80; break;
        case 90: tmpmtx = tmpmtx90; break;
        case 100: tmpmtx = tmpmtx100; break;
        case 0: tmpmtx = tmpmtx0; break;

        // TODO(sen) Error?
        default: aln_assert(!"invalid n"); break;
    }

    count = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j <= i; j++) {
            matrix[i][j] = matrix[j][i] = (double)tmpmtx[count++];
        }
    }

    for (int i = 0; i < 20; i++) {
        freq[i] = freqd[i];
    }

    for (int i = 0; i < 26; i++)
        amino[i] = locaminod[i];
    for (int i = 0; i < 26; i++)
        amino_grp[(int)amino[i]] = locgrpd[i];
}

static int
shishagonyuu(double in) {
    int out;
    if (in > 0.0)
        out = ((int)(in + 0.5));
    else if (in == 0.0)
        out = (0);
    else if (in < 0.0)
        out = ((int)(in - 0.5));
    else
        out = 0;
    return (out);
}

void
constants(aln_Opts opts, Context* ctx) {
    {
        ctx->n_dis = AllocateIntMtx(ctx->nalphabets, ctx->nalphabets);
        ctx->n_disLN = AllocateDoubleMtx(ctx->nalphabets, ctx->nalphabets);
        double** n_distmp = AllocateDoubleMtx(20, 20);
        double*  datafreq = AllocateDoubleVec(20);
        double*  freq = AllocateDoubleVec(20);

        aln_assert(opts.ppenalty != NOTSPECIFIED);
        aln_assert(opts.ppenalty_dist != NOTSPECIFIED);
        aln_assert(opts.poffset != NOTSPECIFIED);
        aln_assert(opts.ppenalty_ex != NOTSPECIFIED);

        if (ctx->ppenalty_OP == NOTSPECIFIED)
            ctx->ppenalty_OP = DEFAULTGOP_B;
        if (ctx->ppenalty_EX == NOTSPECIFIED)
            ctx->ppenalty_EX = DEFAULTGEP_B;
        if (ctx->pamN == NOTSPECIFIED)
            ctx->pamN = 0;
        if (ctx->kimuraR == NOTSPECIFIED)
            ctx->kimuraR = 1;
        ctx->penalty = (int)(0.6 * opts.ppenalty + 0.5);
        ctx->penalty_dist = (int)(0.6 * opts.ppenalty_dist + 0.5);
        ctx->penalty_shift = (int)(opts.penalty_shift_factor * ctx->penalty);
        ctx->penalty_OP = (int)(0.6 * ctx->ppenalty_OP + 0.5);
        ctx->penalty_ex = (int)(0.6 * opts.ppenalty_ex + 0.5);
        ctx->penalty_EX = (int)(0.6 * ctx->ppenalty_EX + 0.5);
        ctx->offset = (int)(0.6 * opts.poffset + 0.5);
        ctx->offsetFFT = (int)(0.6 * (-0) + 0.5);
        ctx->offsetLN = (int)(0.6 * 100 + 0.5);
        ctx->penaltyLN = (int)(0.6 * -2000 + 0.5);
        ctx->penalty_exLN = (int)(0.6 * -100 + 0.5);

        BLOSUMmtx(opts.nblosum, n_distmp, freq, ctx->amino, ctx->amino_grp);

        for (int i = 0; i < 0x80; i++)
            ctx->amino_n[i] = -1;
        for (int i = 0; i < 26; i++)
            ctx->amino_n[(int)ctx->amino[i]] = i;

        double* freq1 = freq;

        double average = 0.0;
        {
            for (int i = 0; i < 20; i++)
                for (int j = 0; j < 20; j++)
                    average += n_distmp[i][j] * freq1[i] * freq1[j];
        }

        {
            for (int i = 0; i < 20; i++)
                for (int j = 0; j < 20; j++)
                    n_distmp[i][j] -= average;
        }

        average = 0.0;
        for (int i = 0; i < 20; i++)
            average += n_distmp[i][i] * freq1[i];

        {
            for (int i = 0; i < 20; i++)
                for (int j = 0; j < 20; j++)
                    n_distmp[i][j] *= 600.0 / average;
        }

        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                n_distmp[i][j] -= ctx->offset;

        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                n_distmp[i][j] = shishagonyuu(n_distmp[i][j]);

        for (int i = 0; i < 26; i++)
            for (int j = 0; j < 26; j++)
                ctx->n_dis[i][j] = 0;
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                ctx->n_dis[i][j] = (int)n_distmp[i][j];

        FreeDoubleMtx(n_distmp);
        FreeDoubleVec(datafreq);
        FreeDoubleVec(freq);
    }

    int charsize = 0x80;
    ctx->amino_dis = AllocateIntMtx(charsize, charsize);

    for (int i = 0; i < charsize; i++)
        ctx->amino_n[i] = -1;

    for (int i = 0; i < ctx->nalphabets; i++)
        ctx->amino_n[(int)ctx->amino[i]] = i;

    for (int i = 0; i < charsize; i++)
        for (int j = 0; j < charsize; j++)
            ctx->amino_dis[i][j] = 0;

    for (int i = 0; i < ctx->nalphabets; i++)
        for (int j = 0; j < ctx->nalphabets; j++)
            ctx->n_disLN[i][j] = 0;

    ctx->n_dis_consweight_multi = AllocateDoubleMtx(ctx->nalphabets, ctx->nalphabets);
    ctx->n_disFFT = AllocateIntMtx(ctx->nalphabets, ctx->nalphabets);
    for (int i = 0; i < ctx->nalphabets; i++)
        for (int j = 0; j < ctx->nalphabets; j++) {
            ctx->amino_dis[(int)ctx->amino[i]][(int)ctx->amino[j]] = ctx->n_dis[i][j];
            ctx->n_dis_consweight_multi[i][j] = (double)ctx->n_dis[i][j] * ctx->consweight_multi;
        }

    {
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                ctx->n_disLN[i][j] = (double)ctx->n_dis[i][j] + ctx->offset - ctx->offsetLN;
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                ctx->n_disFFT[i][j] = ctx->n_dis[i][j] + ctx->offset - ctx->offsetFFT;
    }

    if (ctx->fftThreshold == NOTSPECIFIED) {
        ctx->fftThreshold = FFT_THRESHOLD;
    }
    if (ctx->fftWinSize == NOTSPECIFIED) {
        ctx->fftWinSize = FFT_WINSIZE_P;
    }
}
