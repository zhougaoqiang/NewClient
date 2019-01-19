#pragma once
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <stdlib.h>

#pragma warning(disable:4996)  //关闭4996警告

char sqlPassword[100];

static int callbackTableCreation(void *notUsed, int argc, char**argv, char **azColName);
void tableCreation(int argc, char*argv[]);

static int callbackInsert(void *notUsed, int argc, char**argv, char **azColName);
bool tableInsertion(char *Input1, char *Input2, char *Input3);  //input1: 用户名； input2: 密码；input3：IP地址相关（备用）

static int callbackSelect(void *notUsed, int argc, char**argv, char **azColName);
bool tableSelection(char *Input1, char *Input2);

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
	sql = "CREATE TABLE PASSWORDVALIDATION(USERNAME TEXT PRIMARY KEY NOT NULL,PASSWORD TEXT  NOT NULL, IP_ADDRESS TEXT);";

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
	int j;
	for (j = 0; j < argc; j++)
	{
		printf("%s = %s\n", azColName[j], argv[j] ? argv[j] : "NULL");
	}
	printf("\n");
	return 0;
}
bool tableInsertion(char *Input1, char *Input2, char *Input3) { //int argc, char * argv[]
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
	const char *insert1 = "INSERT INTO PASSWORDVALIDATION(USERNAME, PASSWORD,IP_ADDRESS) VALUES('";
	const char *insert2 = "','";
	const char *insert3 = "');";
	string temp = string(insert1) + string(Input1) + string(insert2) + string(Input2) + string(insert2) + string(Input3) + string(insert3);
	const char *Finalsql = NULL;
	Finalsql = temp.c_str();  
	//const char *sql;
	// sql = "INSERT INTO PASSWORDVALIDATION(USERNAME, PASSWORD, IP_ADDRESS) VALUES ('zhougaoqiang','123456', '10.10.10.10')";

	/*execute SQL STATEMENT*/
	rc = sqlite3_exec(db, Finalsql, callbackInsert, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	else
	{
		fprintf(stdout, "Insert successfully\n");
		sqlite3_close(db);
		return true;
	}
	
}

static int callbackSelect(void *notUsed, int argc, char**argv, char **azColName) {
	memset(sqlPassword, '\0', sizeof(sqlPassword));
	strcpy(sqlPassword, argv[0]);
	return 0;
}
bool tableSelection(char *Input1, char *Input2)  //输入用户名/密码，检查密码。
{
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
	const char *insert1 = "SELECT PASSWORD FROM PASSWORDVALIDATION WHERE USERNAME = '";
	const char *insert2 = "','";
	const char *insert3 = "');";
	string temp = string(insert1) + string(Input1) + string(insert3);
	const char *Finalsql = NULL;
	Finalsql = temp.c_str();
	const char *sql;
	sql = "SELECT PASSWORD FROM PASSWORDVALIDATION WHERE USERNAME = 'zhougaoqiang'";

	/*execute SQL STATEMENT*/
	rc = sqlite3_exec(db, sql, callbackSelect, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	else
	{
		sqlite3_close(db);
		if (strcmp(sqlPassword, Input2) == 0)
			return true;
		else
			return false;
	}
}