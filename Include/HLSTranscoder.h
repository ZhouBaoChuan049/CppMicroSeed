#pragma once
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
    #include <libavutil/dict.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/error.h>
}
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../Include/Log.h"
const std::string HLS_EXTM3U = "#EXTM3U";
const std::string HLS_VERSION = "#EXT-X-VERSION:";
const std::string HLS_TARGETDURATION = "#EXT-X-TARGETDURATION:";
const std::string HLS_SEQUENCE = "#EXT-X-MEDIA-SEQUENCE:";
const std::string HLS_PLAYLIST_TYPE = "#EXT-X-PLAYLIST-TYPE:";
const std::string HLS_INDEPENDENT_SEGMENTS = "#EXT-X-INDEPENDENT-SEGMENTS";
const std::string HLS_ENDLIST = "#EXT-X-ENDLIST";
const std::string HLS_EXTINF = "#EXTINF:";

namespace MicroSeedHLS
{
    class M3U8FileUtil
    {
    public:
        using M3U8Ptr = std::shared_ptr<M3U8FileUtil> ;
        M3U8FileUtil(std::string infilename, std::string outfilename)
            :_InputM3U8filename(infilename),
            _OutputM3U8filename(outfilename){}
        bool Parse();
        bool Write();
        std::vector<std::pair<std::string, std::string>> GetSegments();
        std::vector<std::string> GetHeaderInfos();
    private:
        std::string _InputM3U8filename ;
        std::string _OutputM3U8filename ;
        std::vector<std::string> _HeaderInfos ;
        std::vector<std::pair<std::string, std::string>> _SegmentInfo ;
    };
    struct SettingInfo
    {
        std::string HLS_TIME;
        std::string HLS_BASE_URL;
        std::string HLS_LIST_SIZE;
        std::string HLS_PLAYLIST_TYPE;
        std::string HLS_FLAGS;
    };
    class HLSTranscoder
    {
    public:
        using TranserPtr = std::shared_ptr<HLSTranscoder> ;
        HLSTranscoder(SettingInfo setting)
            :_setting(setting)
        {}
        bool TranscodeToHLS(std::string infile, std::string outfile);
    private:
        char* CheckError(int errnum);
        SettingInfo _setting ;
    };
}
