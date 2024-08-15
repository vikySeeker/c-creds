//include gaurd
#ifndef CREDS_DB_H
#define CREDS_DB_H

#include<sqlite3.h>
// struct used to store credentials that are handled by this program
typedef struct creds {
	char *username;
	char *password;
	char *tag;
} creds;

int prepare_env();
void free_db_stuff(char *err_msg);
sqlite3* getDBHandle();
int save_creds (int argc, char **argv);
int get_creds_id(char *search_value, sqlite3 *db);
int get_creds (char *search_value);
int delete_creds (char *search_value);

#endif
