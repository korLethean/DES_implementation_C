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

des_err permute(const int in_block_length, const int out_block_length, const int table_size, int *in_block, int *out_block, const int *permute_table)
{
	if(!(in_block_length == 64 || in_block_length == 56 || out_block_length == 56 || out_block_length == 48))
		return DES_BLK_LEN_ERR;

	if(!(table_size == 32 || table_size == 48 || table_size == 64))
		return DES_TABLE_LEN_ERR;

	for(int i = 0 ; i < table_size ; i++)
		out_block[i] = in_block[permute_table[i]];

	return DES_SUCCESS;
}

des_err cipher(const int plain_size, const int rk_num, const int rk_size, int *plain_block, int const *round_key[16], int *cipher_block)
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

	permute(plain_size, plain_size, InitialPermutationTableSize, plain_block, in_block, InitialPermutationTable);

	split(plain_size, plain_size / 2, in_block, left_block, right_block);

	for(int i = 0 ; i < 16 ; i++)
	{
		// mixer
		if(i != 15)
		{
			// swapper
		}
	}

	// combine left right -> out

	// permute out -> cipher with finalpermute


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

int main(void)
{
    printf("Encryption \n\n");
    const int TEXT_KEY_SIZE = 64;
    const int RK_SIZE = 48;
    int round_keys[16][RK_SIZE];
    int plaintext[TEXT_KEY_SIZE];
    int ciphertext[TEXT_KEY_SIZE];
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

    // cipher
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
    // decryption


	return 0;
}
