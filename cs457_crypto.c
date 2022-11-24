/* Name: Georgios Mpirmpilis
 * File: cs457_crypto.c
 * cs457 - Assignment 1 - Simple Cipher Library
 */

#include "cs457_crypto.h"



uint8_t* otp_encrypt(uint8_t *plaintext, uint8_t *key) {
	int i;

	for (i = 0; i < message_length; i++) {
		encrypted[i] = plaintext[i] ^ key[i];
	}

	encrypted[i] = '\0';
	return encrypted;
}





uint8_t * otp_decrypt(uint8_t *ciphertext, uint8_t *key) {
	int i;

	for (i = 0; i < message_length; i++) {
		decrypted[i] = ciphertext[i] ^ key[i];
	}

	decrypted[i] = '\0';
	return decrypted;
}






uint8_t* caesar_encrypt(uint8_t *plaintext, uint16_t N) {
	// 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
	int j, k = 0;
	uint8_t ch;
	int i;

	encrypted = (uint8_t *)strdup((const char *)plaintext);
	for (i = 0; i < message_length; i++) {
		if ((plaintext[i] >='A' && plaintext[i] <='Z') || (plaintext[i] >='a' && plaintext[i] <='z') ||
			(plaintext[i] >='0' && plaintext[i] <='9')) {
			encrypted[k] = plaintext[i];
			k++;
		}
	}

	encrypted[k] = '\0';

	for (i = 0; i < (int)strlen((const char *)encrypted); i++) {
		ch = encrypted[i];
		for (j = 0; j < N; j++) {
			switch (ch) {
				case 'Z':
					ch = 'a';
					break;
				case 'z':
					ch = '0';
					break;
				case '9':
					ch = 'A';
					break;
				default:
					ch++;
			}
		}
		encrypted[i] = ch;
	}
	encrypted[i] = '\0';
	return encrypted;
}



uint8_t* caesar_decrypt(uint8_t *ciphertext, uint16_t N) {
	// 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
	int i, j;
	uint8_t ch;

	for (i = 0; i < (int)strlen((const char*)ciphertext); i++) {
		ch = ciphertext[i];
		for (j = 0; j < N; j++) {
			switch (ch) {
				case 'a':
					ch = 'Z';
					break;
				case '0':
					ch = 'z';
					break;
				case 'A':
					ch = '9';
					break;
				default:
					ch--;
			}
		}
		decrypted[i] = ch;
	}
	decrypted[i] = '\0';
	return decrypted;
}




uint8_t* affine_encrypt(uint8_t *plaintext) {
	int i, res, k = 0;
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	uint8_t *s = (uint8_t*)strdup((const char*)plaintext);
	


	for (i = 0; i < message_length; i++) {
		if ( (plaintext[i] >='A' && plaintext[i] <='Z') || (plaintext[i] >='a' && plaintext[i] <='z')) {
			s[k] = toupper(plaintext[i]);
			k++;
		}
	}

	s[k] = '\0';


	for (i = 0; i < k; i++) {
		res = ((11*s[i]-'A') + 19) % 26;  /* subtract from A because it is the first letter so from 1...25 (as in array) */
		s[i] = alphabet[res];
	}

	return s;
}




uint8_t* affine_decrypt(uint8_t *ciphertext) {
	unsigned int i, k;
	uint8_t *s = (uint8_t*)strdup((const char*)ciphertext);


	for (i = 0; i < strlen((const char*)ciphertext); i++) {
		
		/* find the multiplicative inverse of a */
		for (k = 0; k < 26; k++) { 
          	if (((11 * k) % 26) == 1) {  
				break;	/* k values satisfies the above, so this is the inverse (break)*/
        	}
    	}

		s[i] = (((k * ((ciphertext[i]+'A' - 19)) % 26)) + 'A');
	}
	return s;
}










/* This function correctly setups 5x5 grid to be ready for encryption/decryption */
void playfair_keymatrix(unsigned char *key) {
	int i = 0, j = 0, hash_index; //k = 0;
	int hash_table[26] = {0};   /* it will keep track of the letters shown in the key */
	

	/* insert the letters from modified_key only once in the grid */
	while (*key) {
		if ((key[i] >='A' && key[i] <='Z')) {
			hash_index = (*key-'A') % 26;
			if (hash_index == 9)  /* if it happens to be J, make it I (8) */
				hash_index = 8;

			if (!hash_table[hash_index]) {  /* not previously inserted, insert it and mark it */
				hash_table[hash_index] = 1;
				insert_from_key(*key);
			}
		}
		key++;
	}


	/* insert the rest of the alphabet (excluding the previously inserted from key) */
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (!playfair_grid[i][j]) {
				playfair_grid[i][j] = insert_from_hash(hash_table);
			}
		}
	}


	printf("\n");
	/* print the grid */
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (playfair_grid[i][j]) {
					printf("%c ", playfair_grid[i][j]);
			} else {
				printf("? ");  /* for debugging if something goes wrong... */
			}
		}
		printf("\n");
	}
	printf("\n");


	/* if someone wants to see what text will be encrypted
	printf("-Modified text:%s\n", key); */
}





