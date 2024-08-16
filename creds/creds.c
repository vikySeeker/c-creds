#include <stdio.h>
#include <string.h>
#include "../includes/creds.h"
#include "../includes/mystrings.h"

#define MAX_PASS_LEN 64
#define MAX_NAME_LEN 255

strings* split_input(char *input) {
	string s;
	s.value = input;
	s.len = strlen(input);
	return string_split(&s, "/");
}

char* get_password() {
	size_t len = MAX_PASS_LEN;
	char *pass1, *pass2;
	printf("Type your password: ");
	scanf("%ms", &pass1);
	printf("Re-type your password: ");
	scanf("%ms", &pass2);

	if(strnlen(pass1, len) >= len) {
		printf("Password length exceeded!\n"
		       "Default password length is %d", len);
		return NULL;
	}

	if(strncmp(pass2, pass1, len) != 0) {
		printf("Your Entered password does not match!\n");
		return NULL;
	}
	return pass1;
}

char* get_username() {
	size_t len = MAX_NAME_LEN;
	char *name;
	printf("Type your username or email: ");
	scanf("%ms", &name);
	if(strnlen(name, len) == len) {
		printf("Username exceed the length!\n");
		return NULL;
	}
	return name;
}
