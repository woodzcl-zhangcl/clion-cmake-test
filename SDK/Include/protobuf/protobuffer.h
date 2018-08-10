// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/10.
//

#ifndef SDK_PROTOBUFFER_H
#define SDK_PROTOBUFFER_H

#include "protobuf/addressbook.pb.h"

#ifdef __cplusplus
extern "C" {
#endif

int proto_read_write_tofile(const char* file);

#ifdef __cplusplus
}
#endif

#endif //SDK_PROTOBUFFER_H
