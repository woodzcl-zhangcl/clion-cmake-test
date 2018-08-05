// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/5.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 256
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

bool bExit = false;

// sigint signal
void handler(int Signal) {
    bExit = true;
}

const int kReadEvent = 1;
const int kWriteEvent = 2;

void updateEvents(int efd, int fd, int events, bool modify) {
    struct kevent ev[2];
    int n = 0;
    if (events & kReadEvent && !modify) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *) (intptr_t) fd);
    } else if (events & kReadEvent && modify) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void *) (intptr_t) fd);
    }
    if (events & kWriteEvent && !modify) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, (void *) (intptr_t) fd);
    } else if (events & kWriteEvent && modify) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void *) (intptr_t) fd);
    }
    int r = kevent(efd, ev, n, nullptr, 0, nullptr);
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

TEST_CASE("test", "[MAC Server]") {
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds, portnumber = 9091;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;

    // capture sigint
    if (SIG_ERR == signal(SIGINT, handler)) {
        exit(1);
    }

    struct kevent activeEvs[256];
    epfd = kqueue();
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(listenfd);

    updateEvents(epfd, listenfd, kReadEvent, false); //注册epoll事件
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    const char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(portnumber);
    bind(listenfd, (sockaddr *) &serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
    maxi = 0;
    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 500 * 1000 * 1000;
    for (; !bExit;) {
        nfds = kevent(epfd, nullptr, 0, activeEvs, 256, &timeout);
        for (i = 0; i < nfds; ++i) { //处理所发生的所有事件
            int fd = (int) (intptr_t) activeEvs[i].udata;
            int events = activeEvs[i].filter;
            if (fd == listenfd) {
                connfd = accept(listenfd, (sockaddr *) &clientaddr, &clilen);
                if (connfd < 0) {
                    perror("connfd<0");
                    exit(1);
                }
                char *str = inet_ntoa(clientaddr.sin_addr);
                std::cout << "accapt a connection from " << str << std::endl;
                updateEvents(epfd, connfd, kReadEvent, false); //注册ev
            } else if (events == EVFILT_READ) {
                if ((sockfd = fd) < 0)
                    continue;
                if ((n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                    } else
                        std::cout << "readline error" << std::endl;
                } else if (n == 0) {
                    close(sockfd);
                }
                line[n] = '\0';
                std::cout << line << std::endl;
                updateEvents(epfd, sockfd, kReadEvent, true); //注销读取事件监听
                updateEvents(epfd, sockfd, kWriteEvent, false); //修改sockfd上要处理的事件为EPOLLOUT
            } else if (events == EVFILT_WRITE) {
                sockfd = fd;
                write(sockfd, line, n);
                updateEvents(epfd, sockfd, kWriteEvent, true); //注销写入事件监听
                updateEvents(epfd, sockfd, kReadEvent, false); //修改sockfd上要处理的事件为EPOLIN
            }
        }
    }
    close(epfd);
}