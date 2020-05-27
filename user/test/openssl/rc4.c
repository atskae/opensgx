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
#include <openssl/rc4.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

// https://github.com/plenluno/openssl/blob/master/openssl/crypto/rc4/rc4.c
void enclave_main() {
	
    size_t plaintext_size = 65536;

    char* keystr = NULL;
    RC4_KEY key;
    char buf[BUFSIZ];	
    unsigned char md[MD5_DIGEST_LENGTH];

    // create plaintext
	unsigned char plaintext[plaintext_size];
	RAND_bytes(plaintext, plaintext_size);

    // get key string    
    int ret = EVP_read_pw_string(buf, BUFSIZ, "Enter RC4 password:", 0);
    if(ret != 0) {
        OPENSSL_cleanse(buf, BUFSIZ);
        fprintf(stderr,"bad password read\n");
        sgx_exit(NULL);
    }
    keystr = buf;

    // set up key
    EVP_Digest((unsigned char *)keystr, strlen(keystr), md, NULL, EVP_md5(), NULL);
    OPENSSL_cleanse(keystr, strlen(keystr));
    RC4_set_key(&key, MD5_DIGEST_LENGTH, md);

    // encrypt
    RC4(&key, (unsigned int)plaintext_size, (unsigned char *)buf, (unsigned char *)buf);

	sgx_exit(NULL);

}
