#include "../../Include/EmailSend.h"
#include "../../Include/Log.h"
#include <butil/containers/hash_tables.h>
#include <iostream>
#include <gflags/gflags.h>
using namespace MicroSeedEmailSend ;

DEFINE_string(username, "", "用户名称");
DEFINE_string(password, "", "用户密码");
DEFINE_string(url, "", "邮件发送的URL");
DEFINE_string(from, "", "发送邮件的一方");
int main(int argc,char* argv[]){
    MicroSeedLog::MicroSeedLogInit();
    google::ParseCommandLineFlags(
        &argc, &argv, true);
    //聚合初始化,c++11引入的东西,可以不需要对应的构造函数
    MailSetting SettingInfo{
        FLAGS_username,
        FLAGS_password,
        FLAGS_url,
        FLAGS_from,
    };
    EmailSendClient sendclient(
        SettingInfo,
        "/home/dev/workspace/CppMicroSeed/Test/TestMicroEmailSend/MailMessage.html",
        "{{VERIFY_CODE}}"
    ) ;
    bool ret = sendclient.Send("3450014365@qq.com");
    if(ret == false){
        std::cout<<"发送验证码失败!"<<std::endl;
    }
    return 0;
}