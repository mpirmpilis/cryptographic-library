/* Name: Georgios Mpirmpilis
 * File: cs457_crypto.h
 * cs457 - Assignment 1 - Simple Cipher Library
 */

#ifndef CS457_CRYPTO_H
#define CS457_CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define ROUNDS 8



extern int message_length;	/* used for the OTP encryption */
extern uint8_t *encrypted, *decrypted;  /* stores the encrypted-decrypted messages */
extern unsigned char playfair_grid[5][5];  /* the grid for playfair */



uint8_t* otp_encrypt(uint8_t *plaintext, uint8_t *key);
uint8_t* otp_decrypt(uint8_t *ciphertext, uint8_t *key);

uint8_t* caesar_encrypt(uint8_t *plaintext, uint16_t N);
uint8_t* caesar_decrypt(uint8_t *ciphertext, uint16_t N);




unsigned char *playfair_encrypt(unsigned char *plaintext, uint8_t key[]);
unsigned char *playfair_decrypt(unsigned char *ciphertext);
void playfair_keymatrix(unsigned char *key);

/* used in playfair cipher */
void insert_from_key(unsigned char ch);
unsigned char insert_from_hash(int hash_table[26]);
unsigned char *modified_plaintext(unsigned char *plaintext);
void char_row_col(unsigned char c, int *row, int *col);



uint8_t* affine_encrypt(uint8_t *plaintext);
uint8_t* affine_decrypt(uint8_t *ciphertext);


uint8_t* feistel_encrypt(uint8_t* plaintext, uint8_t *key[], int *length);
uint8_t* feistel_decrypt(uint8_t* ciphertext, uint8_t *keys[], int length);

uint8_t *pad_string(uint8_t *plaintext);


void round_func(uint8_t sub_arr[], uint8_t tmp_1[], int round_number, uint8_t *keys[]);
uint8_t *random_key(int len);

#endif	/* CS457_CRYPTO_H */
