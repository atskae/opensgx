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
#include <openssl/sha.h>
#include <string.h>
#include <openssl/rand.h>

#define SHA_OUTPUT_SIZE 20 // 160-bit hash

void enclave_main() {
	
	size_t plaintext_size = 65536;
		
    // create plaintext
	unsigned char plaintext[plaintext_size];
	RAND_bytes(plaintext, plaintext_size);

	SHA_CTX c;
	SHA1_Init(&c);

	unsigned char hash[SHA_OUTPUT_SIZE]; // SHA1 160-bit hash
	SHA1(plaintext, plaintext_size, hash);

	sgx_exit(NULL);

}
