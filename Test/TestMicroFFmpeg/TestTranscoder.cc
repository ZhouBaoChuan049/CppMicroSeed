#include "../../Include/Util.h"
#include "../../Include/Log.h"
#include "../../Include/HLSTranscoder.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace MicroSeedHLS;
using namespace MicroSeedLog;
using namespace MicroSeedUtil;

void TestTranscoder(){
    SettingInfo setting;
    setting.HLS_TIME = "5";
    setting.HLS_BASE_URL = "/video/";
    setting.HLS_LIST_SIZE = "0";
    setting.HLS_PLAYLIST_TYPE = "vod";
    setting.HLS_FLAGS = "independent_segments";
    HLSTranscoder transcoder(setting);
    if(transcoder.TranscodeToHLS("Test.mp4", "Output.m3u8"))
        INF("转码成功!");
    else
        ERR("转码失败!");
}

void TestM3U8(){
    M3U8FileUtil util("Output.m3u8", "Output2.m3u8");
    if(!util.Parse()){
        ERR("解析M3U8失败!");
        return ;
    }
    INF("分片数量:{}", util.GetSegments().size());
    INF("头部信息数量:{}", util.GetHeaderInfos().size());
    if(!util.Write())
        ERR("写回M3U8失败!");
    else
        INF("写回M3U8成功!");
}

int main()
{
    MicroSeedLog::MicroSeedLogInit();
    TestTranscoder();
    TestM3U8();
    return 0 ;
}