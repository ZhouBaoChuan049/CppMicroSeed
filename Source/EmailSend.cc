#include "../Include/EmailSend.h"
#include <algorithm>
#include <sstream>
namespace MicroSeedEmailSend{
    bool VerificationCodeGenerator::Generate(std::string& code) {
        unsigned int random_num = 0;
        // 生成安全随机数:
        // 把 random_num 的内存地址交给 OpenSSL,让 OpenSSL 往这块内存里写入随机数据.
        if (RAND_bytes(reinterpret_cast<unsigned char*>(&random_num),
            sizeof(random_num)) != 1) return false;
        // 转换成六位验证码
        char buffer[7] = {0};
        std::snprintf(
            buffer,
            sizeof(buffer),
            "%06u",
            /*
            格式如下:
                位数不足时,前面用 0 补齐,
                最小宽度为 6 位,
                按照无符号整数进行格式化
            */
            random_num % 1000000 //0 ~ 999999
        );
        code = buffer;
        return true;
    }
    CurlHandle::CurlHandle(){
        _curl = curl_easy_init();
        if(_curl == nullptr)
            std::cout<<"Curl句柄创建失败!"<<std::endl;
    }
    CURL * CurlHandle::GetCurlHandle(){ return _curl ;}
    CurlHandle::~CurlHandle(){ curl_easy_cleanup(_curl);}

    bool EmailSendClient::ErrProcess(std::string ErrMessage, CURLcode code){
        if(code != CURLE_OK){
            std::cout<<ErrMessage
                <<curl_easy_strerror(code)<<std::endl;
            return false;
        }
        return true;
    }
    EmailSendClient::EmailSendClient(
        MailSetting setting,
        std::string filename,
        std::string placeholder)
        :_setting(setting),
        _filename(filename),
        _placeholder(placeholder){
        auto ret = curl_global_init(CURL_GLOBAL_DEFAULT);
        bool Answer = EmailSendClient::ErrProcess("初始化全局配置失败!",ret);
    }
    EmailSendClient::~EmailSendClient(){
        curl_global_cleanup();
    }
    size_t EmailSendClient::Callback(char* buffer, size_t size, size_t nitems, void* userdata){
        std::stringstream* ss = (std::stringstream*)userdata;
        ss->read(buffer, size * nitems);
        return ss->gcount();
    }

    std::string EmailSendClient::MessageBody(){
        std::stringstream messagebody;
        MicroSeedUtil::FileUtil util;
        std::string buffer ;
        bool ret = util.ReadFile(_filename, buffer);
        if(ret == false)
            return "";
        auto Anspos = buffer.find(_placeholder);
        if (Anspos == std::string::npos)
            return "";
        std::string code ;
        VerificationCodeGenerator::Generate(code);
        buffer.replace(Anspos, _placeholder.size(), code);
        std::stringstream ss ;
        ss << "Subject: 验证码\r\n";
        ss << "Content-Type: text/html\r\n";
        ss << "\r\n";
        ss << buffer;
        return ss.str() ;
    }
    //参数配置接口
    struct curl_slist* EmailSendClient::Configure(
        CURL* curl, 
        const std::string& dest
    ){
        //收件人链表
        
        struct curl_slist* recipients = nullptr;
        recipients = curl_slist_append(recipients, dest.c_str());
        _stream.str(MessageBody());
        //把传入的字符串替换掉流里的全部内容，并把读写位置重置到开头.
        if(!ErrProcess("设置服务器地址失败!",       curl_easy_setopt(curl, CURLOPT_URL,               _setting.Url.c_str())))        return nullptr;
        if(!ErrProcess("设置发件人失败!",           curl_easy_setopt(curl, CURLOPT_MAIL_FROM,        _setting.From.c_str())))       return nullptr;
        if(!ErrProcess("设置收件人链表失败!",       curl_easy_setopt(curl, CURLOPT_MAIL_RCPT,         recipients)))                  return nullptr;
        if(!ErrProcess("设置读数据回调失败!",       curl_easy_setopt(curl, CURLOPT_READFUNCTION,      &EmailSendClient::Callback)))  return nullptr;
        if(!ErrProcess("设置正文内容失败!",         curl_easy_setopt(curl, CURLOPT_READDATA,          &_stream)))                    return nullptr;
        if(!ErrProcess("启用证书验证失败!",         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,    0L)))                          return nullptr;
        if(!ErrProcess("设置登录用户名失败!",       curl_easy_setopt(curl, CURLOPT_USERNAME,          _setting.Username.c_str())))   return nullptr;
        if(!ErrProcess("设置登录密码失败!",         curl_easy_setopt(curl, CURLOPT_PASSWORD,          _setting.Password.c_str())))   return nullptr;
        if(!ErrProcess("设置SSL/TLS加密失败!",     curl_easy_setopt(curl, CURLOPT_USE_SSL,           CURLUSESSL_ALL)))              return nullptr;
        if(!ErrProcess("启用上传模式失败!",         curl_easy_setopt(curl, CURLOPT_UPLOAD,            1L)))                          return nullptr;
        return recipients;
    }
    bool EmailSendClient::Send(std::string dest){
        CurlHandle handle;
        CURL*  curl  = handle.GetCurlHandle();
        struct curl_slist* recipients = Configure(curl, dest);
        if(recipients == nullptr)
            return false;
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(recipients);
        return ErrProcess("发送邮件失败!", res);
    }
}