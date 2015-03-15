#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <sys/times.h>
#include <string.h>
#include <assert.h>

#define OPENSSL_LOAD_CONF     1
#include <openssl/evp.h>
#undef SHLIB_VERSION_NUMBER
#define SHLIB_VERSION_NUMBER "0.9.8:1.0.1"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "poissinv.h"

#define KEYS 8192

void showkey (const char *title, BIO *key) {
	size_t len = BIO_pending(key);
	char *s;
	s = malloc(len+1);
	BIO_read(key, s, len); s[len] = '\0';
	printf("%s\n%s\n", title, s);
	free(s);
}

void showkeys (RSA *keypair) {
	BIO *prv = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(prv, keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSAPublicKey(pub, keypair);

	showkey("Private Key:", prv);
	showkey("Public Key:", pub);

	BIO_free_all(prv); BIO_free_all(pub);
}

void showerr(const char *typ) {
	char *err = malloc(130);
	ERR_load_crypto_strings();
     	ERR_error_string(ERR_get_error(), err);
	fprintf(stderr, "Error %s message: %s\n", typ, err);
	free(err);
}
// Sleep for time (poisson distributed around specified mean in us) 
void thinksome(int think) {
        struct timespec t1, t2;
	think = poissinv(think);
	t1.tv_sec = think / 1000000;           		// full seconds
	t1.tv_nsec = (long) (think % 1000000) * 1000000L;  // rest in ns
	nanosleep(&t1, &t2);
}

void doRSA(RSA *keypair, int pass, int think) {
	char *clear = "The quick brown fox jumps over the lazy dog";
	int msglen = RSA_size(keypair)-42 < strlen(clear) ? RSA_size(keypair)-42 : strlen(clear);
	assert(RSA_size(keypair) - strlen(clear) >= 8);
	char *crypt = malloc(RSA_size(keypair));
	int crypt_len = RSA_public_encrypt(msglen,
				(unsigned char *) clear, 
				(unsigned char *) crypt, 
				keypair,
				RSA_PKCS1_OAEP_PADDING);
	if (-1 == crypt_len) {
		 showerr("encrypting");
	} else {
 		char *text = malloc(crypt_len);
		unsigned int i;
		for (i=0; i < pass; i++) {
			if (RSA_private_decrypt((int) crypt_len,
					(unsigned char *) crypt,
					(unsigned char *) text, 
					keypair, RSA_PKCS1_OAEP_PADDING) == -1) {
				showerr("decrypting");
        		}
			if (think) thinksome(think);
		}
		printf("Did %d RSA private decrypts\n", i);
		free(text);
	}
	free(crypt);
}

int main (int argc, char **argv) {
	int keys = 2048;
	int pass = 1000;
	int think = 0; 
	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "k:p:d:")) != -1) {
		switch (c) {
		case 'k':
			keys = atoi(optarg);
			break;
		case 'p':
			pass = atoi(optarg);
			break;
		case 'd':			// Think time in us
			think = atoi(optarg);
			break;
                case ':':
                        fprintf(stderr, "Missing value for option -%c\n", optopt);
                        break;
		default: 
                        if (isprint (optopt))
                                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                        else
                                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
	}
	printf("With %d keys and %d passes\n", keys, pass);


	OpenSSL_add_all_algorithms();
	assert(keys <= KEYS);

	RSA *keypair = RSA_generate_key(keys, 3, NULL, NULL);
  	
	doRSA(keypair, pass, think);

	RSA_free(keypair);

	return 0;
}

