// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/10.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Qt/qt.h"

TEST_CASE("qt", "[Qt]") {
    sax2_parse("sax2_xmlfile.xml");
}