#include <stdio.h>
#include <stdlib.h>
#include "DES.h"

des_err key_generator(const int pk_size, int *key_with_parities, int *round_keys)
{
	if(pk_size != 64)
		return DES_BLK_LEN_ERR;

	const int prk_size = 56;
	const int rk_size = 48;

	int cipher_key[prk_size];
	int left_key[prk_size / 2];
	int right_key[prk_size / 2];
	int pre_round_key[prk_size];

	// permute

	// split

	// for
		// shiftleft
		// shiftleft
		// combine
		// permute

	return DES_SUCCESS;
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
    int round_keys[16];
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

    error_code = key_generator(TEXT_KEY_SIZE, key, round_keys);
    if(error_code != DES_SUCCESS)
    {
    	printf("DES Error occurred while key generating: please check key size \n");
    	return 0;
    }

	return 0;
}
