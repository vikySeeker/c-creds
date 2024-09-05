#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/creds.h"
#include "../includes/mystrings.h"

#define MAX_PASS_LEN 64
#define MAX_NAME_LEN 255

strings* split_input(char *input) {
	if(input[0] == '/') {
		printf("Tag cannot be empty!\n\n");
		return NULL;
	}
	string s;
	s.value = input;
	s.len = strlen(input);
	char *delim = "/";
	return string_split(&s, delim);
}

char* get_password() {
	size_t len = MAX_PASS_LEN;
	char *pass1, *pass2;
	pass1 = (char*)malloc(len*sizeof(char));
	pass2 = (char*)malloc(len*sizeof(char));
	printf("Type your password: ");
	fgets(pass1, len, stdin);
	printf("Re-type your password: ");
	fgets(pass2, len, stdin);
	size_t pass1len, pass2len;

	//trimming newline at the end of buffer
	pass1len = strnlen(pass1, len);
	pass2len = strnlen(pass2, len);
	pass1[pass1len-1] = 0;
	pass2[pass2len-1] = 0;

	if(strnlen(pass1, len) >= len) {
		printf("Password length exceeded!\n"
		       "Default password length is %d", len);
		return NULL;
	}

	if(strncmp(pass2, pass1, len) != 0) {
		printf("Your passwords does not match!\n");
		return NULL;
	}
	return pass1;
}

char* get_username() {
	size_t len = MAX_NAME_LEN;
	char *name;
	name = (char*)malloc(len*sizeof(char));
	printf("Type your username or email: ");
	fgets(name, len, stdin);

	//trimming newline at the end of buffer
	size_t namelen = strnlen(name, len);
	name[namelen-1] = 0;
	return name;
}

void free_creds(creds *c) {
	if(c->username != NULL) {
		free(c->username);
		free(c->password);
	}
}
