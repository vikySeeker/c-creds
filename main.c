#include<stdio.h>
#include<getopt.h>
#include<sqlite3.h>
#include<string.h>

typedef struct creds {
	char *username;
	char *password;
	char *tag;
} creds;

int print_help ();
int prepare_env();
sqlite3* getDBHandle();
int save_creds (int argc, char **argv);
creds* get_creds (char *search_value);
int delete_creds (char *search_value);
void interactive ();

sqlite3 *db = NULL;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	printf("%d", argc);
    if(argc == 0) {
    	puts("No data found!");
    }
    for(i = 0; i<argc; i++) {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

sqlite3* getDBHandle () {
	char *err_msg;
	if (db != NULL) {
		return db;
	}
	int rc = sqlite3_open("./secrets.db", &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Database Error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	return db;

}

int prepare_env () {
	sqlite3 *db = getDBHandle();
	char *err_msg;
	if (db == NULL) {
		return 0;
	}
	char *sql = "CREATE TABLE IF NOT EXISTS creds(username TEXT, password TEXT, tag TEXT);";
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return 1;
	}

	return 0;
} 

int save_creds (int argc, char **argv) {
	opterr = 0;
	char opt;
	creds c;
	c.tag = NULL;
	c.password = NULL;
	c.username = NULL;
	while((opt=getopt(argc-1, argv+1, "h::u:p:t:")) != -1) {
		switch(opt) {
			case 'u':
				c.username = optarg;
				break;
			case 'p':
				c.password = optarg;
				break;
			case 't':
				c.tag = optarg;
				break;
			case 'h':
				print_help();
				return 0;
		}
	}

	printf("Username: %s\nPassword: %s\nTag: %s\n", c.username, c.password, c.tag);
	sqlite3 *db = getDBHandle();
	char *err_msg = 0;
	int rc = 0;

	if(db == NULL) {
		return 0;
	}

	char *sql = sqlite3_mprintf("INSERT INTO creds(username, password, tag) VALUES ('%q', '%q', '%q');", c.username, c.password, c.tag);
	rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

	if(rc != SQLITE_OK) {
		puts("Cannot store data!");
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return 1;
	}

	puts("Executed successfully\n");
	sqlite3_free(err_msg);
	return 0;
}

creds* get_creds(char *search_value) {
	sqlite3 *db = getDBHandle();
	if(db == NULL) {
		return 0;
	}

	char *sql = sqlite3_mprintf("SELECT username, password, tag from creds where username like '%%%q%%' or tag like '%%%q%%';", search_value);
	char *err_msg;
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return NULL;
	}
	int idx = 1;
	while(sqlite3_step(res) == SQLITE_ROW) {
		printf("%d.Username = %s\n  Password = %s\n  Tag = %s\n\n", idx++, sqlite3_column_text(res, 0), sqlite3_column_text(res, 1), sqlite3_column_text(res, 2));
	}
	printf("Found total of %d credentials...\n", idx-1);
	sqlite3_free(res);

	return NULL;
}

int print_help () {
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

int main (int argc, char **argv) {
	prepare_env();

	if (argc < 2) {
		print_help();
		return 0;
	}
	
	int mode=0;
	char *action = argv[1];

	switch(action[0]) {
		case 's':
			mode = 1;
			save_creds(argc, argv);
			break;
		case 'g':
			mode = 2;
			if(argc < 3 || argv[2] == "") {
				print_help();
				return 1;
			}
			get_creds(argv[2]);
			break;
		case 'd':
			mode = 3;
			//delete_creds(argv[2]);
			break;
		case 'i':
			mode = 4;
			interactive();
			return 0;
		default:
			print_help();
			return 0;
	}

	return 0;
}
