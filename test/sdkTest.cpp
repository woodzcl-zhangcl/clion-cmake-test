// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/2.
//

#include <pthread.h>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Introduce.h"
#include "Algorithm/algorithmBase.h"
#include "ThreadPool/thpool.h"

TEST_CASE("magic number", "[SDK]") {
    uint64_t magicNumber = getMagicNumber();
    printf("Magic Number is %llu.\n", magicNumber);
}

TEST_CASE("algorithmBase test", "[SDK]") {
    using namespace Algorithm;
    SECTION("LinkList") {
        int i = 100;
        std::cout << "I own " << i << " score" << std::endl;
        CMBlock a, p, q, del;
        LinkList<CMBlock> li;

        CMBlock cmBlock0(1), cmBlock1(1), cmBlock2(1), cmBlock3(1), cmBlock4(1), cmBlock5(1);
        cmBlock0[0] = 0;
        cmBlock1[0] = 1;
        cmBlock2[0] = 2;
        cmBlock3[0] = 3;
        cmBlock4[0] = 4;
        cmBlock5[0] = 5;
        li.insertElem(0, cmBlock0);
        li.insertElem(1, cmBlock1);
        li.insertElem(2, cmBlock2);
        li.insertElem(3, cmBlock3);
        li.insertElem(3, cmBlock4);
        li.insertElem(1, cmBlock5);

        std::cout << "链表长度" << li.length() << std::endl;

        std::cout << "各个元素的值是：";
        // traverse list
        for (size_t i = 0; i < li.length(); i++) {

            if (li.getElem(i, a)) {
                std::cout << (int) a[0] << " ";
            }
        }

        std::cout << std::endl;
        size_t ix = li.getElem(3, a);
        std::cout << "ix=" << ix << std::endl;

        size_t locix = li.locateElem(cmBlock2);
        std::cout << "locix=" << locix << std::endl;

        bool bF = li.priorElem(cmBlock2, p);
        if (bF) {
            std::cout << "p=" << (int) p[0] << std::endl;
        }

        bF = li.nextElem(cmBlock2, q);
        if (bF) {
            std::cout << "q=" << (int) q[0] << std::endl;
        }

        std::cout << std::endl;
        std::cout << "反转后各个元素的值是：";
        Node<CMBlock> *re_li = li.reverse();
        while (re_li) {
            std::cout << (int) re_li->_data[0] << " ";
            re_li = re_li->next;
        }
        std::cout << std::endl;

        bool bD = li.delElem(4, del);
        if (bD) {
            std::cout << "del:" << (int) del[0] << std::endl;
        }
        std::cout << std::endl;
    }
    SECTION("Queue") {
        Queue<int> q(10);
        size_t n = 20;
        for (size_t i = 0; i < n; i++)
            q.push(i + 1);
        std::cout << "pop sequence: ";
        while (!q.empty()) {
            int tmp = q.front();
            std::cout << tmp << " ";
            q.pop();
            if (!q.empty()) {
                tmp = q.front();
                q.push(tmp);
                q.pop();
            }
        }
        std::cout << std::endl << std::endl;
    }
    SECTION("BinaryTree") {
        int root = 5;
        int a = 1, b = 2, c = 3, d = 4, e = 6, f = 7;

        BinaryTree<int> bTree(root);
        bTree.insert(a);
        bTree.insert(b);
        bTree.insert(c);
        bTree.insert(d);
        bTree.insert(e);
        bTree.insert(f);
        bTree.preOrder();
        std::cout << std::endl << std::endl;
        bTree.remove(5);
        bTree.preOrder();
        std::cout << std::endl << std::endl;
    }
}

void task1(void*) {
    printf("Thread #%ld working on task1\n", (long) pthread_self());
}

void task2(void*) {
    printf("Thread #%ld working on task2\n", (long) pthread_self());
}

TEST_CASE("thread pool", "[SDK]") {
    puts("Making threadpool with 6 threads");
    threadpool thpool = thpool_init(6);

    puts("Adding 40 tasks to threadpool");
    int i;
    for (i=0; i<20; i++){
        thpool_add_work(thpool, task1, nullptr);
        thpool_add_work(thpool, task2, nullptr);
    };

    puts("Killing threadpool");
    thpool_destroy(thpool);
}