#include<stdio.h>
#include<getopt.h>
#include<sqlite3.h>
#include<string.h>

typedef struct creds {
	char *username;
	char *password;
	char *tag;
} creds;

void print_help ();
int prepare_env();
void free_db_stuff(char *err_msg);
sqlite3* getDBHandle();
int save_creds (int argc, char **argv);
int get_creds_id(char *search_value, sqlite3 *db);
int get_creds (char *search_value);
int delete_creds (char *search_value);
void interactive ();

sqlite3 *db = NULL;

void free_db_stuff(char *err_msg) {
	if(err_msg != NULL) {
		sqlite3_free(err_msg);
	}
	if(db != NULL) {
		sqlite3_close(db);
		db = NULL;
	}
}

sqlite3* getDBHandle () {
	char *err_msg = NULL;
	if (db != NULL) {
		return db;
	}
	int rc = sqlite3_open("./secrets.db", &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Database Error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		db = NULL;
		return NULL;
	}

	return db;
}

int prepare_env () {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	if (db == NULL) {
		return 1;
	}
	char *sql = "CREATE TABLE IF NOT EXISTS creds(id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT, tag TEXT);";
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		free_db_stuff(err_msg);
		return 1;
	}
	free_db_stuff(err_msg);
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
	char *err_msg = NULL;
	int rc = 0;

	if(db == NULL) {
		return 1;
	}

	char *sql = sqlite3_mprintf("INSERT INTO creds(username, password, tag) VALUES ('%q', '%q', '%q');", c.username, c.password, c.tag);
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if(rc != SQLITE_OK) {
		puts("Cannot store data!");
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		free_db_stuff(err_msg);
		return 1;
	}

	puts("Executed successfully\n");
	free_db_stuff(err_msg);
	return 0;
}

int get_creds_id(char *search_value, sqlite3 *db) {
	if(db == NULL) {
		return -1;
	}

	char *sql = sqlite3_mprintf("SELECT id, username, tag from creds where username like '%%%q%%' or tag like '%%%q%%';", search_value, search_value);
	char *err_msg = NULL;
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	int idx = 1;
	printf("Listing ID, Username and Tag of matched credentials...\n");
	while(sqlite3_step(res) == SQLITE_ROW) {
		printf("\n%d. Username = %s\tTag = %s\n\n", sqlite3_column_int(res, 0), sqlite3_column_text(res, 1), sqlite3_column_text(res, 2));
		idx++;
	}
	if(idx == 1) {
		fputs("No Data found in the database...\n", stdin);
		return -1;
	}
	printf("Enter the number to continue:");
	char buf[10];
	fgets(buf, sizeof buf, stdin);

	int id = -1;
	if(sscanf(buf, "%d", &id) != 1 || id < 1) {
		fputs("Invalid input detected... Exiting!\n", stdout);
		return -1;
	}
	return id;
}

int get_creds(char *search_value) {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	sqlite3_stmt *res = NULL;
	if(db == NULL) {
		return 1;
	}
	int id = get_creds_id(search_value, db);
	if(id == -1) {
		free_db_stuff(err_msg);
		return 1;
	}
	char *sql = sqlite3_mprintf("SELECT password FROM creds WHERE id = %d LIMIT 1", id);
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	if(rc != SQLITE_OK) {	
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		free_db_stuff(err_msg);
		return 1;
	}
	if(sqlite3_step(res) != SQLITE_ROW){
		fputs("No data found... Exiting!\n", stdout);
		free_db_stuff(err_msg);
		sqlite3_free(res);
		return 1;
	}

	printf("\n\n------------------------\nPassword: %s\n------------------------\n\n", sqlite3_column_text(res, 0));
	sqlite3_free(res);
	free_db_stuff(err_msg);
	return 0;
}

int delete_creds(char *search_value) {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	if(db == NULL) {
		return 1;
	}
	int id = get_creds_id(search_value, db);
	if(id == -1) {
		free_db_stuff(err_msg);
		return 1;
	}
	printf("\nAre you sure you want to delete the selected credentials?(y/n):");
	char ch;
	ch = fgetc(stdin);
	if(ch != 'y' && ch != 'Y'){
		fputs("\n\nOperation Aborted... Exiting!\n", stdout);
		free_db_stuff(err_msg);
		return 1;
	}

	char *sql = sqlite3_mprintf("DELETE FROM creds WHERE id = '%d';", id);
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		free_db_stuff(err_msg);
		return 1;
	}
	fprintf(stdout, "\n\n\tCredentials deleted successfully!\n");
	free_db_stuff(err_msg);
	return 0;
}

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
