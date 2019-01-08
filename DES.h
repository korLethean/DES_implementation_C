#ifndef DES_H_
#define DES_H_

#if defined(_MSC_VER) && _MSC_VER < 1600
/* Under Visual Studio 2010 */
typedef unsigned int des_drr;
#else
/* C99 Compatible */
#include <stdint.h>
typedef uint32_t des_err;
#endif

#define DES_SUCCESS			0x00
#define DES_BLK_LEN_ERR 	0x01
#define DES_TABLE_LEN_ERR	0x02


/*** In the below permutation tables, values of contents are 1 less than
*** the given values in FIPS 46-3.
*** This is because the logical count starts from 1 as described in FIPS 46-3.
*** However, when it comes to the implementation of the given document,
*** the index of an array starts from 0, not from 1.
***/

const int InitialPermutationTable[64] =	{57, 49, 41, 33, 25, 17, 9, 1,
										59, 51, 43, 35, 27, 19, 11, 3,
										61, 53, 45, 37, 29, 21, 13, 5,
										63, 55, 47, 39, 31, 23, 15, 7,
										56, 48, 40, 32, 24, 16, 8, 0,
										58, 50, 42, 34, 26, 18, 10, 2,
										60, 52, 44, 36, 28, 20, 12, 4,
										62, 54, 46, 38, 30, 22, 14, 6};
const int InitialPermutationTableSize = 64;

const int FinalPermutationTable[64] =	{39, 7, 47, 15, 55, 23, 63, 31,
										38, 6, 46, 14, 54, 22, 62, 30,
										37, 5, 45, 13, 53, 21, 61, 29,
										36, 4, 44, 12, 52, 20, 60, 28,
										35, 3, 43, 11, 51, 19, 59, 27,
										34, 2, 42, 10, 50, 18, 58, 26,
										33, 1, 41, 9, 49, 17, 57, 25,
										32, 0, 40, 8, 48, 16, 56, 24};
const int FinalPermutationTableSize = 64;

const int ExpansionPermutationTable[48] = 	{31, 0, 1, 2, 3, 4,
											3, 4, 5, 6, 7, 8,
											7, 8, 9, 10, 11, 12,
											11, 12, 13, 14, 15, 16,
											15, 16, 17, 18, 19, 20,
											19, 20, 21, 22, 23, 24,
											23, 24, 25, 26, 27, 28,
											27, 28, 29, 30, 31, 0};
const int ExpansionPermutationTableSize = 48;

const int StraightPermutationTable[32] =	{15, 6, 19, 20,
											28, 11, 27, 16,
											0, 14, 22, 25,
											4, 17, 30, 9,
											1, 7, 23, 13,
											31, 26, 2, 8,
											18, 12, 29, 5,
											21, 10, 3, 24};
const int StraightPermutationTableSize = 32;

const int ParityDropTable[64] =	{56, 48, 40, 32, 24, 16, 8,
								0, 57, 49, 41, 33, 25, 17,
								9, 1, 58, 50, 42, 34, 26,
								18, 10, 2, 59, 51, 43, 35,
								62, 54, 46, 38, 30, 22, 14,
								6, 61, 53, 45, 37, 29, 21,
								13, 5, 60, 52, 44, 36, 28,
								20, 12, 4, 27, 19, 11, 3};
const int ParityDropTableSize = 64;

const int KeyCompressionTable[48] =	{13, 16, 10, 23, 0, 4,
									2, 27, 14, 5, 20, 9,
									22, 18, 11, 3, 25, 7,
									15, 6, 26, 19, 12, 1,
									40, 51, 30, 36, 46, 54,
									29, 39, 50, 44, 32, 47,
									43, 48, 38, 55, 33, 52,
									45, 41, 49, 35, 28, 31};
const int KeyCompressionTableSize = 48;

// In the below substitution tables, values of contents are exactly the same as the given values in FIPS 46-3.
const int S_BOX_1[4][16] =	{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},	// Row 0
							{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},		// Row 1
							{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},		// Row 2
							{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};	// Row 3

const int S_BOX_2[4][16] =	{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},	// Row 0
							{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},		// Row 1
							{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},		// Row 2
							{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}};	// Row 3

const int S_BOX_3[4][16] =	{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},	// Row 0
							{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},		// Row 1
							{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},		// Row 2
							{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}};	// Row 3

const int S_BOX_4[4][16] =	{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},	// Row 0
							{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},		// Row 1
							{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},		// Row 2
							{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}};	// Row 3

const int S_BOX_5[4][16] =	{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},	// Row 0
							{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},		// Row 1
							{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},		// Row 2
							{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}};	// Row 3

const int S_BOX_6[4][16] =	{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},	// Row 0
							{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},		// Row 1
							{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},		// Row 2
							{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}};	// Row 3

const int S_BOX_7[4][16] =	{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},	// Row 0
							{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},		// Row 1
							{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},		// Row 2
							{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}};	// Row 3

const int S_BOX_8[4][16] =	{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},	// Row 0
							{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},		// Row 1
							{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},		// Row 2
							{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};	// Row 3

// int *const SubstituteTables[8] = {S_BOX_1, S_BOX_2, S_BOX_3, S_BOX_4, S_BOX_5, S_BOX_6, S_BOX_7, S_BOX_8};

// The below ShiftTable is used to generate round keys
const int ShiftTable[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};


des_err permute(const int in_block_length, const int out_block_length, const int table_size, int *in_block, int *out_block, int *permute_table);

des_err split(const int in_block_length, const int out_block_length, int *in_block, int *left_block, int *right_block);

des_err combine(const int in_block_length, const int out_block_length, int *in_block, int *left_block, int *right_block, int *out_block);

des_err copy(const int block_length, int *in_block, int *out_block);

des_err substitute(const int block_length, int *in_block1, int *in_block2, int *out_block);

des_err function(const int round_key, int *in_block, int *out_block);

des_err mixer(const int round_key, int *left_block, int *right_block);

des_err swapper(int *left_block, int *right_block);

des_err cipher(const int plain_block, int const *round_key[16], int *cipher_block);

des_err key_generator(const int key_size, int *key_with_parities, int *round_keys);

des_err shift_left(const int num_of_shifts, int *block);

int H2B(char value);

char B2H(int value);

void hex_string_to_binary_array(char *string, int *binary);

void binary_to_hex_string_array(int *binary, char *string);


#endif /* DES_H_ */
