// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/6.
//

#ifndef SDK_THPOOL_H
#define SDK_THPOOL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct thpool_ *threadpool;

threadpool thpool_init(int num_threads);

int thpool_add_work(threadpool, void (*function_p)(void *), void *arg_p);

void thpool_wait(threadpool);

void thpool_destroy(threadpool);

int thpool_num_threads_working(threadpool);

#ifdef __cplusplus
}
#endif

#endif //SDK_THPOOL_H
