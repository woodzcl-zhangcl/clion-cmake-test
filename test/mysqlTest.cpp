// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/3.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <stdio.h>

#include "mysql.h"

TEST_CASE("test", "[MySql]") {
    SECTION("connect") {
        MYSQL my_connection;
        mysql_init(&my_connection);

        if (mysql_real_connect(&my_connection, "localhost", "root", nullptr, "mysql", 0, nullptr, 0)) {
            printf("Connection success/n");
            mysql_close(&my_connection);
        } else {
            fprintf(stderr, "Connection failed/n");
            if (mysql_errno(&my_connection)) {
                fprintf(stderr, "Connection error %d: %s/n", mysql_errno(&my_connection),
                        mysql_error(&my_connection));
            }
        }
    }
    SECTION("query tabless") {
        MYSQL my_connection;
        mysql_init(&my_connection);

        if (mysql_real_connect(&my_connection, "localhost", "root", nullptr, "mysql", 0, nullptr, 0)) {
            printf("Connection success/n");
            if (0 == mysql_real_query(&my_connection, "use mysql", strlen("use mysql"))) {
                if (0 == mysql_real_query(&my_connection, "show tables", strlen("show tables"))) {
                    MYSQL_RES *result;
                    result = mysql_store_result(&my_connection);
                    if (result) {
                        unsigned int num_fields;
                        MYSQL_FIELD *field;
                        num_fields = mysql_num_fields(result);
                        printf("\n");
                        while (nullptr != (field = mysql_fetch_field(result))) {
                            printf("field name: %s\n", field->name);
                        }

                        MYSQL_ROW row;
                        while (nullptr != (row = mysql_fetch_row(result))) {
                            unsigned long *lengths;
                            lengths = mysql_fetch_lengths(result);

                            for (unsigned int i = 0; i < num_fields; i++) {
                                printf("%.*s ", (int) lengths[i], row[i] ? row[i] : "NULL");
                            }
                            printf("\n");
                        }
                    }
                }
            }
            mysql_close(&my_connection);
        } else {
            fprintf(stderr, "Connection failed/n");
            if (mysql_errno(&my_connection)) {
                fprintf(stderr, "Connection error %d: %s/n", mysql_errno(&my_connection),
                        mysql_error(&my_connection));
            }
        }
    }
}

