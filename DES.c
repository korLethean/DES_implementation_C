#include "DES.h"

const int InitialPermutationTable[InitialPermutationTableSize] =
			{57, 49, 41, 33, 25, 17, 9, 1,
			59, 51, 43, 35, 27, 19, 11, 3,
			61, 53, 45, 37, 29, 21, 13, 5,
			63, 55, 47, 39, 31, 23, 15, 7,
			56, 48, 40, 32, 24, 16, 8, 0,
			58, 50, 42, 34, 26, 18, 10, 2,
			60, 52, 44, 36, 28, 20, 12, 4,
			62, 54, 46, 38, 30, 22, 14, 6};

const int FinalPermutationTable[FinalPermutationTableSize] =
			{39, 7, 47, 15, 55, 23, 63, 31,
			38, 6, 46, 14, 54, 22, 62, 30,
			37, 5, 45, 13, 53, 21, 61, 29,
			36, 4, 44, 12, 52, 20, 60, 28,
			35, 3, 43, 11, 51, 19, 59, 27,
			34, 2, 42, 10, 50, 18, 58, 26,
			33, 1, 41, 9, 49, 17, 57, 25,
			32, 0, 40, 8, 48, 16, 56, 24};

const int ExpansionPermutationTable[ExpansionPermutationTableSize] =
			{31, 0, 1, 2, 3, 4,
			3, 4, 5, 6, 7, 8,
			7, 8, 9, 10, 11, 12,
			11, 12, 13, 14, 15, 16,
			15, 16, 17, 18, 19, 20,
			19, 20, 21, 22, 23, 24,
			23, 24, 25, 26, 27, 28,
			27, 28, 29, 30, 31, 0};

const int StraightPermutationTable[StraightPermutationTableSize] =
			{15, 6, 19, 20,
			28, 11, 27, 16,
			0, 14, 22, 25,
			4, 17, 30, 9,
			1, 7, 23, 13,
			31, 26, 2, 8,
			18, 12, 29, 5,
			21, 10, 3, 24};

const int ParityDropTable[ParityDropTableSize] =
			{56, 48, 40, 32, 24, 16, 8,
			0, 57, 49, 41, 33, 25, 17,
			9, 1, 58, 50, 42, 34, 26,
			18, 10, 2, 59, 51, 43, 35,
			62, 54, 46, 38, 30, 22, 14,
			6, 61, 53, 45, 37, 29, 21,
			13, 5, 60, 52, 44, 36, 28,
			20, 12, 4, 27, 19, 11, 3};

const int KeyCompressionTable[KeyCompressionTableSize] =
			{13, 16, 10, 23, 0, 4,
			2, 27, 14, 5, 20, 9,
			22, 18, 11, 3, 25, 7,
			15, 6, 26, 19, 12, 1,
			40, 51, 30, 36, 46, 54,
			29, 39, 50, 44, 32, 47,
			43, 48, 38, 55, 33, 52,
			45, 41, 49, 35, 28, 31};

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

