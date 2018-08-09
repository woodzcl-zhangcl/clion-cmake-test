// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/8.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "OpenCV/NumRecognition.h"

TEST_CASE("opencv base", "[OpenCV]") {
    SECTION("show picture") {
        ShowPic("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
    }
    SECTION("transfer") {
        TransferPic("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
    }
}

TEST_CASE("number recognition", "[OpenCV]") {
    recognition();
}