/* inserts the given char from KEY in the grid */
void insert_from_key(unsigned char ch) {
	int i, j;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (!playfair_grid[i][j]) {
				if (ch == 'J')
					ch = 'I';

				playfair_grid[i][j] = ch;
				return;
			}
		}
	}
}



/* inserts the given char from HASH_TABLE in the grid */
unsigned char insert_from_hash(int hash_table[26]) {
	int i;

	for (i = 0; i < 26; i++) {
		if (!hash_table[i]) {
			hash_table[i] = 1;  /* mark as inserted */
			if (i == 9)   /* if it's J, mark it as 1 and continue (I covers it) */
				continue;
			break;
		}
	}

	return (i + 'A');
}


unsigned char *playfair_encrypt(unsigned char *plaintext, uint8_t key[]) {
	/* row and col used to catch the row-col of the first letter to be matched with the 2nd one */
	int char_a_row, char_a_col, char_b_row, char_b_col;
	unsigned int i;
	
	unsigned char *modified_key = modified_plaintext(plaintext);
	playfair_keymatrix(key);  /* setup the grid with the new modified text */
	

	size_t k = strlen((const char *)modified_key);
	if (k % 2 == 1)
		strcat((char *)modified_key, "X");


	for (i = 0; i < k; i += 2) {
		if (modified_key[i] == modified_key[i+1]) {    /* found the same letter, replace the second with X */
			modified_key[i+1] = 'X';
		}
		char_row_col(modified_key[i],   &char_a_row, &char_a_col);
		char_row_col(modified_key[i+1], &char_b_row, &char_b_col);

		if (char_a_row == char_b_row) {  /* same row --> shift 1 to the right */
			char_a_col = (char_a_col == 4) ? 0 : char_a_col + 1;
			char_b_col = (char_b_col == 4) ? 0 : char_b_col + 1;

			modified_key[i]   = playfair_grid[char_a_row][char_a_col];
			modified_key[i+1] = playfair_grid[char_b_row][char_b_col];

		} else if (char_a_col == char_b_col) {  /* same col --> shift 1 below */
			char_a_row = (char_a_row == 4) ? 0 : char_a_row + 1;
			char_b_row = (char_b_row == 4) ? 0 : char_b_row + 1;

			modified_key[i]   = playfair_grid[char_a_row][char_a_col];
			modified_key[i+1] = playfair_grid[char_b_row][char_b_col];

		} else {   /* they form a box */
			modified_key[i]   = playfair_grid[char_a_row][char_b_col];
			modified_key[i+1] = playfair_grid[char_b_row][char_a_col];
		}
	}

	return modified_key;	
}




unsigned char *playfair_decrypt(unsigned char *ciphertext) {
	int char_a_row, char_a_col, char_b_row, char_b_col;
	size_t i;

	unsigned char *original_text = (unsigned char *)strdup((const char *)ciphertext);

	for (i = 0; i < strlen((const char *)ciphertext); i += 2) {
		char_row_col(ciphertext[i],   &char_a_row, &char_a_col);
		char_row_col(ciphertext[i+1], &char_b_row, &char_b_col);

		if (char_a_row == char_b_row) {  /* same row --> shift 1 to the right */
			char_a_col = (char_a_col == 0) ? 4 : char_a_col - 1;
			char_b_col = (char_b_col == 0) ? 4 : char_b_col - 1;

			original_text[i]   = playfair_grid[char_a_row][char_a_col];
			original_text[i+1] = playfair_grid[char_b_row][char_b_col];

		} else if (char_a_col == char_b_col) {  /* same col --> shift 1 below */
			char_a_row = (char_a_row == 0) ? 4 : char_a_row - 1;
			char_b_row = (char_b_row == 0) ? 4 : char_b_row - 1;

			original_text[i]   = playfair_grid[char_a_row][char_a_col];
			original_text[i+1] = playfair_grid[char_b_row][char_b_col];

		} else {   /* they form a box */
			original_text[i]   = playfair_grid[char_a_row][char_b_col];
			original_text[i+1] = playfair_grid[char_b_row][char_a_col];
		}
	}

	return original_text;
}










/* find the row and the col of the given char in the 5x5 grid */
void char_row_col(unsigned char c, int *row, int *col) {
	int i, j;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (c == playfair_grid[i][j]) {
				*row = i;
				*col = j;
				return;
			}
		}
	}
}








/* keeps only upprcase letters from A-Z and discards every other character */
unsigned char *modified_plaintext(unsigned char *plaintext) {
	unsigned char *text = malloc((message_length+2) * sizeof(unsigned char));
	memcpy(text, plaintext, message_length);

	int i, k=0;

	/* keep uppercase letters only */
	for (i = 0; i < message_length; i++) {
		if ( (plaintext[i] >='A' && plaintext[i] <='Z') ) {
			text[k] = plaintext[i];
			k++;
		}
	}
	text[k] = '\0';

	return text;
}





