// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/8.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "OpenCV/NumRecognition.h"
#include "OpenCV/ui_cvui.h"

#define TEST_BASE 0
#define TEST_RECOGNITION 0
#define TEST_CVUI1 0
#define TEST_CVUI2 1


TEST_CASE("opencv base", "[OpenCV]") {
    SECTION("show picture") {
        if (1 == TEST_BASE) {
            ShowPic("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
        }
    }
    SECTION("transfer") {
        if (1 == TEST_BASE) {
            TransferPic("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
        }
    }
}

TEST_CASE("number recognition", "[OpenCV]") {
    if (1 == TEST_RECOGNITION) {
        recognition();
    }
}

TEST_CASE("cvui layer", "[CVUI]") {
    if (1 == TEST_CVUI1) {
        ShowUI();
    }
}

TEST_CASE("cvui canny", "[CVUI]") {
    if (1 == TEST_CVUI2) {
        ShowCanny();
    }
}