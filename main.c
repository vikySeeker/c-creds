#include<stdio.h>
#include<getopt.h>

#include "db/db.h"

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
	       "Options:\n\t-h: help.\n"
	       "-u: username or email.\n"
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
 * This is the main function, what else it may do? as intended it only calls few function to make this program work.
 *
 * */
int main (int argc, char **argv) {
	if(prepare_env() == 1)
		return 1;

	if (argc < 2) {
		print_help();
		return 0;
	}
	
	int mode=0, ret_code = 0;
	char *action = argv[1];

	switch(action[0]) {
		case 's':
			mode = 1;
			ret_code = save_creds(argc, argv);
			break;
		case 'g':
			mode = 2;
			if(argc < 3 || argv[2] == "") {
				print_help();
				return 1;
			}
			ret_code = get_creds(argv[2]);
			break;
		case 'd':
			mode = 3;
			if(*(argv+2) == 0) {
				ret_code = 0;
				break;
			}
			ret_code = delete_creds(argv[2]);
			break;
		case 'i':
			mode = 4;
			//interactive();
			break;
		default:
			print_help();
			break;
	}

	return 0;
}
