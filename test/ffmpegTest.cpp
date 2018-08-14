// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/14.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"

#ifdef __cplusplus
}
#endif

/**
 * AVFormat Support Information
 */
char *avformatinfo() {

    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);

    void* opaque = nullptr;
    const AVInputFormat *if_temp = av_demuxer_iterate(&opaque);
    //Input
    while (if_temp != nullptr) {
        sprintf(info, "%s[In] %10s\n", info, if_temp->name);
        if_temp = av_demuxer_iterate(&opaque);
    }

    opaque = nullptr;
    const AVOutputFormat *of_temp = av_muxer_iterate(&opaque);
    //Output
    while (of_temp != nullptr) {
        sprintf(info, "%s[Out] %10s\n", info, of_temp->name);
        of_temp = av_muxer_iterate(&opaque);
    }
    return info;
}

/**
 * AVCodec Support Information
 */
char *avcodecinfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);

    void *opaque = nullptr;
    const AVCodec *c_temp = av_codec_iterate(&opaque);

    while (c_temp != nullptr) {
        if (c_temp->decode != nullptr) {
            sprintf(info, "%s[Dec]", info);
        } else {
            sprintf(info, "%s[Enc]", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s[Video]", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s[Audio]", info);
                break;
            default:
                sprintf(info, "%s[Other]", info);
                break;
        }

        sprintf(info, "%s %10s\n", info, c_temp->name);

        c_temp = av_codec_iterate(&opaque);
    }
    return info;
}

/**
 * AVFilter Support Information
 */
char *avfilterinfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);

    void *opaque = nullptr;
    const AVFilter *f_temp = (AVFilter *) av_filter_iterate(&opaque);

    while (f_temp != nullptr) {
        sprintf(info, "%s[%15s]\n", info, f_temp->name);
        f_temp = (AVFilter *) av_filter_iterate(&opaque);
    }
    return info;
}

/**
 * Configuration Information
 */
char *configurationinfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);

    sprintf(info, "%s\n", avcodec_configuration());

    return info;
}

TEST_CASE("ffmpeg", "[FFMPEG]") {
    char *infostr = nullptr;
    infostr = configurationinfo();
    printf("\n<<Configuration>>\n%s", infostr);
    free(infostr);

    infostr = avformatinfo();
    printf("\n<<AVFormat>>\n%s", infostr);
    free(infostr);

    infostr = avcodecinfo();
    printf("\n<<AVCodec>>\n%s", infostr);
    free(infostr);

    infostr = avfilterinfo();
    printf("\n<<AVFilter>>\n%s", infostr);
    free(infostr);
}
