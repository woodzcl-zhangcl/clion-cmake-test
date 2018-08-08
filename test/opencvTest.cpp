// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/8.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void ftransfer(IplImage *image) {
    // Create some windows to show the input
    // and output images in.
    //
    cvNamedWindow("Example2_4-in", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Example2_4-out", CV_WINDOW_AUTOSIZE);

    // Create a window to show our input image
    //
    cvShowImage("Example2_4-in", image);


    // 分配自己的图像结构空间用来存储平滑处理后的图像
    // 第一个参数说明了当前图像结构的大小
    // 第二个参数告诉了我们各通道每个像素点的数据类型
    // 最后一个参数说明了通道的总数。
    IplImage *out = cvCreateImage(
            cvGetSize(image),
            IPL_DEPTH_8U,
            3
    );

    // 我们通过使用每个像素周围3*3区域进行高斯平滑处理
    cvSmooth(image, out, CV_GAUSSIAN, 5, 5);
    // 输入与输出可以是相同的，这将会使我们的程序更加有效。
    cvSmooth(out, out, CV_GAUSSIAN, 5, 5);

    // Show the smoothed image in the output window
    //
    cvShowImage("Example2_4-out", out);

    // Be tidy
    //
    cvReleaseImage(&out);

    // Wait for the user to hit a key, then clean up the windows
    //
    cvWaitKey(0);
    cvDestroyWindow("Example2_4-in");
    cvDestroyWindow("Example2_4-out");

}

TEST_CASE("opencv", "[OpenCV]") {
    SECTION("show picture") {
        Mat srcImage = imread("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
        imshow("[img]", srcImage);
        waitKey(0);
    }
    SECTION("transfer") {
        IplImage *img = cvLoadImage("/Users/woodzcl/Documents/图片/泰国巴逸上将.png");
        cvNamedWindow("1", CV_WINDOW_AUTOSIZE);
        cvShowImage("1", img);
        ftransfer(img);
        cvReleaseImage(&img);
        cvDestroyWindow("1");
    }
}
