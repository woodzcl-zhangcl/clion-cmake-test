// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/5.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MYPORT  9091
#define BUFFER_SIZE 1024

TEST_CASE("test", "[MAC Client]") {
    ssize_t len;
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    if (0 > connect(sock_cli, (struct sockaddr *) &servaddr, sizeof(servaddr))) {
        perror("connect");
        exit(1);
    }

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];

    strcpy(sendbuf, "Hello, I am li hong");
    len = write(sock_cli, sendbuf, strlen(sendbuf));

    len = read(sock_cli, recvbuf, sizeof(recvbuf));
    if (0 <= len) {
        recvbuf[len] = '\0';
        printf("recv: %s.\n", recvbuf);
    }

    close(sock_cli);

//    pause();
}