#include<stdio.h>

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

int process_mode(char *input) {
	//printf("Parsing input: %s !\n", input);
	strings *inputs = split_input(input);

	/*printf("Parsed the input successfully!\n");
	printf("Length of split_input: %d", inputs->length);\
	for(int i=0; i < inputs->length; i++) {
		printf("Word %d: %s\n", i+1, inputs->arr[i].value);
	}*/

	if(inputs->length != 2) {
		return -1;
	}

	creds c;
	c.tag = inputs->arr[0].value;
	c.space = inputs->arr[1].value;
	c.username = get_username();
	c.password = get_password();


	if(c.password == NULL) {
		return 1;
	}

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
	return 0;
}
