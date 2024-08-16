#include<stdio.h>

#include "../includes/modes.h"
#include "../includes/creds.h"
#include "../includes/mystrings.h"

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
		printf("Null check works!\n");
	}
	if(c.username == NULL) {
		printf("Null check works!\n");
	}
	
	printf("Credentials collected successfully!\n");
	printf("You enterd username or email is: %s\n", c.username);
	printf("Your entered password is: %s\n", c.password);
	printf("You enterd tag is: %s\n", c.tag);
	printf("Your entered space is: %s\n", c.space);

	
	/*switch	(get_mode()) {
		case Save:
			
	}*/
}
