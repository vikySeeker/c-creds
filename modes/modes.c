#include <stdio.h>
#include <stdlib.h>

#include "../includes/modes.h"
#include "../includes/creds.h"
#include "../includes/mystrings.h"
#include "../includes/db.h"

Mode mode = Invalid;

void set_mode(Mode m) {
	mode = m;
}

Mode get_mode() {
	return mode;
}

void _print_creds(creds *c) {
	printf("Username: %s\n", c->username);
	printf("Password: %s\n", c->password);
	printf("Space: %s\n", c->space);
	printf("Tag: %s\n", c->tag);
}

int process_mode(char *input) {
	//printf("Parsing input: %s !\n", input);
	strings *inputs = split_input(input);
	if(inputs == NULL || inputs->length == 0) {
		printf("Invalid values provided!\n");
		free(inputs);
		return 5;
	}
	creds c;
	c.tag = inputs->arr[0].value;

	if(inputs->length == 2)
		c.space = inputs->arr[1].value;
	else
		c.space = DEFAULT_SPACE;

	c.username = get_username();
	c.password = get_password();

	if(c.password == NULL) {
		return 1;
	}
	_print_creds(&c);

	switch(get_mode()) {
		case Save:
			//call save function from db
			save_creds(&c);
			break;
		case Get:
			//call get function from db
			printf("calling get...\n");
			break;
		case Delete:
			//call delete function from db
			printf("calling delete...\n");
			break;
		default:
			printf("This mode is not supposed to be here!");
			break;
	}
	
	free_creds(&c);
	free(inputs);

	return 0;
}
