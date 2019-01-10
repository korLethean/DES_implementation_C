#include "DES.h"

int main(void)
{
    const int TEXT_KEY_SIZE = 64;
    const int RK_NUM = 16;
    const int RK_SIZE = 48;
    int round_keys[RK_NUM][RK_SIZE];
    int reversed_round_keys[RK_NUM][RK_SIZE];
    int plaintext[TEXT_KEY_SIZE];
    int plaintext_c[TEXT_KEY_SIZE / 4];
    int ciphertext[TEXT_KEY_SIZE];
    int ciphertext_c[TEXT_KEY_SIZE / 4];
    int restoredtext[TEXT_KEY_SIZE];
    int restoredtext_c[TEXT_KEY_SIZE / 4];
    int key[TEXT_KEY_SIZE];
    des_err error_code = DES_SUCCESS;

    hex_string_to_binary_array("123456ABCD132536", plaintext);
    hex_string_to_binary_array("AABB09182736CCDD", key);

    /** for check plaintext, key **/
    /*printf("Plaintext: ");
    for(int i = 0 ; i < TEXT_KEY_SIZE ; i++)
    	printf("%d", plaintext[i]);
    printf("\nKey: ");
    for(int i = 0 ; i < TEXT_KEY_SIZE ; i++)
		printf("%d", key[i]);
	printf("\n");*/
    /******************************/

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

    /** for check round key **/
	/*for(int i = 0 ; i < RK_NUM ; i++)
	{
		printf("Round key %d:\t", i + 1);
		for(int j = 0 ; j < RK_SIZE ; j++)
			printf("%d", round_keys[i][j]);
		printf("\n");
	}*/
	/*************************/

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

    printf("Decryption \n\n");

    for(int i = 0 ; i < RK_NUM ; i++)
    {
    	for(int j = 0 ; j < RK_SIZE ; j++)
    		reversed_round_keys[i][j] = round_keys[RK_NUM - 1 - i][j];
    }

    /** for check reversed round key **/
	/*for(int i = 0 ; i < RK_NUM ; i++)
	{
		printf("Round key %d:\t", i + 1);
		for(int j = 0 ; j < RK_SIZE ; j++)
			printf("%d", reversed_round_keys[i][j]);
		printf("\n");
	}*/
	/*************************/

    error_code = cipher(TEXT_KEY_SIZE, RK_NUM, RK_SIZE, ciphertext, reversed_round_keys, restoredtext);
    if(error_code == DES_BLK_LEN_ERR)
	{
		printf("DES Decryption Error occurred while cipher: please check key size \n");
		return 0;
	}
	else if(error_code == DES_TABLE_LEN_ERR)
	{
		printf("DES Decryption Error occurred while cipher: please check table size \n");
		return 0;
	}
	else if(error_code == DES_RK_QUANTITY_ERR)
	{
		printf("DES Decryption Error occurred while cipher: please check number of round keys \n");
		return 0;
	}
	else if(error_code == DES_RK_LEN_ERR)
	{
		printf("DES Decryption Error occurred while cipher: please check round keys size \n");
		return 0;
	}

    binary_to_hex_string_array(plaintext, plaintext_c);
    binary_to_hex_string_array(ciphertext, ciphertext_c);
    binary_to_hex_string_array(restoredtext, restoredtext_c);

	/** for check result **/
	printf("Plain text:\t");
    for(int i = 0 ; i < 16 ; i++)
		printf("%X", plaintext_c[i]);
	printf("\n");

    printf("Cipher text:\t");
    for(int i = 0 ; i < 16 ; i++)
		printf("%X", ciphertext_c[i]);
	printf("\n");

    printf("Restored text:\t");
    for(int i = 0 ; i < 16 ; i++)
		printf("%X", restoredtext_c[i]);
	printf("\n");
	/***************************/

	return 0;
}

