//include gaurd
#ifndef CREDS_DB_H
#define CREDS_DB_H

#define CREDS_TABLE   "creds"
#define SPACES_TABLE  "spaces"

#define DEFAULT_SPACE "default"
#define NO_SPACE      "__no_space__"
#include<sqlite3.h>
#include "creds.h"

int prepare_env();
void free_db_stuff(char *err_msg);
sqlite3* getDBHandle();
int is_space_exist(creds*);
int is_cred_exist(creds*);
int create_space(creds *c);
int save_creds (creds*);
int list_creds (creds*);
int get_creds (creds*);
int update_creds(creds*)
int delete_creds (creds*);

#endif
