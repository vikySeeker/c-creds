#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../includes/mystrings.h"


int string_trim(string* input);
int to_int(string* input);
strings* string_split(const string *str, const char *delim);
string* string_join(strings *src, char *delim);
char* read_input(char delim);

/* returns a signed int if given string is valid number;
 * returns -1 if given string is not a valid number;
 */
int to_int(string* input) {
	int number = 0;
	for(int i=0; i<input->len; i++) {
		if(input->value[i] >= 48 && input->value[i] <= 57) {
			number += input->value[i] - 48;
			if(i<input->len-1)
			number *=10;
			
		} else {
			return -1;
		}
	}
	return number;
}

/* trims the leading and trailing spaces in a string
 * takes char** pointer as argument and returns number of spaces removed.
 * if returns -1 invalid string.
 *
 */
int string_trim(string* input) {

	int len = input->len-1;
	if(input->len<=0) {
		return -1;
	}
	int i = 0;
	while(input->value[i] == ' ') {
		++i;
	}
	input->value = (input->value)+i;
	len -= i;
	while(input->value[len] == ' ') {
		input->value[len] = 0;
		--len;
		++i;
	}
	//input->value = (input->value)+i;
	input->len = len-i;
	return i;
}


/* splits a string of type string struct based on given delimeter
 * returns a string array of type strings struct.
 * uses strtok under the hood.
 */
strings* string_split(const string *str, const char *delim) {
	strings *s = calloc(1, sizeof(strings));
	const int buffer = 10;
	int tok_limit = buffer, tok_count = 0;
	string *sarray;
	sarray = calloc(tok_limit, sizeof(string));
	sarray[tok_count].value = strtok(str->value, delim);
	if(sarray[tok_count].value == NULL) {
		sarray[tok_count].len = 0;
		goto finalize;
	}

	while(sarray[tok_count].value != NULL) {
		sarray[tok_count].len = strlen(sarray[tok_count].value);
		tok_count++;
		if(tok_count >= tok_limit) {
			tok_limit += buffer;
			sarray = reallocarray(sarray, tok_limit, sizeof(string));
		}
		sarray[tok_count].value = strtok(NULL, delim);
	}
	finalize:
		s->length = tok_count;
		s->arr = sarray;
	return s;
}

/* Joins the array of string of type strings struct
 * uses seperators between each words
 * returns a pointer to a string struct.
 */
string* string_join(strings *src, char *delim) {
	if(delim == NULL) {
		delim = "";
	}
	//puts("Entered sjt..");
	int bc=0, wc = src->length, dc = strlen (delim);
	for(int i=0;i<wc;i++) {
		bc += src->arr[i].len;
	}
	
	if(strcmp(delim, "") != 0) {
		bc += strlen(delim)*(wc-1);
	}
	string *dest = (string*)malloc(sizeof(string));
	dest->value = malloc(bc);

	int len = 0;
	for(int i=0;i<wc;i++) {
		strcpy(dest->value+len, src->arr[i].value);
		len += src->arr[i].len;
		strcpy(dest->value+len, delim);
		len += dc;
	}
	len -= dc;
	dest->value[len] = 0;
	dest->len = len;
	return dest;
}


/* gets input from stdin til delimeter
 * delimeter is passed as argument.
 * passing '\n' as delim gets input from user til it encounters new line.
 * returns address of input.
 */
char* read_input(char delim) {

    int buffersize=0, currbuffer = 40; 
	char char_input;
	char *input;
	input = (char*)malloc(sizeof(char)*currbuffer);

	while((char_input=getchar())!=delim){
	input[buffersize++] = char_input;
			if(buffersize >= currbuffer) {
					currbuffer += 20;
					input = (char*)realloc(input, sizeof(char)+currbuffer);
			}
	}
	input[buffersize] = 0;
    return input;
}
