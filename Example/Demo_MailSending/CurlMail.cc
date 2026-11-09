#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <sstream>

size_t callback(char* buffer, size_t size , size_t nitems , void* userdata){
    std::stringstream* ss = (std::stringstream*)userdata;
    ss->read(buffer,size* nitems);
    return ss->gcount();
}

int main(){
    //假定发送方:3450014365@qq.com
    //假定接收方:MrKonata@163.com
    const std::string username = "MrKonata@163.com";
    const std::string password = "RDTadqbrb2T3YZZG";
    const std::string url = "smtps://smtp.163.com:465";
    const std::string from = "MrKonata@163.com";
    const std::string to = "3450014365@qq.com";
    //初始化全局配置
    auto ret = curl_global_init(CURL_GLOBAL_DEFAULT);
    if(ret != CURLE_OK){
        std::cout<<"curl_global_init fail!"<<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    //构造和获取操作句柄
    auto curlkey = curl_easy_init();
    if(curlkey == nullptr){
        std::cout<<"curl_easy_init fail"<<std::endl;
        return -1;
    }
    ret = curl_easy_setopt(curlkey, CURLOPT_URL, url.c_str());
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_URL fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    ret = curl_easy_setopt(curlkey, CURLOPT_USERNAME, username.c_str());
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_USERNAME fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    ret = curl_easy_setopt(curlkey, CURLOPT_PASSWORD, password.c_str());
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_PASSWORD fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    ret = curl_easy_setopt(curlkey, CURLOPT_MAIL_FROM, from.c_str());
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_MAIL_FROM fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    struct curl_slist* cs = nullptr;
    cs = curl_slist_append(cs,to.c_str());
    ret = curl_easy_setopt(curlkey, CURLOPT_MAIL_RCPT, cs);
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_MAIL_RCPT fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    std::stringstream ss;
    ss << "Subject: 登录验证码\r\n";
    ss << "MIME-Version: 1.0\r\n";
    ss << "Content-Type: text/html; charset=UTF-8\r\n";
    ss << "\r\n";
    ss << "<!DOCTYPE html>";
    ss << "<html><body style='font-family:Arial,sans-serif;background:#fff;'>";
    ss << "<div style='max-width:600px;margin:40px auto;padding:30px;'>";
    ss << "<h1 style='color:#000;font-size:36px;'>示例服务</h1>";
    ss << "<p style='font-size:20px;color:#333;margin-top:60px;'>"
        "输入此临时验证码以继续：</p>";
    ss << "<div style='background:#f2f2f2;border-radius:24px;"
        "padding:35px 40px;margin:40px 0;'>";
    ss << "<span style='font-size:40px;letter-spacing:5px;color:#555;'>"
    << "454709"
    << "</span>";
    ss << "</div>";
    ss << "<p style='font-size:18px;color:#333;'>"
        "如果你没有尝试登录，请忽略此邮件。</p>";
    ss << "<p style='color:#999;margin-top:70px;'>"
        "谨致问候<br>示例服务团队</p>";
    ss << "<hr style='border:0;border-top:1px solid #ddd;margin-top:70px;'>";
    ss << "<p style='color:#888;font-size:13px;'>"
        "示例服务 · 帮助中心</p>";
    ss << "</div></body></html>\r\n";
    ret = curl_easy_setopt(curlkey, CURLOPT_READDATA, (void*)&ss);
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_READDATA fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    ret = curl_easy_setopt(curlkey, CURLOPT_READFUNCTION, callback);
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_READFUNCTION fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    ret = curl_easy_setopt(curlkey, CURLOPT_UPLOAD, 1L);
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_setopt CURLOPT_UPLOAD fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    ret = curl_easy_perform(curlkey);
    if(ret != CURLE_OK){
        std::cout<<"curl_easy_perform fail!"
            <<curl_easy_strerror(ret)<<std::endl;
        return -1;
    }
    curl_slist_free_all(cs);
    curl_easy_cleanup(curlkey);
    curl_global_cleanup();
    return 0 ;
}