const int ShiftTable[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

des_err shift_left(const int num_of_shifts, const int block_size, int *block)
{
	int half_size = block_size / 2;

	if(half_size != 28)
		return DES_BLK_LEN_ERR;

	for(int i = 0 ; i < num_of_shifts ; i++)
	{
		int temp = block[0];
		for(int j = 1 ; j < half_size ; j++)
			block[j - 1] = block[j];
		block[half_size - 1] = temp;
	}

	return DES_SUCCESS;
}

des_err combine(const int in_block_length, const int out_block_length, int *left_block, int *right_block, int *out_block)
{
	if(in_block_length * 2 != out_block_length)
		return DES_BLK_LEN_ERR;

	for(int i = 0 ; i < in_block_length ; i++)
	{
		out_block[i] = left_block[i];
		out_block[i + in_block_length] = right_block[i];
	}

	return DES_SUCCESS;
}

des_err split(const int in_block_length, const int out_block_length, int *in_block, int *left_block, int *right_block)
{
	const int half_length = in_block_length / 2;

	if(out_block_length != half_length)
		return DES_BLK_LEN_ERR;

	for(int i = 0 ; i < half_length ; i++)
		left_block[i] = in_block[i];

	for(int i = 0 ; i < half_length ; i++)
		right_block[i] = in_block[i + half_length];

	return DES_SUCCESS;
}

des_err swapper(const int block_size, int *left_block, int *right_block)
{
	if(block_size != 32)
		return DES_BLK_LEN_ERR;

	int temp[block_size];

	for(int i = 0 ; i < block_size ; i++)
	{
		temp[i] = right_block[i];
		right_block[i] = left_block[i];
		left_block[i] = temp[i];
	}

	return DES_SUCCESS;
}

des_err xor(const int block_length, int *in_block_one, int *in_block_two, int *out_block)
{
	if(!(block_length == 48 || block_length == 32))
		return DES_BLK_LEN_ERR;

	for(int i = 0 ; i < block_length ; i++)
		out_block[i] = in_block_one[i] ^ in_block_two[i];

	return DES_SUCCESS;
}

des_err substitute(const int block_length, int *in_block, int *out_block)
{
	if(block_length != 48)
		return DES_BLK_LEN_ERR;

	int row;
	int column;
	char row_c[3];
	row_c[3] = '\0';
	char column_c[5];
	column_c[5] = '\0';

	int value;
	int const (*s_table)[16];

	for(int i = 0 ; i < 8 ; i++)
	{
		sprintf(row_c, "%d%d", in_block[6 * i], in_block[6 * i + 5]);
		row = strtol(row_c, NULL, 2);

		sprintf(column_c, "%d%d%d%d", in_block[6 * i + 1], in_block[6 * i + 2], in_block[6 * i + 3], in_block[6 * i + 4]);
		column = strtol(column_c, NULL, 2);

		if(i == 0)
			s_table = S_BOX_1;
		else if(i == 1)
			s_table = S_BOX_2;
		else if(i == 2)
			s_table = S_BOX_3;
		else if(i == 3)
			s_table = S_BOX_4;
		else if(i == 4)
			s_table = S_BOX_5;
		else if(i == 5)
			s_table = S_BOX_6;
		else if(i == 6)
			s_table = S_BOX_7;
		else if(i == 7)
			s_table = S_BOX_8;

		value = s_table[row][column];

		out_block[i * 4] = value & 8 ? 1 : 0;
		out_block[i * 4 + 1] = value & 4 ? 1 : 0;
		out_block[i * 4 + 2] = value & 2 ? 1 : 0;
		out_block[i * 4 + 3] = value & 1 ? 1 : 0;

		//*** for check row, column, value, block value ***//
		//printf("row: %d\tcolumn: %d\ttable: %d\tvalue: %d \n", row, column, i + 1, value);
		//printf("out_block: %d%d%d%d \n", out_block[i * 4], out_block[i * 4 + 1], out_block[i * 4 + 2], out_block[i * 4 + 3]);
	}

	return DES_SUCCESS;
}

des_err function(const int block_size, const int rk_size, int *round_key, int *in_block, int *out_block)
{
	des_err error_code = DES_SUCCESS;

	if(block_size != 32)
	{
		error_code = DES_BLK_LEN_ERR;
		return error_code;
	}

	if(rk_size != 48)
	{
		error_code = DES_RK_LEN_ERR;
		return error_code;
	}

	const int extended_size = rk_size;

	int temp_extended[extended_size];
	int temp_xor[extended_size];
	int temp_substitute[block_size];

	error_code = permute(block_size, extended_size, ExpansionPermutationTableSize, in_block, temp_extended, ExpansionPermutationTable);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = xor(rk_size, temp_extended, round_key, temp_xor);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = substitute(extended_size, temp_xor, temp_substitute);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = permute(block_size, block_size, StraightPermutationTableSize, temp_substitute, out_block, StraightPermutationTable);
	if(error_code != DES_SUCCESS)
		return error_code;

	return error_code;
}

des_err mixer(const int block_size, const int rk_size, int *round_key, int *left_block, int *right_block)
{
	des_err error_code = DES_SUCCESS;

	if(block_size != 32)
		return DES_BLK_LEN_ERR;

	if(rk_size != 48)
		return DES_RK_LEN_ERR;

	int temp_function[block_size];
	int temp_xor[block_size];

	error_code = function(block_size, rk_size, round_key, right_block, temp_function);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = xor(block_size, left_block, temp_function, temp_xor);
	if(error_code != DES_SUCCESS)
		return error_code;

	for(int i = 0 ; i < block_size ; i++)
		left_block[i] = temp_xor[i];

	return error_code;
}

des_err permute(const int in_block_length, const int out_block_length, const int table_size, int *in_block, int *out_block, const int *permute_table)
{
	switch(in_block_length)
	{
		case 64:
		case 56:
		case 32:
			break;
		default :
			return DES_BLK_LEN_ERR;
	}

	switch(out_block_length)
	{
		case 64:
		case 56:
		case 48:
		case 32:
			break;
		default :
			return DES_BLK_LEN_ERR;
	}

	if(!(table_size == 32 || table_size == 48 || table_size == 64))
		return DES_TABLE_LEN_ERR;

	//*** thanks to @d57_kr about helping find error ***//
	for(int i = 0 ; i < out_block_length ; i++)
		out_block[i] = in_block[permute_table[i]];

	return DES_SUCCESS;
}

des_err cipher(const int plain_size, const int rk_num, const int rk_size, int *plain_block, int (*round_keys)[48], int *cipher_block)
{
	des_err error_code = DES_SUCCESS;

	if(rk_num != 16)
	{
		error_code = DES_RK_QUANTITY_ERR;
		return error_code;
	}

	if(rk_size != 48)
	{
		error_code = DES_RK_LEN_ERR;
		return error_code;
	}

	if(plain_size != 64)
	{
		error_code = DES_BLK_LEN_ERR;
		return error_code;
	}

	int in_block[plain_size];
	int out_block[plain_size];
	int left_block[plain_size / 2];
	int right_block[plain_size / 2];

	error_code = permute(plain_size, plain_size, InitialPermutationTableSize, plain_block, in_block, InitialPermutationTable);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = split(plain_size, plain_size / 2, in_block, left_block, right_block);
	if(error_code != DES_SUCCESS)
		return error_code;

	for(int i = 0 ; i < 16 ; i++)
	{
		error_code = mixer(plain_size / 2, rk_size, round_keys[i], left_block, right_block);
		if(error_code != DES_SUCCESS)
			return error_code;

		if(i != 15)
		{
			error_code = swapper(plain_size / 2, left_block, right_block);
			if(error_code != DES_SUCCESS)
				return error_code;
		}
	}

	error_code = combine(plain_size / 2, plain_size, left_block, right_block, out_block);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = permute(plain_size, plain_size, FinalPermutationTableSize, out_block, cipher_block, FinalPermutationTable);

	return error_code;
}

des_err key_generator(const int pk_size, const int rk_size, int *key_with_parities, int (*round_keys)[48])
{
	des_err error_code = DES_SUCCESS;

	if(pk_size != 64 || rk_size != 48)
	{
		error_code = DES_BLK_LEN_ERR;
		return error_code;
	}

	const int prk_size = 56;

	int cipher_key[prk_size];
	int left_key[prk_size / 2];
	int right_key[prk_size / 2];
	int pre_round_key[prk_size];

	error_code = permute(pk_size, prk_size, ParityDropTableSize, key_with_parities, cipher_key, ParityDropTable);
	if(error_code != DES_SUCCESS)
		return error_code;

	error_code = split(prk_size, prk_size / 2, cipher_key, left_key, right_key);
	if(error_code != DES_SUCCESS)
		return error_code;

	/*** for check left, right key ***/
	/*printf("Left Key:\t");
	for(int i = 0 ; i < prk_size / 2 ; i++)
		printf("%d", left_key[i]);
	printf("\nRight Key:\t");
	for(int i = 0 ; i < prk_size / 2 ; i++)
		printf("%d", right_key[i]);
	printf("\n");*/
	/*********************************/

	for(int i = 0 ; i < 16 ; i++)
	{
		error_code = shift_left(ShiftTable[i], prk_size, left_key);
		if(error_code != DES_SUCCESS)
			return error_code;

		error_code = shift_left(ShiftTable[i], prk_size, right_key);
		if(error_code != DES_SUCCESS)
			return error_code;

		error_code = combine(prk_size / 2, prk_size, left_key, right_key, pre_round_key);
		if(error_code != DES_SUCCESS)
			return error_code;

		error_code = permute(prk_size, rk_size, KeyCompressionTableSize, pre_round_key, round_keys[i], KeyCompressionTable);
		if(error_code != DES_SUCCESS)
			return error_code;

		for(int j = 0 ; j < prk_size ; j++)
			pre_round_key[j] = 0;
	}

	return error_code;
}

int H2B(char value)
{
	if(value == 'a' || value == 'A')
		return 10;
	else if(value == 'b' || value == 'B')
		return 11;
	else if(value == 'c' || value == 'C')
		return 12;
	else if(value == 'd' || value == 'D')
		return 13;
	else if(value == 'e' || value == 'e')
		return 14;
	else if(value == 'f' || value == 'F')
		return 15;
	else
		return (int) value - 48;
}

void hex_string_to_binary_array(char *string, int *binary)
{
	int value = 0;
	int len = strlen(string);

	for(int i = 0 ; i < len ; i++)
	{
		value = H2B(string[i]);
		binary[i * 4] = value / 8;
		value %= 8;
		binary[i * 4 + 1] = value / 4;
		value %= 4;
		binary[i * 4 + 2] = value / 2;
		value %= 2;
		binary[i * 4 + 3] = value;
	}
}

void binary_to_hex_string_array(int *binary, int *string)
{
	char temp[5];
	temp[5] = '\0';

	for(int i = 0 ; i < 16 ; i++)
	{
		sprintf(temp, "%d%d%d%d", binary[i * 4], binary[i * 4 + 1], binary[i * 4 + 2], binary[i * 4 + 3]);
		string[i] = strtol(temp, NULL, 2);
	}
}
