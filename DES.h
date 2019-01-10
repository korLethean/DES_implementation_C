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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DES_SUCCESS			0x00
#define DES_BLK_LEN_ERR 	0x01
#define DES_TABLE_LEN_ERR	0x02
#define DES_RK_LEN_ERR		0x03
#define DES_RK_QUANTITY_ERR	0x04

#define InitialPermutationTableSize	64
#define FinalPermutationTableSize	64
#define ExpansionPermutationTableSize	48
#define	StraightPermutationTableSize	32
#define ParityDropTableSize		64
#define KeyCompressionTableSize	48


/*** In the below permutation tables, values of contents are 1 less than
*** the given values in FIPS 46-3.
*** This is because the logical count starts from 1 as described in FIPS 46-3.
*** However, when it comes to the implementation of the given document,
*** the index of an array starts from 0, not from 1.
***/

const int InitialPermutationTable[InitialPermutationTableSize];

const int FinalPermutationTable[FinalPermutationTableSize];

const int ExpansionPermutationTable[ExpansionPermutationTableSize];

const int StraightPermutationTable[StraightPermutationTableSize];

const int ParityDropTable[ParityDropTableSize];

const int KeyCompressionTable[KeyCompressionTableSize];

// In the below substitution tables, values of contents are exactly the same as the given values in FIPS 46-3.
const int S_BOX_1[4][16];

const int S_BOX_2[4][16];

const int S_BOX_3[4][16];

const int S_BOX_4[4][16];

const int S_BOX_5[4][16];

const int S_BOX_6[4][16];

const int S_BOX_7[4][16];

const int S_BOX_8[4][16];

// The below ShiftTable is used to generate round keys
const int ShiftTable[16];


des_err permute(const int in_block_length, const int out_block_length, const int table_size, int *in_block, int *out_block, const int *permute_table);

des_err split(const int in_block_length, const int out_block_length, int *in_block, int *left_block, int *right_block);

des_err combine(const int in_block_length, const int out_block_length, int *left_block, int *right_block, int *out_block);

des_err xor(const int block_length, int *in_block_one, int *in_block_two, int *out_block);

des_err substitute(const int block_length, int *in_block, int *out_block);

des_err function(const int block_size, const int rk_size, int *round_key, int *in_block, int *out_block);

des_err mixer(const int block_size, const int rk_size, int *round_key, int *left_block, int *right_block);

des_err swapper(const int block_size, int *left_block, int *right_block);

des_err cipher(const int plain_size, const int rk_num, const int rk_size, int *plain_block, int (*round_keys)[48], int *cipher_block);

des_err key_generator(const int key_size, const int rk_size, int *key_with_parities, int (*round_keys)[48]);

des_err shift_left(const int num_of_shifts, const int block_size, int *block);

int H2B(char value);

void hex_string_to_binary_array(char *string, int *binary);

void binary_to_hex_string_array(int *binary, int *string);


#endif /* DES_H_ */
