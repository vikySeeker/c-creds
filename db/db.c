#include<stdio.h>
#include<sqlite3.h>
#include<string.h>
#include<getopt.h>

#include "../includes/db.h"

sqlite3 *db = NULL;	//Global database handler variable

/* 
* This function has lot of responsiblities, it is called first every time the program is executed.
* to prepare the environment for the program to run.
* it creates the required database if it is not present and sets up the environment.
* it returns an integer 0 if success, 1 if any failure.
*
* */
int prepare_env () {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	if (db == NULL) {
		return 1;
	}
	char *sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS \
                     '%q'(space TEXT PRIMARY KEY);\
                     CREATE TABLE IF NOT EXISTS \
                     '%q'(id INTEGER PRIMARY KEY AUTOINCREMENT, \
                     username TEXT, password TEXT, tag TEXT, \
                     space TEXT,FOREIGN KEY (space) REFERENCES '%q'(space));", 
		     SPACES_TABLE, 
		     CREDS_TABLE, 
		     SPACES_TABLE);
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		free_db_stuff(err_msg);
		return 1;
	}
	free_db_stuff(err_msg);
	return 0;
 }

/* 
 * Fuction used to free the memory used by this program either on success or failure.
 * it takes err_msgof type char* as argument. the variable that is used to save the error message of previously executed database operation.
 * it also free the memory of global db handler if it is not null.
 *
 * */
void free_db_stuff(char *err_msg) {
	if(err_msg != NULL) {
		sqlite3_free(err_msg);
	}
	if(db != NULL) {
		sqlite3_close(db);
		db = NULL;
	}
}

/*
 * It is responsible for creating database handler that is used by other functions.
 * it returns address for newly created sqlite3 db handler.
 * it returns NULL if any error occurs.
 *
 * */
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

/*
 * is_space_exist function checks if a given is exist or not
 * it return 0 if requested space did not exist. 1 if it exist
 * it returns -1 if any error occurs.
 *
 * */
int is_space_exist(creds *cred) {	
	sqlite3 *db = getDBHandle();

	if(db == NULL) {
		return -1;
	}

	char *sql = sqlite3_mprintf("SELECT COUNT(*) from '%q' where space='%q';", SPACES_TABLE, cred->space);
	char *err_msg = NULL;
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	if(sqlite3_step(res) != SQLITE_ROW) {
		return -1;
	}
	
	int is_space_exist = 0;
	do {
		is_space_exist = sqlite3_column_int(res, 0);
	} while(sqlite3_step(res) == SQLITE_ROW);
	
	return is_space_exist;
}

/*
 * create_space function creates a new space given by user.
 * it takes a pointer to creds as a parameter.
 * user needs to use is_space_exist function before calling create_space.
 *
 * */
int create_space(creds *cred) {	
	creds c = *cred;
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	int rc = 0;

	if(db == NULL) {
		return -1;
	}

	char *sql = sqlite3_mprintf("INSERT INTO '%q'(space) VALUES ('%q');", SPACES_TABLE, c.space);
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if(rc != SQLITE_OK) {
		puts("Cannot store data!");
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		free_db_stuff(err_msg);
		return -1;
	}

	printf("Space %s created successfully\n", cred->space);
	free_db_stuff(err_msg);
	return 1;
}


/*
 * save_creds is responsible for saving the credentials given by user as commandline argument in the database..
 * it takes an integer and double char pointer as arguments which is probably the count of char* variables and char* data itself.
 * it return integer 1 or 0.
 *
 * */
