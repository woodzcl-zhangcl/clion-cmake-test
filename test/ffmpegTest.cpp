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

#define TEST_1 0

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#ifdef __cplusplus
};
#endif
#endif

#define TEST_2 1

/**
 * AVFormat Support Information
 */
char *avformatinfo() {

    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);

    void *opaque = nullptr;
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
    if (1 == TEST_1) {
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
}

/**
 * video stream
 */


TEST_CASE("stream", "[FFMPEG]") {
    if (0 == TEST_2)
        return;

    AVOutputFormat *ofmt = nullptr;
    //Input AVFormatContext and Output AVFormatContext
    AVFormatContext *ifmt_ctx = nullptr, *ofmt_ctx = nullptr;
    AVPacket pkt;
    const char *in_filename, *out_filename;
    int ret, i;
    int videoindex = -1;
    int frame_index = 0;
    int64_t start_time = 0;
    //in_filename  = "cuc_ieschool.mov";
    //in_filename  = "cuc_ieschool.mkv";
    //in_filename  = "cuc_ieschool.ts";
    //in_filename  = "cuc_ieschool.mp4";
    //in_filename  = "cuc_ieschool.h264";
    in_filename = "cuc_ieschool.flv";//输入URL（Input file URL）
    //in_filename  = "shanghai03_p.h264";

    out_filename = "rtmp://localhost/publishlive/livestream";//输出 URL（Output URL）[RTMP]
    //out_filename = "rtp://233.233.233.233:6666";//输出 URL（Output URL）[UDP]

    //Network
    avformat_network_init();
    //Input
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        printf("Could not open input file.");
        goto end;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        printf("Failed to retrieve input stream information");
        goto end;
    }

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        if (ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

    //Output

    avformat_alloc_output_context2(&ofmt_ctx, nullptr, "flv", out_filename); //RTMP
    //avformat_alloc_output_context2(&ofmt_ctx, nullptr, "mpegts", out_filename);//UDP

    if (!ofmt_ctx) {
        printf("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    ofmt = ofmt_ctx->oformat;
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        //Create output AVStream according to input AVStream
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVCodec *codec = avcodec_find_decoder(in_stream->codecpar->codec_id);
        AVStream *out_stream = avformat_new_stream(ofmt_ctx, codec);
        if (!out_stream) {
            printf("Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }
        //Copy the settings of AVCodecContext
        out_stream->codecpar = in_stream->codecpar;
        out_stream->codecpar->codec_tag = 0;
        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
//            out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
    }
    //Dump Format------------------
    av_dump_format(ofmt_ctx, 0, out_filename, 1);
    //Open output URL
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            printf("Could not open output URL '%s'", out_filename);
            goto end;
        }
    }
    //Write file header
    ret = avformat_write_header(ofmt_ctx, nullptr);
    if (ret < 0) {
        printf("Error occurred when opening output URL\n");
        goto end;
    }

    start_time = av_gettime();
    while (1) {
        AVStream *in_stream, *out_stream;
        //Get an AVPacket
        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;
        //FIX：No PTS (Example: Raw H.264)
        //Simple Write PTS
        if (pkt.pts == AV_NOPTS_VALUE) {
            //Write PTS
            AVRational time_base1 = ifmt_ctx->streams[videoindex]->time_base;
            //Duration between 2 frames (us)
            int64_t calc_duration = (double) AV_TIME_BASE / av_q2d(ifmt_ctx->streams[videoindex]->r_frame_rate);
            //Parameters
            pkt.pts = (double) (frame_index * calc_duration) / (double) (av_q2d(time_base1) * AV_TIME_BASE);
            pkt.dts = pkt.pts;
            pkt.duration = (double) calc_duration / (double) (av_q2d(time_base1) * AV_TIME_BASE);
        }
        //Important:Delay
        if (pkt.stream_index == videoindex) {
            AVRational time_base = ifmt_ctx->streams[videoindex]->time_base;
            AVRational time_base_q = {1, AV_TIME_BASE};
            int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
            int64_t now_time = av_gettime() - start_time;
            if (pts_time > now_time)
                av_usleep(pts_time - now_time);

        }

        in_stream = ifmt_ctx->streams[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];
        /* copy packet */
        //Convert PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        //Print to Screen
        if (pkt.stream_index == videoindex) {
            printf("Send %8d video frames to output URL\n", frame_index);
            frame_index++;
        }
        //ret = av_write_frame(ofmt_ctx, &pkt);
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);

        if (ret < 0) {
            printf("Error muxing packet\n");
            break;
        }

        av_packet_unref(&pkt);

    }
    //Write file trailer
    av_write_trailer(ofmt_ctx);

    end:

    avformat_close_input(&ifmt_ctx);
    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);
    if (ret < 0 && ret != AVERROR_EOF) {
        printf("Error occurred.\n");
        return;
    }
}