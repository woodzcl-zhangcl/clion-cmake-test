// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/8.
//

#include "OpenCV/NumRecognition.h"

void ftransfer(IplImage *image) {
    // Create some windows to show the input
    // and output images in.
    cvNamedWindow("2_4-in", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("2_4-out", CV_WINDOW_AUTOSIZE);

    // Create a window to show our input image
    cvShowImage("2_4-in", image);


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
    cvShowImage("2_4-out", out);

    // Be tidy
    cvReleaseImage(&out);

    // Wait for the user to hit a key, then clean up the windows
    cvWaitKey(0);
    cvDestroyWindow("2_4-in");
    cvDestroyWindow("2_4-out");
}

void ShowPic(std::string file) {
    cv::Mat srcImage = cv::imread(file.c_str());
    imshow("[img]", srcImage);
    cv::waitKey(0);
}

void TransferPic(std::string file) {
    IplImage *img = cvLoadImage(file.c_str());
    cvNamedWindow("1", CV_WINDOW_AUTOSIZE);
    cvShowImage("1", img);
    ftransfer(img);
    cvReleaseImage(&img);
    cvDestroyWindow("1");
}

//加载图片
bool loadImage(cv::Mat &src, cv::Mat &gray, cv::String &filename) {
    cv::Mat cbgray; //合成后图像
    int chans;    //bgR分量
    src = cv::imread(filename, true);
    if (src.empty()) return false;
    chans = src.channels();
    std::vector<cv::Mat> bgR(chans);
    //分割通道
    split(src, bgR);
    //直方图均衡化
    for (int chan = 0; chan < chans; ++chan) {
        equalizeHist(bgR[chan], bgR[chan]);
    }
    //单通道合并
    merge(bgR, cbgray);
    //灰度化
    cvtColor(cbgray, gray, CV_RGB2GRAY);
    return true;
}

//二值化
bool binImage(cv::Mat &src, cv::Mat &dst, int _size, int lparam, int mediansize) {
    //采用自适应二值化
    adaptiveThreshold(src, src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, _size, lparam);
    //中值滤波
    medianBlur(src, dst, mediansize);
    return true;
}

//装载特征库
/*通常来说这个应该是预先被加载好,以数据库或者其他的形式保存起来
 在这地方由于东西比较少，直接现场处理
*/
int loadProperty(std::vector<dataBase> &db, int index[], int _size, int lparam, int mediansize) {
    //固定路径
    char filename[256];
    for (int i = 0; i < 1; ++i) {
        sprintf(filename, "/Users/woodzcl/Documents/图片/OpenCV_Test_Pic/%d.png", index[i]);
        cv::Mat tmp;
        cv::String path = filename;
        loadImage(tmp, tmp, path); //装载并灰度化
        binImage(tmp, tmp, _size, lparam, mediansize); //二值化
        db.push_back(dataBase(index[i] % 10, tmp));
    }
    return true;
}

//对于模块进行匹配
int StartMatch(cv::Mat src, std::vector<dataBase> db, cv::Point &curpx) {
    int res;
    double maxValue, minValue, resValue;
    cv::Point minloc, maxloc, resloc;

    std::vector<dataBase>::iterator it;
    std::vector<cv::Mat>::iterator m_it;
    cv::Mat sample, result;
    int curcols, currows;
    resValue = 1.;

    for (it = db.begin(); it != db.end(); it++) {
        for (m_it = it->sample.begin(); m_it != it->sample.end(); m_it++) {
            sample = *m_it;
            int res_rows = src.rows - sample.rows + 1;
            int res_cols = src.cols - sample.cols + 1;
            if (res_rows < 1 || res_cols < 1) break;
            result = cv::cvarrToMat(cvCreateImage(cvSize(res_cols, res_rows), 1, 1));
            matchTemplate(src, sample, result, CV_TM_SQDIFF_NORMED);  //模板匹配算法,平方差匹配
            minMaxLoc(result, &minValue, &maxValue, &minloc, &maxloc, cv::Mat());
            if (resValue > minValue) {
                resValue = minValue;
                resloc = minloc;
                res = it->value; //记录这个值的大小
                curcols = sample.cols;
                currows = sample.rows;
            }
        }
    }
    if (resValue < 0.2) {
        curpx.x += resloc.x + curcols / 2.;
        rectangle(src, resloc, cvPoint(resloc.x + curcols, resloc.y + currows), cvScalar(0, 0, 255));
    } else {
        ++curpx.x;
        res = -1;
    }
    return res;
}

//逐步的扫描
std::vector<int> ScanImage(cv::Mat &src, std::vector<dataBase> db, int window_width, int window_height) {
    cv::Point srcp;

    window_height = src.rows;
    std::vector<int> ans;
    while (srcp.x < src.cols) {
        if (srcp.x + window_width > src.cols)
            window_width = src.cols - srcp.x;
        cv::Mat tmp = src(cv::Rect(srcp.x, srcp.y, window_width, window_height));

        //轮廓检测
        /* vector< vector <Point> >reg;
        Mat newtmp = tmp.clone();
        findContours(newtmp, reg,CV_RETR_EXTERNAL , CV_CHAIN_APPROX_NONE);
        if( reg.empty() ) break;
        Rect rect = boundingRect(Mat(reg[0]));
        Mat ttmp = tmp(rect);
        imshow("ttmp",ttmp);
        waitKey(0);*/

        int ansvalue = StartMatch(tmp, db, srcp);
        if (ansvalue != -1) {
            ans.push_back(ansvalue);
            printf("%d ", ansvalue);
        }
    }
    puts("");
    return ans;
}

void recognition() {
    cv::Mat check;

    //装载特征库
    std::vector<dataBase> dblist;
    int dex[20];//{0,1,2,3,4,5,6,7,8,9}; //建立一个索引
    for (int i = 0; i < 20; dex[i] = i, i++);
    loadProperty(dblist, dex, 7, 33, 3);

    //待匹配目标源
    char path[256];
    sprintf(path, "/Users/woodzcl/Documents/图片/OpenCV_Test_Pic/src.png");

    //装载目标源
    cv::String s_path(path);
    loadImage(check, check, s_path);
    imshow("check", check);
    cvWaitKey(0);

    //二值化目标源
    binImage(check, check, 17, 50, 3);

    //获得匹配结果
    std::vector<int> vec_result = ScanImage(check, dblist, 11, 3);

    imshow("final", check);
    cvWaitKey(0);
}