// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/7.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

TEST_CASE("sqlite", "[SQLITE]") {
    SECTION("create database test.db") {
        sqlite3 *db;
        int rc;

        rc = sqlite3_open("test.db", &db);

        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            exit(0);
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }
        sqlite3_close(db);
    }
    SECTION("operation for table company in database test.db") {
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        const char *sql;

        /* Open database */
        rc = sqlite3_open("test.db", &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            exit(0);
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }

        sql = "SELECT * FROM COMPANY";
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);

            /* Create SQL statement */
            sql = "CREATE TABLE COMPANY("  \
             "ID INT PRIMARY KEY     NOT NULL," \
             "NAME           TEXT    NOT NULL," \
             "AGE            INT     NOT NULL," \
             "ADDRESS        CHAR(50)," \
             "SALARY         REAL );";

            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                fprintf(stdout, "Table created successfully\n");
            }
        } else {
            /* Create SQL statement */
            sql = "SELECT * from COMPANY";

            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else {
                fprintf(stdout, "Operation done successfully\n");

                /* Create SQL statement */
                sql = "DELETE from COMPANY";

                /* Execute SQL statement */
                rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
                    fprintf(stdout, "Operation done successfully\n");

                    /* Create SQL statement */
                    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
                    "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
                    "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
                    "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
                    "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
                    "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
                    "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
                    "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

                    /* Execute SQL statement */
                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                    if (rc != SQLITE_OK) {
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    } else {
                        fprintf(stdout, "Records created successfully\n");
                    }

                    /* Create merged SQL statement */
                    sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
                    "SELECT * from COMPANY";

                    /* Execute SQL statement */
                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }else{
                        fprintf(stdout, "Operation done successfully\n");
                    }

                    /* Create merged SQL statement */
                    sql = "DELETE from COMPANY where ID=1; " \
                    "SELECT * from COMPANY";

                    /* Execute SQL statement */
                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }else{
                        fprintf(stdout, "Operation done successfully\n");
                    }
                }
            }
        }
        sqlite3_close(db);
    }
}


