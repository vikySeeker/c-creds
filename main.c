#include<stdio.h>
#include<getopt.h>
#include<string.h>

#include "includes/db.h"
#include "includes/modes.h"

void print_invalid();
void print_help ();
void interactive ();

/*
 * the print_help function act as the help section for the user.
 *
 * */
void print_help () {
	printf("Usage: secrets <action> [options]\n\n"
	       "Actions:\n\tsave|s:to save the given credentials.\n"
	       "\tget|g: to retrieve a specific account/domain's password based on the given username or tag.\n"
	       "\tdelete|d: to delete a credential of a given username or name value.\n"
	       "\tinteractive|i: to start an interactive session.\n\n"
	       "Options:\n"
	       "\t-h | --help: help.\n"
	       "\t-u: username or email.\n"
	       "\t-p: password.\n"
	       "\t-t: name of the account or domain of the given credentials.\n\n"
	       "NOTE:\n\tThis program only checks for the first letter in the action word.\n"
	       "\tFor example, if save is the action the it only check for the first letter 's'.\n");
}

/*
 * this function is amied to provied an interactive functionality for this program.
 * 
 * BUT IS IS IT COMPLETED.
 *
 * */
void interactive () {
	puts("welcome to secrets!\n");
	puts("Please select any action to continue.");
	puts("[1] Save New Password.\n[2] Get Saved Password.\n[3]. Delete Password.\n[4] Exit.\n\n");
	printf("Enter your choice: ");
	int choice=0;
	scanf("%d",&choice);
	switch(choice) {
		case 1:
			puts("Saving...");
			break;
		case 2:
			puts("Retrieving...");
			break;
		case 3:
			puts("Deleting...\n");
			break;
		case 4:
		default:
			puts("Exiting...");
			break;
	}
	return;
}
 
/*
 * parse_mode() prases the first command line argument and comapres it with valid avaliable modes
 * it returns a mode from the list modes defined in the enum Mode
 * it returns invalid mode when the given modevalue does not match any valid mode.
 */
Mode parse_mode(const char *modevalue) {
	if (strncmp(modevalue, "-h", (size_t)2) == 0 || strncmp(modevalue, "--help", (size_t)6) == 0) {
		return Help;
	}

	if (strncmp(modevalue, "save", (size_t)4) == 0)	 {
		return Save;
	}

	if (strncmp(modevalue, "get", (size_t)3) == 0)	 {
		return Get;
	}

	if (strncmp(modevalue, "delete", (size_t)6) == 0)	 {
		return Delete;
	}

	if (strncmp(modevalue, "save", (size_t)4) == 0)	 {
		return Save;
	}

	if (strncmp(modevalue, "shell", (size_t)5) == 0 || strncmp(modevalue, "cli", (size_t)3) == 0)	 {
		return Interactive;
	}

	return Invalid;
}

//declare print_invalid function to use if given arguments are invalid!
void print_invalid (int *ret_code){
	*ret_code = -1;
	printf("Invalid use of program! use -h or --help for help\n");
}

/* 
 * This is the main function, what else it may do? as intended it only calls few function to make this program work.
 *
 * */
int main (int argc, char **argv) {

	if(prepare_env() == 1)
		return 1;
	
	int ret_code = 0;
	
	Mode mode = parse_mode(argv[1]);
	if (mode == Help) {
		print_help();
		return 0;
	}

	if (argc != 3) {
		print_invalid(&ret_code);
		return ret_code;
	}

	if (mode == Invalid){
		print_invalid(&ret_code);
		return ret_code;
	}
	set_mode(mode);
	process_mode(argv[2]);
	//creds *c = get_creds();

	return ret_code;
}
