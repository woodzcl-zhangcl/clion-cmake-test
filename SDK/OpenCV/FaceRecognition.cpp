// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/9.
//

#include "OpenCV/FaceRecognition.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//训练文件
string face_cascade_name = "/Users/woodzcl/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "人脸识别";

void detectAndDisplay(Mat frame) {
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    imshow("equalizehist----", frame_gray);
    equalizeHist(frame_gray, frame_gray);
    imshow("equalizehist", frame_gray);
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

    for (int i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
        ellipse(frame, center, Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8,
                0);
    }
    imshow(window_name, frame);
}

int recognition() {
    VideoCapture cap;
    Mat img;

    if (cap.isOpened())
        cap.release();
    cap.open(0);
    while (1) {
        while (1) {
            cap >> img;
            waitKey(40);
            if (img.data && 0 != strcmp((char *) img.data, ""))
                break;
        }
        imshow("cap_img", img);
        cvWaitKey(0);
        resize(img, img, Size(img.cols / 3, img.rows / 3));

        if (!img.data) {
            printf("[error] 没有图片\n");
            return -1;
        }

        if (!face_cascade.load(face_cascade_name)) {
            printf("[error] 无法加载级联分类器文件！\n");
            return -1;
        }

        detectAndDisplay(img);
    }
    return 0;
}