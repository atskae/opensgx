/*
 *  Copyright (C) 2015, OpenSGX team, Georgia Tech & KAIST, All Rights Reserved
 *
 *  This file is part of OpenSGX (https://github.com/sslab-gatech/opensgx).
 *
 *  OpenSGX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenSGX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenSGX.  If not, see <http://www.gnu.org/licenses/>.
 */

// test openssl api

#include "../test.h"
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <string.h>
#include <openssl/rand.h>

void enclave_main() {

	// https://shanetully.com/2012/04/simple-public-key-encryption-with-rsa-and-openssl/
	
	//int plaintext_size = 65536;
	int plaintext_size = 64; // rsa is significantly more expensive than the other crypto algorithms...
    int key_length = 1024;	
	char err[130];

    // create key	
	RSA* keypair = RSA_generate_key(key_length, 3, NULL, NULL); // (key length, public exponent)	

    // create plaintext
	unsigned char plaintext[plaintext_size];
	RAND_bytes(plaintext, plaintext_size);
 
    // create ciphertext	
	const size_t ciphertext_size = RSA_size(keypair);
    unsigned char ciphertext[ciphertext_size];
	memset(ciphertext, 0, ciphertext_size);

    // encrypt   
	int encrypt_len = RSA_public_encrypt(plaintext_size, plaintext, ciphertext, keypair, RSA_PKCS1_OAEP_PADDING);	
    
	if(encrypt_len == -1) {
		printf("Encryption failed.\n");
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		fprintf(stderr, "Error encrypting message: %s\n", err);	
	} else printf("RSA encryption complete.\n");

	sgx_exit(NULL);

}
