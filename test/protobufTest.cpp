// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/10.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "protobuf/protobuffer.h"

TEST_CASE("protobuf", "[protobuf]") {
   int ret = proto_read_write_tofile("proto_file.dat");
}
