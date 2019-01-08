#include <stdio.h>
#include <stdlib.h>
#include "DES.h"

des_err split(const int in_block_length, const int out_block_length, int *in_block, int *left_block, int *right_block)
{
	const int half_length = in_block_length / 2;

	if(out_block_length != half_length)
		return DES_BLK_LEN_ERR;

	for(int i = 0 ; i < half_length / 2 ; i++)
		left_block[i] = in_block[i];

	for(int i = 0 ; i < half_length / 2 ; i++)
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

des_err key_generator(const int pk_size, const int rk_size, int *key_with_parities, int (*round_keys)[48])
{
	des_err error_code;

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

	for(int i = 0 ; i < 16 ; i++)
	{
		// shiftleft
		// shiftleft
		// combine
		error_code = permute(prk_size, rk_size, KeyCompressionTableSize, pre_round_key, round_keys[i], KeyCompressionTable);
		if(error_code != DES_SUCCESS)
			return error_code;
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
    	printf("DES Error occurred while key generating: please check key size \n");
    	return 0;
    }
    else if(error_code == DES_TABLE_LEN_ERR)
    {
    	printf("DES Error occurred while key generating: please check table size \n");
    	return 0;
    }


	return 0;
}
