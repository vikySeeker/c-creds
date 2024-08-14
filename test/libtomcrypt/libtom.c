#include<tomcrypt.h>

// Test AES ecnryption and decryption using tomcrypt library file
// this test only implements CRT mode stream cipher using AES algorithm.
// To compile and run:
// `gcc -o aes_test aes_test.c -ltomcrypt`
int main() {
	//declaring variables required in the crypto procss. 
	unsigned char data[12] = "helloworld", 		//variable to hold both plain and cipher data
		      key[16] = "supersecretkeys!", 	//secret key used in crypto process.
		      IV[16];				//IV(initialization vector) required by crypto function sot store the starting point of crypto proess.
	symmetric_CTR ctr;				//like context storage;
	int err;

	//need to register the cipher using its descriptor name before using find_cipher function
	//using `aes_enc_desc` and `aes_desc` descriptors yeilds same result.
	if(register_cipher(&aes_enc_desc) == -1) {
		printf("Error registering cipher.\n");
		return -1;
	}

	//initializes the ctr mode with required value
	if((err = ctr_start(
	    find_cipher("aes"), //index of algorithm we are going to use
			    IV, //IV to be used by ctr
			   key, //key for encryption
			    16, //size of key 
			     0, //no of rounds. 0 indiacates to go with default no of rounds.
 CTR_COUNTER_LITTLE_ENDIAN | 4, //counter value
			  &ctr) //ctr state
	   ) != CRYPT_OK) {
		printf("Error initializing ctr mode: %s\n", error_to_string(err));
		//handle_error(err);
		return -1;
	}
	printf("Successfully initialized ctr!\n");
	
	printf("Given plaintext: %s\n", data);
	
	//ctr_encrypt is used to encrypt the given plaintext and stores in the given ciphertext's memeory
	if((err = ctr_encrypt(data, //plaintext
			      data, //ciphertext
		      sizeof(data), //buffer length of plaintext
		             &ctr)  //ctr state
	   ) != CRYPT_OK) {
		printf("ctr_encrypt error: %s\n", error_to_string(err));
		return -1;
	}

	printf("Encrypted text successfully: %s\n", data);
	for(int i=0; i<12; i++) {
		printf("index %d: %d\n", i, data[i]);
	}
	
	//ctr_setiv needs to be set after encryption for decryption purpose
	if((err = ctr_setiv(IV, //initial IV given to ctr_start 
			    16, //IV length
			  &ctr) //ctr state
	   ) != CRYPT_OK) {
		printf("ctr_setiv error: %s\n", error_to_string(err));
		return -1;
	}
	
	//ctr_decrypt is used to decrypt the given ciphertext and stores in the given plaintext's memeory
	if((err = ctr_decrypt(data, //ciphertext
			      data, //plaintext
		      sizeof(data), //plaintext buffer length
		             &ctr) //ctr state
	   ) != CRYPT_OK) {
		printf("ctr_decrypt error: %s\n", error_to_string(err));
		return -1;
	}
	printf("Decrypted text successfully: %s\n", data);
	

	//ctr_done terminates the stream
	if((err = ctr_done(&ctr)) != CRYPT_OK) {
		printf("ctr_done error: %s\n", error_to_string(err));
		return -1;
	}
	
	//clear up and return
	zeromem(key, sizeof(key));
	zeromem(&ctr, sizeof(ctr));

	return 0;
}
