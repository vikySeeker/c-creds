#ifndef MY_STRING_LIB
#define MY_STRING_LIB
//function from cutils library

typedef struct string {
	int len;
	char* value;
} string;


typedef struct stringarray {
	int length;
	string* arr;
} strings;

int to_int(string*);

/* trims the leading and trailing spaces in a string
 * takes char** pointer as argument and returns number of spaces removed.
 * if returns -1 invalid string.
 *
 */
int string_trim(string*);


/* splits a string of type string struct based on given delimeter
 * returns a string array of type strings struct.
 * uses strtok under the hood.
 */
strings* string_split(const string*, const char*);

/* Joins the array of string of type strings struct     
 * uses seperators between each words
 * returns a pointer to a string struct.
 */
string* string_join(strings*, char*);

/* gets input from stdin til delimeter
 * delimeter is passed as argument.
 * passing '\n' as delim gets input from user til it encounters new line.
 * returns address of input.
 */
char* read_input(char);

#endif
