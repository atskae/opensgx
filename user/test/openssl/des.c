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

// plaintext openssl api

#include "../test.h"
#include <openssl/bn.h>
#include <openssl/des.h>
#include <string.h>
#include <openssl/rand.h>

#define DES_BLOCK_SIZE 8

void enclave_main() {
	
	int plaintext_size = 65536;
	
    // create key
    DES_cblock key;
	DES_random_key(&key); // generates a random key	
	DES_key_schedule schedule;
	DES_key_sched(&key, &schedule); // runs key schedule

    // create plaintext
	unsigned char plaintext[plaintext_size];
	RAND_bytes(plaintext, plaintext_size);
    
    // create ciphertext
	const size_t ciphertext_size = ( (plaintext_size + DES_BLOCK_SIZE) / DES_BLOCK_SIZE) * DES_BLOCK_SIZE; // ?
	unsigned char ciphertext[ciphertext_size];
	memset(ciphertext, 0, ciphertext_size);
 
    // initialization vector	
	DES_cblock iv;
	memset(iv, 0, sizeof(iv));

    // encrypt	
	DES_ncbc_encrypt(plaintext, ciphertext, plaintext_size, &schedule, &iv, DES_ENCRYPT);			

	sgx_exit(NULL);

}
