#pragma once
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <stdlib.h>

static int callbackTableCreation(void *notUsed, int argc, char**argv, char **azColName);
void tableCreation(int argc, char*argv[]);

static int callbackInsert(void *notUsed, int argc, char**argv, char **azColName);
void tableInsertion(char *Input1, char *Input2);



using namespace std;

void tableCreation()  //int argc, char*argv[]
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;

	//check database
	rc = sqlite3_open("test.db", &db);
	if (rc)
	{
		fprintf(stderr, "Can't Open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else
	{
		cout << "Opened database successfully.\n";
	}

	/*create SQL statement*/
	sql = "CREATE TABLE PASSWORDVALIDATION(USERNAME TEXT   NOT NULL,PASSWORD TEXT   NOT NULL);";

	/*execute SQL statement*/
	rc = sqlite3_exec(db, sql, callbackTableCreation, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Table create successfully\n");
	}
	sqlite3_close(db);
}
static int callbackTableCreation(void *notUsed, int argc, char**argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int callbackInsert(void *notUsed, int argc, char**argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
void tableInsertion(char *Input1, char *Input2) { //int argc, char * argv[]
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	

	//check database
	rc = sqlite3_open("test.db", &db);
	if (rc)
	{
		fprintf(stderr, "Can't Open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else
	{
		cout << "Opened database successfully.\n";
	}

	/*create SQL statement*/
	const char *insert1 = "INSERT INTO PASSWORDVALIDATION(USERNAME, PASSWORD) VALUES('";
	const char *insert2 = "','";
	const char *insert3 = "');";

	string temp = string(insert1) + string(Input1) + string(insert2) + string(Input2) + string(insert3);

	const char *Finalsql = NULL;
	Finalsql = temp.c_str();
	const char *sql;
	sql = "INSERT INTO PASSWORDVALIDATION(USERNAME, PASSWORD) VALUES ('zhougaoqiang','123456')";

	/*execute SQL STATEMENT*/
	rc = sqlite3_exec(db, Finalsql, callbackInsert, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Insert successfully\n");
	}
	sqlite3_close(db);
}