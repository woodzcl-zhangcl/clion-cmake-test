// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/5.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

using namespace std;
#define MAXLINE 256
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

bool bExit = false;

// sigint signal
void handler(int Signal) {
    bExit = true;
}

// set none block soeckt
void setnonblocking(int sock) {
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock, SETFL, opts)");
        exit(1);
    }
}

TEST_CASE("test", "[UNIX Server]") {
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds, portnumber = 9091;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;

    // capture sigint
    signal(SIGINT, handler);

    struct epoll_event ev, events[256]; //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    epfd = epoll_create(256); //生成用于处理accept的epoll专用的文件描述符
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(listenfd); //把socket设置为非阻塞方式
    ev.data.fd = listenfd; //设置与要处理的事件相关的文件描述符
    ev.events = EPOLLIN | EPOLLET;  //设置要处理的事件类型

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev); //注册epoll事件
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    const char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(portnumber);
    bind(listenfd, (sockaddr *) &serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
    maxi = 0;
    for (; !bExit;) {
        nfds = epoll_wait(epfd, events, 256, 500); //等待epoll事件的发生
        for (i = 0; i < nfds; ++i) { //处理所发生的所有事件
            if (events[i].data.fd == listenfd) { //如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
                connfd = accept(listenfd, (sockaddr *) &clientaddr, &clilen);
                if (connfd < 0) {
                    perror("connfd<0");
                    exit(1);
                }
                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accapt a connection from " << str << endl;
                ev.data.fd = connfd; //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET; //设置用于注册的读操作事件
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev); //注册ev

                ev.data.fd = listenfd; //设置用于监听的文件描述符
                ev.events = EPOLLIN | EPOLLET; //设置用于注册的监听事件
                epoll_ctl(epfd, EPOLL_CTL_MOD, listenfd, &ev); //注册ev
            } else if (events[i].events & EPOLLIN) { //如果是已经连接的用户，并且收到数据，那么进行读入。
                cout << "EPOLLIN" << endl;
                if ((sockfd = events[i].data.fd) < 0)
                    continue;
                if ((n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else
                        std::cout << "readline error" << std::endl;
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                line[n] = '\0';
                cout << "read " << line << endl;
                ev.data.fd = sockfd;  //设置用于写操作的文件描述符
                ev.events = EPOLLOUT | EPOLLET; //设置用于注册的写操作事件
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //修改sockfd上要处理的事件为EPOLLOUT
            } else if (events[i].events & EPOLLOUT) { // 如果有数据发送
                sockfd = events[i].data.fd;
                write(sockfd, line, n);
                ev.data.fd = sockfd; //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET; //设置用于注册的读操作事件
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);  //修改sockfd上要处理的事件为EPOLIN
            }
        }
    }
    close(epfd);
}
