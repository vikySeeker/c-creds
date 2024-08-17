//include gaurd
#ifndef CREDS_DB_H
#define CREDS_DB_H

#include<sqlite3.h>
#include "creds.h"

int prepare_env();
void free_db_stuff(char *err_msg);
sqlite3* getDBHandle();
int is_space_exist(creds*);
int create_space(creds *c);
int save_creds (creds*);
int get_creds_id(char *, sqlite3*);
int get_creds (creds*);
int delete_creds (creds*);

#endif
