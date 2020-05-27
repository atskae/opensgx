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


#include "../test.h"
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/blowfish.h>
#include <string.h>

#define BLOWFISH_BLOCK_SIZE 8

void enclave_main() {
	
	int plaintext_size = 65536;	
    int key_length = 128;

    // create key
	unsigned char key_str[key_length/8];
	RAND_bytes(key_str, key_length/8);	
    BF_KEY key;
	BF_set_key(&key, key_length, key_str);

    // create plaintext
    unsigned char plaintext[plaintext_size];
	RAND_bytes(plaintext, plaintext_size);
 
    // create ciphertext
	const size_t ciphertext_size = ( (plaintext_size + BLOWFISH_BLOCK_SIZE) / BLOWFISH_BLOCK_SIZE) * BLOWFISH_BLOCK_SIZE; // ?
	unsigned char ciphertext[ciphertext_size];
	memset(ciphertext, 0, ciphertext_size);
	
    // initialization vector	
   	unsigned char iv[BLOWFISH_BLOCK_SIZE];
	RAND_bytes(iv, BLOWFISH_BLOCK_SIZE);
	
    BF_cbc_encrypt(plaintext, ciphertext, plaintext_size, &key, iv, BF_ENCRYPT);

    // encrypt
	sgx_exit(NULL);

}
