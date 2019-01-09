#include <stdio.h>
#include <stdlib.h>
#include "DES.h"

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

	for(int i = 0 ; i < table_size ; i++)
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
	printf("Left Key:\t");
	for(int i = 0 ; i < prk_size / 2 ; i++)
		printf("%d", left_key[i]);
	printf("\nRight Key:\t");
	for(int i = 0 ; i < prk_size / 2 ; i++)
		printf("%d", right_key[i]);
	printf("\n");
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

	for(int i = 0 ; i < strlen(string) ; i++)
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

int main(void)
{
    const int TEXT_KEY_SIZE = 64;
    const int RK_NUM = 16;
    const int RK_SIZE = 48;
    int round_keys[RK_NUM][RK_SIZE];
    int plaintext[TEXT_KEY_SIZE];
    int plaintext_c[TEXT_KEY_SIZE / 4];
    int ciphertext[TEXT_KEY_SIZE];
    int ciphertext_c[TEXT_KEY_SIZE / 4];
    int key[TEXT_KEY_SIZE];
    des_err error_code = DES_SUCCESS;

    hex_string_to_binary_array("123456ABCD132536", plaintext);
    hex_string_to_binary_array("AABB09182736CCDD", key);

    /*** for check plaintext, key ***/
    /*printf("Plaintext: ");
    for(int i = 0 ; i < 64 ; i++)
    	printf("%d", plaintext[i]);
    printf("\nKey: ");
    for(int i = 0 ; i < 64 ; i++)
		printf("%d", key[i]);
	printf("\n");*/
    /********************************/

    printf("Encryption \n\n");

    error_code = key_generator(TEXT_KEY_SIZE, RK_SIZE, key, round_keys);
    if(error_code == DES_BLK_LEN_ERR)
    {
    	printf("DES Encryption Error occurred while key generating: please check key size \n");
    	return 0;
    }
    else if(error_code == DES_TABLE_LEN_ERR)
    {
    	printf("DES Encryption Error occurred while key generating: please check table size \n");
    	return 0;
    }

    /*** for check round key ***/
	/*for(int i = 0 ; i < 16 ; i++)
	{
		printf("Round key %d:\t", i + 1);
		for(int j = 0 ; j < 48 ; j++)
			printf("%d", round_keys[i][j]);
		printf("\n");
	}*/
	/***************************/

    error_code = cipher(TEXT_KEY_SIZE, RK_NUM, RK_SIZE, plaintext, round_keys, ciphertext);
    if(error_code == DES_BLK_LEN_ERR)
	{
		printf("DES Encryption Error occurred while cipher: please check key size \n");
		return 0;
	}
	else if(error_code == DES_TABLE_LEN_ERR)
	{
		printf("DES Encryption Error occurred while cipher: please check table size \n");
		return 0;
	}
	else if(error_code == DES_RK_QUANTITY_ERR)
	{
		printf("DES Encryption Error occurred while cipher: please check number of round keys \n");
		return 0;
	}
	else if(error_code == DES_RK_LEN_ERR)
	{
		printf("DES Encryption Error occurred while cipher: please check round keys size \n");
		return 0;
	}

    binary_to_hex_string_array(ciphertext, ciphertext_c);

    /*** for check cipher text ***/
	printf("Cipher text:\t");
    for(int i = 0 ; i < 16 ; i++)
		printf("%X", ciphertext_c[i]);
	printf("\n");
	/*****************************/

    // TODO: decryption

	return 0;
}
