/* Name: Georgios Mpirmpilis
 * File: test_program.c
 * cs457 - Assignment 1 - Simple Cipher Library
 */



/* This program is used for testing the implemented crypto algorithms */


#include "cs457_crypto.h"


int message_length;
uint8_t *encrypted, *decrypted;
unsigned char playfair_grid[5][5];



/* reads the whole line from the input file */
uint8_t *read_the_string(int argc, char *input_file) {
	FILE *fp;
	int i = 0;
	char c;



	uint8_t *plaintext = malloc(sizeof(uint8_t) * 1);  /* will change later below */

	if (argc == 1 || !(fp = fopen(input_file, "r"))) {
		printf("Error!! Correct execution is : ./cipher input_file\n\n");
		exit(EXIT_FAILURE);
	}

	while ((c = getc(fp)) != EOF) {
    	plaintext = realloc(plaintext, (i+1)*sizeof(uint8_t));		/* realloc for every character read by one extra byte */
    	plaintext[i] = c;
    	i++;
	}
	plaintext = realloc(plaintext, (i+1)*sizeof(uint8_t));
	plaintext[i] = '\0';

	fclose(fp);
	return plaintext;
}





/* read random bytes from /dev/urandom */
uint8_t *random_key(int len) {
	uint8_t *key = malloc(sizeof(uint8_t) * len);
	FILE *fp;
	if (!(fp = fopen("/dev/urandom", "r"))) {
		printf("Error!! Couldn't read random bytes from /dev/urandom...\n\n");
		exit(EXIT_FAILURE);
	}

	fread(key, 1, len, fp);
	fclose(fp);

	return key;
}








int main(int argc, char* argv[]) {
	uint8_t *otp_key;
	int i;
	uint8_t *keys[ROUNDS];
	uint8_t grid_key[2049] = "Hello COMPUTER SCIENCE DEPARTMENT and HY457";
	uint16_t N;

	

	uint8_t *string = read_the_string(argc, argv[1]);
	message_length = strlen((const char*)string);
	encrypted = malloc(sizeof(uint8_t) * (message_length+1));	/* +1 for the null byte */
	decrypted = malloc(sizeof(uint8_t) * (message_length+1));	/* +1 for the null byte */


	printf("\n---------------- CS457 - Simple Cipher Library ----------------\n");



/* ---------------------------------------------------------------------- */
	printf(" \n\n **** One-Time Pad ****\n");
	printf("-Plaintext:%s\n", string);
	otp_key = random_key(message_length);
	encrypted = otp_encrypt(string,otp_key);
	
	printf("-Encrypted:");
	for (i = 0; i < message_length; i++) {
		printf("%02X ", encrypted[i]);  /* printed in hex to avoid unprintable chars */
	}
	//printf("\n-Encrypted%s\n", encrypted);
	printf("\n-Decrypted:%s", otp_decrypt(encrypted,otp_key));
	free(otp_key);
	free(encrypted);


/* ---------------------------------------------------------------------- */
	printf("\n\n\n **** Caesar's cipher ****\n");
	srand(time(NULL));
	N = (rand()%101)+1;
	printf("Rounds are set to: %d\n", N);
#if 0
	printf("Give N:");
	scanf("%hu", &N);
	getchar();  /* to consume the newline scanf leaves in stdin... */
#endif	
	printf("-Plaintext:%s\n", string);
	uint8_t *ceas_enc = caesar_encrypt(string,N);
	printf("-Encrypted:%s\n", ceas_enc);
	printf("-Decrypted:%s\n", caesar_decrypt(ceas_enc,N));
	free(ceas_enc);



/* ---------------------------------------------------------------------- */
	printf("\n\n **** Playfair cipher ****\n\n");
	printf("The Key is:%s\n", grid_key);
#if 0
	printf("Give key for 5x5 grid:");
	fgets((char *)grid_key, 2048, stdin);
#endif
	

	unsigned char *playf_enc = playfair_encrypt(string, grid_key);
	printf("-Plaintext:%s\n", string);
	printf("-Encrypted:%s\n", playf_enc);
	unsigned char *playf_dec = playfair_decrypt(playf_enc);
	printf("-Decrypted:%s\n", playf_dec);
	free(playf_enc);
	free(playf_dec);





/* ---------------------------------------------------------------------- */
	printf("\n\n\n **** Affine cipher ****\n");
	printf("-Plaintext:%s\n", string);
	uint8_t *aff_enc = affine_encrypt(string);
	printf("-Encrypted:%s\n", aff_enc);
	uint8_t *aff_dec = affine_decrypt(aff_enc);
	printf("-Decrypted:%s\n", aff_dec);
	free(aff_enc);
	free(aff_dec);

	




	/* ---------------------------------------------------------------------- */
	printf("\n\n\n **** Feistel cipher ****\n");
	printf("-Plaintext:%s\n", string);
	int len;
	uint8_t *feist_enc = feistel_encrypt(string,keys, &len);

	printf("-Encrypted:");
	for (i = 0; i < len; i++) {
		printf("%02X ", feist_enc[i]);
	}
	//printf("\n-Encrypted%s\n", feist_enc);
	uint8_t *feist_dec = feistel_decrypt(feist_enc,keys, len);
	printf("\n-Decrypted:%s\n\n", feist_dec);


	free(feist_enc);
	free(decrypted);

	for (i = 0; i < ROUNDS; i++) {
		free(keys[i]);
	}


	return 0;
}

