#include <cstddef>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
#include <libavutil/mathematics.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include "../../Include/Log.h"
#include <vector>
char* CheckError(int errnum){
    static char ErrBuffer[64] ;
    int ret = av_strerror(
        errnum, 
        ErrBuffer, 
        sizeof(ErrBuffer)
    );
    if(ret != 0) 
        ERR("错误处理函数发生未知错误!");
    return ErrBuffer ;
}
int main()
{
    //1. 打开输入文件，创建输入格式化上下文对象
    AVFormatContext* InputContext = new AVFormatContext();
    int ret = avformat_open_input(
        &InputContext,
        "https://127.0.0.1:9000/video/Test.mp4",
        nullptr,
        nullptr
    );
    if(ret < 0){
        char* errstring = CheckError(ret);
        ERR("创建输入格式化上下文对象发生错误,错误详情:{}",errstring);
        return -1;
    }
    //2. 通过输入格式化上下文对象解析视频文件元信息
    ret = avformat_find_stream_info(
        InputContext,/*就是说，我们把视频文件里面的元信息
        读取到这个格式化对象里面是吧*/
        nullptr
    );
    if(ret < 0){
        char* errstring = CheckError(ret);
        ERR("通过输入格式化上下文对象解析视频文件元信息发生错误,错误详情:{}",errstring);
        return -1 ;
    }
    //3. 申请创建输出格式化上下文对象，并且设定输出格式 hls
    AVFormatContext* OutputContext = new AVFormatContext();
    std::string FormatName = "hls" ;
    std::string FileName = "Test.m3u8" ;
    ret = avformat_alloc_output_context2(
        &OutputContext,
        nullptr,
        FormatName.c_str(),
        FileName.c_str()
    );
    //4. 遍历输入格式化上下文中的媒体流信息，为输出格式化上下文对象创建媒体流，并复制编解码器参数
    for(int i = 0 ; i < InputContext->nb_streams; i++){
        AVStream * InputAvstream = InputContext->streams[i];
        AVStream * OutputAvstream = avformat_new_stream(OutputContext, nullptr);
        if(OutputAvstream == nullptr){
            ERR("为输出格式化上下文对象创建媒体流时出现异常错误!出错时遍历到{}",i);
            return -1;
        }
        int check = avcodec_parameters_copy(
            OutputAvstream->codecpar,
            InputAvstream->codecpar
        );
        if(check < 0){
            char* errstring = CheckError(ret);
            ERR("复制编解码参数时发生错误,错误详情:{}",errstring);
            return -1 ;
        }
        //     AVRational time_base;               //时间基（时间戳单位），如 {1, 1000} 表示毫秒
        //     AVRational avg_frame_rate;          //平均帧率（如 {25, 1} 表示 25 FPS）
        //     AVRational r_frame_rate;            //标称帧率用于关键帧间隔计算。
        OutputAvstream->time_base = InputAvstream->time_base ;
        OutputAvstream->avg_frame_rate = InputAvstream->avg_frame_rate ;
        OutputAvstream->r_frame_rate = InputAvstream->r_frame_rate ;
    }
    //5. 设置HLS转码的各项细节参数：播放类型-点播vod，分片时间， 路径前缀 /video/
    AVDictionary * HeadInfo ;
    int ret1 = av_dict_set(&HeadInfo,"hls_time","5",0);
    int ret2 = av_dict_set(&HeadInfo,"hls_base_url","./",0);
    int ret3 = av_dict_set(&HeadInfo,"hls_list_size","0",0);
    int ret4 = av_dict_set(&HeadInfo,"hls_playlist_type","vod",0);
    std::vector<int> ErrCheck = {ret1,ret2,ret3,ret4};
    for(auto e : ErrCheck)
        if(e < 0 ){
            char* errstring = CheckError(ret);
            ERR("设置HLS转码的各项细节参数时发生错误,错误详情:{}",errstring);
            return -1 ;
        }
    //6. 通过输出格式化上下文，向输出文件写入头部信息
    ret = avformat_write_header(
        OutputContext,
        &HeadInfo
    );
    if(ret < 0){
        char* errstring = CheckError(ret);
        ERR("通过输出格式化上下文，向输出文件写入头部信息时发生错误,错误详情:{}",errstring);
        return -1 ;
    }
    //7. 遍历输入格式化上下文中的数据帧，
    AVPacket* PackAge ;
    while(av_read_frame(InputContext,  PackAge) >= 0){
        AVStream* InputAVStream = InputContext->streams[PackAge->stream_index];
        AVStream* OutputAVStream = OutputContext->streams[PackAge->stream_index];
        if(PackAge->pts == AV_NOPTS_VALUE){
            PackAge->pts = av_rescale_q(0,AV_TIME_BASE_Q, InputAVStream->time_base);
            PackAge->dts = PackAge->pts ;
        }
        av_packet_rescale_ts(
            PackAge, 
            InputAVStream->time_base, 
            OutputAVStream->time_base
        );
        ret = av_interleaved_write_frame(OutputContext, PackAge);
        if(ret < 0){
            char* errstring = CheckError(ret);
            ERR("将数据帧通过输出格式化上下文对象，写入输出文件中时发生错误,错误详情:{}",errstring);
            return -1 ;
        }
        av_packet_unref(PackAge);
    }
    //8. 向输出文件写入文件尾部信息
    ret = av_write_trailer(OutputContext);
    if(ret < 0){
        char* errstring = CheckError(ret);
        ERR("向输出文件写入文件尾部信息时发生错误,错误详情:{}",errstring);
        return -1 ;
    }
    //9. 转码完成，释放资源：参数字典对象，输入格式化上下文对象，输出格式化上下文对象
    av_dict_free(&HeadInfo);
    avformat_free_context(OutputContext);
    avformat_close_input(&InputContext);
    return 0 ;
}