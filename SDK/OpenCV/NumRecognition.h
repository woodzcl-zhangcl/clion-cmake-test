// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/8.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

#ifndef SDK_NUMRECOGNITION_H
#define SDK_NUMRECOGNITION_H

#ifdef __cplusplus
extern "C" {
#endif

// base
void ftransfer(IplImage *image);
void ShowPic(std::string file);
void TransferPic(std::string file);

// number recognition
// ref http://www.cnblogs.com/gongxijun/p/5152099.html
struct DataBase {
    int value;        //库特征对应的值
    std::vector<cv::Mat> sample;   //特征库
    DataBase(int var, cv::Mat &sam) {
        value = var;
        sample.push_back(sam);
    }
};

typedef struct DataBase dataBase;

//加载图片
bool loadImage(cv::Mat &src, cv::Mat &gray, std::string &filename);

//二值化
bool binImage(cv::Mat &src, cv::Mat &dst, int _size, int lparam, int mediansize);

//装载特征库
/*通常来说这个应该是预先被加载好,以数据库或者其他的形式保存起来
 在这地方由于东西比较少，直接现场处理
*/
int loadProperty(std::vector<dataBase> &db, int index[], int _size = 31, int lparam = 7, int mediansize = 3);

//对于模块进行匹配
int StartMatch(cv::Mat src, std::vector<dataBase> db, cv::Point &curpx);

//逐步的扫描
std::vector<int> ScanImage(cv::Mat &src, std::vector<dataBase> db, int window_width = 12, int window_height = 12);

//逐步的扫描
std::vector<int> ScanImage(cv::Mat &src, std::vector<dataBase> db, int window_width, int window_height);

//pattern match
void recognition();

#ifdef __cplusplus
}
#endif

#endif //SDK_NUMRECOGNITION_H
