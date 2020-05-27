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
#include <openssl/dsa.h>
#include <openssl/engine.h>
#include <string.h>
#include <openssl/rand.h>

void enclave_main() {

	//int plaintext_size = 65536;
	int plaintext_size = 64; // dsa is significantly more expensive than the other crypto algorithms...
    int key_length = 1024;	
	char err[130];

    // create plaintext
	unsigned char plaintext[plaintext_size];
	RAND_bytes(plaintext, plaintext_size);

    // Generate DSA parameters
    DSA* dsa = DSA_generate_parameters(key_length, NULL, NULL, NULL, NULL, NULL, NULL);

    // Create key
    DSA_generate_key(dsa);

    // Allocate buffer for signature
    unsigned char* sig = (unsigned char*) malloc(DSA_size(dsa));

    // Allocate digest ?
    unsigned char md[EVP_MAX_MD_SIZE];

    // Sign the plaintext
    int siglen;
    int ret = DSA_sign(plaintext, md, plaintext_size, sig, &siglen, dsa);

   	if(ret == -1) {
		printf("Encryption failed.\n");
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		fprintf(stderr, "Error encrypting message: %s\n", err);	
	} else printf("DSA encryption complete.\n");

    // Clear DSA object
    DSA_free(dsa);

	sgx_exit(NULL);

}
