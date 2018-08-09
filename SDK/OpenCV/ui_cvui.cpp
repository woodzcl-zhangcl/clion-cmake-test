// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/9.
//

#include "OpenCV/ui_cvui.h"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define CVUI_IMPLEMENTATION

#include "cvui.h"

#define WINDOW_NAME_GROUP    "Complex layout"
#define WINDOW_NAME_CANNY    "CVUI Canny Edge"

void group(cv::Mat &frame, int x, int y, int width, int height) {
    int padding = 5, w = (width - padding) / 4, h = (height - 15 - padding) / 2;
    cv::Point pos(x + padding, y + 5);

    cvui::text(frame, pos.x, pos.y, "Group title");
    pos.y += 15;

    cvui::window(frame, pos.x, pos.y, width - padding * 2, h - padding, "Something");
    cvui::rect(frame, pos.x + 2, pos.y + 20, width - padding * 2 - 5, h - padding - 20, 0xff0000);
    pos.y += h;

    cvui::window(frame, pos.x, pos.y, w / 3 - padding, h, "Some");
    cvui::text(frame, pos.x + 25, pos.y + 60, "65", 1.1);
    pos.x += w / 3;

    cvui::window(frame, pos.x, pos.y, w / 3 - padding, h, "Info");
    cvui::text(frame, pos.x + 25, pos.y + 60, "30", 1.1);
    pos.x += w / 3;

    cvui::window(frame, pos.x, pos.y, w / 3 - padding, h, "Here");
    cvui::text(frame, pos.x + 25, pos.y + 60, "70", 1.1);
    pos.x += w / 3;

    cvui::window(frame, pos.x, pos.y, w - padding, h, "And");
    cvui::rect(frame, pos.x + 2, pos.y + 22, w - padding - 5, h - padding - 20, 0xff0000);
    pos.x += w;

    cvui::window(frame, pos.x, pos.y, w - padding, h, "Here");
    cvui::rect(frame, pos.x + 2, pos.y + 22, w - padding - 5, h - padding - 20, 0xff0000);
    pos.x += w;

    cvui::window(frame, pos.x, pos.y, w - padding, h, "More info");
    cvui::rect(frame, pos.x + 2, pos.y + 22, w - padding - 5, h - padding - 20, 0xff0000);
    pos.x += w;
}

void ShowUI() {
    int height = 220, spacing = 10;
    cv::Mat frame = cv::Mat(height * 3, 1300, CV_8UC3);

    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME_GROUP);

    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(49, 52, 49);

        // Render three groups of components.
        group(frame, 0, 0, frame.cols, height - spacing);
        group(frame, 0, height, frame.cols, height - spacing);
        group(frame, 0, height * 2, frame.cols, height - spacing);

        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();

        // Show everything on the screen
        cv::imshow(WINDOW_NAME_GROUP, frame);

        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

void ShowCanny() {
    cv::Mat lena = cv::imread("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
    cv::Mat frame = lena.clone();
    int low_threshold = 50, high_threshold = 150;
    bool use_canny = false;

    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME_CANNY);

    while (true) {
        // Should we apply Canny edge?
        if (use_canny) {
            // Yes, we should apply it.
            cv::cvtColor(lena, frame, CV_BGR2GRAY);
            cv::Canny(frame, frame, low_threshold, high_threshold, 3);
            cv::cvtColor(frame, frame, CV_GRAY2BGR);
        } else {
            // No, so just copy the original image to the displaying frame.
            lena.copyTo(frame);
        }

        // Render the settings window to house the checkbox
        // and the trackbars below.
        cvui::window(frame, 10, 50, 180, 180, "Settings");

        // Checkbox to enable/disable the use of Canny edge
        cvui::checkbox(frame, 15, 80, "Use Canny Edge", &use_canny);

        // Two trackbars to control the low and high threshold values
        // for the Canny edge algorithm.
        cvui::trackbar(frame, 15, 110, 165, &low_threshold, 5, 150);
        cvui::trackbar(frame, 15, 180, 165, &high_threshold, 80, 300);

        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();

        // Show everything on the screen
        cv::imshow(WINDOW_NAME_CANNY, frame);

        // Check if ESC was pressed
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
}