int save_creds (creds *cred) {
	creds c = *cred;
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	int rc = 0;

	if(db == NULL) {
		return 1;
	}
	if (is_space_exist(cred) != 1)  {
		printf("\nThe requested space %s, did not exist!\nDo you want to create a new one? (y/N) ", cred->space);
		char answer = 'y';
		answer = fgetc(stdin);
		if(answer == 'N') {
			return 2;
		}
		if (create_space(cred) != 1) {
			printf("Some error occured while creating space!\n");
			return 3;
		}
	}
	char *sql = sqlite3_mprintf("INSERT INTO '%q'(username, password, tag, space) VALUES ('%q', '%q', '%q', '%q');", CREDS_TABLE, c.username, c.password, c.tag, c.space);
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if(rc != SQLITE_OK) {
		puts("Cannot store data!");
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
		free_db_stuff(err_msg);
		return 1;
	}

	puts("Stored successfully\n");
	free_db_stuff(err_msg);
	return 0;
}

/*
 * list_creds lists the available matching creds from the creds table;
 * it takes creds *cred as an argument to search for matching creds.
 * it does not return anything.
 * */
void list_creds(creds *cred) {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	sqlite3_stmt *res = NULL;
	if(db == NULL) {
		return;
	}
	creds c = *cred;
	char *sql = sqlite3_mprintf("SELECT tag, space FROM '%q' WHERE tag = '%q'", CREDS_TABLE, c.tag);
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
		return;
	}
	if(sqlite3_step(res) != SQLITE_ROW){
		fputs("No data found... Exiting!\n", stdout);
		free_db_stuff(err_msg);
		sqlite3_free(res);
		return;
	}
	sqlite3_reset(res);
	printf("Listing matching tag and spaces...\n");
	while(sqlite3_step(res) == SQLITE_ROW) {
		printf("%s/%s\n", sqlite3_column_text(res, 0), sqlite3_column_text(res, 1));
	}
}

/*
 * get_creds retrieves the pasword of the credentials that matches the users search value.
 * it uses get_creds_id to get the id of the exact credentials that the user want see.
 * it takes one arguments the search value which is the vaule given by the user to search for matching credentials.
 *
 * */ 
int get_creds(creds *cred) {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	sqlite3_stmt *res = NULL;
	if(db == NULL) {
		return 1;
	}
	
	creds c = *cred;

	if(is_space_exist(cred) == 1 && !(strncmp(c.space, NO_SPACE, (size_t)12) == 0)) {
		char *sql = sqlite3_mprintf("SELECT username, tag, password FROM '%q' WHERE tag = '%q' AND space='%q' LIMIT 1", CREDS_TABLE, c.tag, c.space);

		//get the creds with matching tag and space from the db
		int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		if(rc != SQLITE_OK) {
			fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
		}
		if(sqlite3_step(res) != SQLITE_ROW){
			fputs("No data found... Exiting!\n", stdout);
			free_db_stuff(err_msg);
			sqlite3_free(res);
			return 1;
		}

		printf("Username: %s\n", sqlite3_column_text(res, 0));
		printf("Tag: %s\n", sqlite3_column_text(res, 1));
		printf("Password: %s\n", sqlite3_column_text(res, 2));
		sqlite3_free(res);
		free_db_stuff(err_msg);
	} else {
		list_creds(cred);
	}
	return 0;
}

/*
 * delete_creds deletes the password based on the matching id.
 * it uses get_creds_id to get the id of the exact credentials that the user want delete.
 * it takes one arguments the search value which is the vaule given by the user to search for matching credentials.
 *
 * */
int delete_creds(creds *cred) {
	sqlite3 *db = getDBHandle();
	char *err_msg = NULL;
	sqlite3_stmt *res = NULL;
	if(db == NULL) {
		return 1;
	}

	creds c = *cred;
	if(is_space_exist(cred) == 1 && !(strncmp(c.space, NO_SPACE, (size_t)12) == 0)) {
		char *sql = sqlite3_mprintf("DELETE FROM '%q' WHERE space = '%q' AND tag = '%q'", CREDS_TABLE, c.space, c.tag);

		int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
		if(rc != SQLITE_OK) {
			fprintf(stderr, "SQL ERROR: %s\n", err_msg);
			return 1;
		}
		fprintf(stdout, "\nCredentials deleted successfully!\n\n");

	
	} else {
		list_creds(cred);
	}
	free_db_stuff(err_msg);
	return 0;
}
