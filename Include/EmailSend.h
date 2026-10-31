#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <openssl/rand.h>
#include "Util.h"
namespace MicroSeedEmailSend{
    struct MailSetting{
        const std::string Username ;
        const std::string Password ;
        const std::string Url ;
        const std::string From ;
    };
    //经过多方考究,出于安全考量,
    //我们不打算使用C语言的Rand()%x这种简单方式来生成验证码,
    //缺乏密码学相关知识,我们让GPT帮我生成一个科学的验证码生成器
    class VerificationCodeGenerator {
    public:
        static bool Generate(std::string& code);
    };
    class CurlHandle{
    public:
        //为了防止使用者忘记释放句柄,我单独封装一个句柄获取器
        CurlHandle();
        CURL * GetCurlHandle();
        ~CurlHandle();
    private:
        CURL* _curl ;
    };
    class BaseClient{
    public:
        BaseClient() = default;
        ~BaseClient() = default;
        virtual bool Send(std::string dest) = 0;
    };
    class EmailSendClient : public BaseClient{
    public:
        EmailSendClient(MailSetting setting, 
            std::string filename, 
            std::string placeholder = "{{CODE}}"
        );
        ~EmailSendClient();
        bool Send(std::string dest);
    private:
        std::string MessageBody();
        static bool ErrProcess(std::string ErrMessage, CURLcode code);
        static size_t Callback(char* buffer, size_t size , size_t nitems , void* userdata);
        //这个参数配置接口要讲一下:
        //返回值:成功返回收件人链表(供 Send 在 perform 后释放),失败返回 nullptr
        struct curl_slist* Configure(CURL* curl, 
            const std::string& dest);
        MailSetting _setting;
        std::stringstream _stream;   //持有正文回调用
        std::string _filename; //文件路径设置的时候建议设置成绝对路径,不然找不到
        std::string _placeholder; 
        //用户应该把他想要发出去的邮件放在一个文件里面,然后告诉我文件的名称
    };
}