uint8_t* feistel_encrypt(uint8_t* plaintext, uint8_t* keys[], int *length) {
	int i, j, k;
	uint8_t left[4], right[4], xor_arr[4], tmp_1[4], tmp_2[4];  /* tmp_1 -> for the F, tmp_2 the 2nd source for XOR */


	uint8_t *cpy = pad_string(plaintext);
	*length = (int)strlen((const char *)cpy);


	for (i = 0; i < ROUNDS; i++) {
		keys[i] = random_key(4);   /* 4 bytes bcz block is 4 bytes */
	}


	for (i = 0; i < *length; i += 8) {
		/* hardcode initialize the 4 bytes for encryption */
		left[0]=cpy[i];  left[1]=cpy[i+1];  left[2]=cpy[i+2]; left[3]=cpy[i+3];
		right[0]=cpy[i+4]; right[1]=cpy[i+5]; right[2]=cpy[i+6]; right[3]=cpy[i+7];
		


		for (j = 0; j < ROUNDS; j++) {
			round_func(right, tmp_1, j, keys);


			for (k = 0; k < 4; k++) {
					xor_arr[k] = left[k] ^ tmp_1[k];
			}

			memcpy(tmp_2, right, 4);
			memcpy(right, xor_arr, 4);
			memcpy(left, tmp_2, 4);
		}

		cpy[i]   = right[0];
		cpy[i+1] = right[1];
		cpy[i+2] = right[2];
		cpy[i+3] = right[3];

		cpy[i+4] = left[0];
		cpy[i+5] = left[1];
		cpy[i+6] = left[2];
		cpy[i+7] = left[3];
	}

	return cpy;
}





uint8_t* feistel_decrypt(uint8_t* ciphertext, uint8_t *keys[], int length) {
	int i, j, k;
	uint8_t left[4], right[4], xor_arr[4], tmp_1[4], tmp_2[4];  /* tmp_1 -> for the F, tmp_2 the 2nd source for XOR */

	for (i = 0; i < length; i += 8) {
		/* hardcode initialize the 4 bytes for encryption */
		left[0]=ciphertext[i];  left[1]=ciphertext[i+1];  left[2]=ciphertext[i+2]; left[3]=ciphertext[i+3];
		right[0]=ciphertext[i+4]; right[1]=ciphertext[i+5]; right[2]=ciphertext[i+6]; right[3]=ciphertext[i+7];
		


		for (j = ROUNDS-1; j >= 0; j--) {
			round_func(right, tmp_1, j, keys);


			for (k = 0; k < 4; k++) {
					xor_arr[k] = left[k] ^ tmp_1[k];
			}

			memcpy(tmp_2, right, 4);
			memcpy(right, xor_arr, 4);
			memcpy(left, tmp_2, 4);
		}

		ciphertext[i]   = right[0];
		ciphertext[i+1] = right[1];
		ciphertext[i+2] = right[2];
		ciphertext[i+3] = right[3];

		ciphertext[i+4] = left[0];
		ciphertext[i+5] = left[1];
		ciphertext[i+6] = left[2];
		ciphertext[i+7] = left[3];
	}

	return ciphertext;
}








/* the round function just before the XOR of left-right */
void round_func(uint8_t sub_arr[], uint8_t tmp_1[], int round_number, uint8_t *keys[]) {
	uint8_t *selected_key;
	int i;
	int power = (int)pow(2.0, 32.0);


	switch (round_number) {
		case 0:
			selected_key = keys[0];
			break;
		case 1:
			selected_key = keys[1];
			break;
		case 2:
			selected_key = keys[2];
			break;
		case 3:
			selected_key = keys[3];
			break;
		case 4:
			selected_key = keys[4];
			break;
		case 5:
			selected_key = keys[5];
			break;
		case 6:
			selected_key = keys[6];
			break;
		default:
			selected_key = keys[7];
	}

	for (i = 0; i < 4; i++) {
		tmp_1[i] = (sub_arr[i] * selected_key[i]) % power;
	}	
}















/* pads the string if it's not divisible by 8 Bytes (=64 bits) using the PKCS5 Padding */
uint8_t *pad_string(uint8_t *plaintext) {
	int i;

	int len = (int)strlen((const char *)plaintext);
	/* plaintext cannot be splitted in blocks of 8 Bytes (64 bits) */
	if (len % 8) {
		
		plaintext = realloc(plaintext, (len + (8-(len%8))+1)*sizeof(uint8_t));
		
		for (i = len; i < message_length + (8-(len%8)); i++) {
			plaintext[i] = 0x06;  /* Pad with a non-printable harmless byte */
			
		}
		plaintext[i] = '\0';
	}

	return plaintext;
}
