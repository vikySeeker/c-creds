//include gaurd
#ifndef CREDS_CREDS_H
#define CREDS_CREDS_H

#include "mystrings.h"

// struct used to store credentials that are handled by this program
typedef struct creds {
	char *username;	//not mandatory
	char *password;
	char *tag;
	char *space;
} creds;

strings* split_input(char*);
char* get_username();
char* get_password();
#endif
