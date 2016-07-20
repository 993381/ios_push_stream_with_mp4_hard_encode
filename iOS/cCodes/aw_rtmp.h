//
//  aw_rtmp.h
//  pushStreamInC
//
//  Created by kaso on 20/7/16.
//  Copyright © 2016年 xcyo. All rights reserved.
//

#ifndef aw_rtmp_h
#define aw_rtmp_h

#include "rtmp.h"
#include "aw_alloc.h"
#include <string.h>

typedef enum aw_rtmp_state{
    aw_rtmp_state_idle,//默认情况
    aw_rtmp_state_connecting,//连接中
    aw_rtmp_state_opened,//打开
    aw_rtmp_state_closed,//关闭，发送后回到idle状态
    aw_rtmp_state_error_write,//写入失败，发送完毕回到open状态
    aw_rtmp_state_error_open,//打开失败，发送后回到idle
    
    aw_rtmp_state_error_net,//多次连接失败，网络错误
}aw_rtmp_state;

typedef void (*aw_rtmp_state_changed_cb) (aw_rtmp_state old_state, aw_rtmp_state new_state);

typedef struct aw_rtmp_context{
    char rtmp_url[256];
    RTMP *rtmp;
    
    //写入错误断线重连
    int8_t write_error_retry_time_limit;//写入错误重试次数，超过这个次数就要重连。
    int8_t write_error_retry_curr_time;//当前重试错误次数。
    
    //open错误重连
    int8_t open_error_retry_time_limit;//连接重试次数，超过这个次数。
    int8_t open_error_retry_curr_time;//当前连接重试的次数，表示连接错误。
    
    int8_t is_header_sent;//是不是发送过flv的header了
    
    //时间校正
    double last_video_time_stamp;
    double last_audio_time_stamp;
    //当前mp4文件开始时间戳
    double current_time_stamp;
    
    //总时间
    double total_duration;
    
    aw_rtmp_state_changed_cb state_changed_cb;
    aw_rtmp_state rtmp_state;
} aw_rtmp_context;

extern void aw_init_rtmp_context(aw_rtmp_context *ctx, const char *rtmp_url, aw_rtmp_state_changed_cb state_changed_cb);
extern aw_rtmp_context *alloc_aw_rtmp_context(const char *rtmp_url, aw_rtmp_state_changed_cb state_changed_cb);
extern void free_aw_rtmp_context(aw_rtmp_context **ctx);

//rtmp是否打开
extern int8_t aw_is_rtmp_opened(aw_rtmp_context *ctx);

//打开rtmp
extern int aw_rtmp_open(aw_rtmp_context *ctx);

//写入数据
extern int aw_rtmp_write(aw_rtmp_context *ctx, const char *buf, int size);

//关闭rtmp
extern int aw_rtmp_close(aw_rtmp_context *ctx);

//获取当前时间
extern uint32_t aw_rtmp_time();

#endif /* aw_rtmp_h */
