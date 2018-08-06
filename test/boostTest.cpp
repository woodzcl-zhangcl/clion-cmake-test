// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/1.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "CMemBlock.h"
#include "boost/shared_ptr.hpp"

TEST_CASE("shared_ptr", "[BOOST]") {
    boost::shared_ptr<char> name(new char[13]);
    memset(name.get(), 0, 13);
    memcpy(name.get(), "123", 3);
    printf("%s.\n", name.get());
}
