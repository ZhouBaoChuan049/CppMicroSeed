#include <iostream>
#include <sstream>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/common/functional.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/server.hpp>

using WebsocketServer = websocketpp::server<websocketpp::config::asio> ;
using MessagePtr = WebsocketServer::message_ptr ;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


void OnOpen(WebsocketServer* server, websocketpp::connection_hdl hdl){
    std::cout <<"连接成功"<<std::endl;
}
void OnClose(WebsocketServer* server, websocketpp::connection_hdl hdl){
    std::cout <<"连接关闭"<<std::endl;
}
void OnMessage(
    WebsocketServer* server,
    websocketpp::connection_hdl hdl,
    MessagePtr msg
){
    std::cout <<"收到消息:["<<msg->get_payload()<<"]"<<std::endl;
    server->send(
        hdl, 
        msg->get_payload(), 
        websocketpp::frame::opcode::text
    );
}
void OnFail(WebsocketServer* server, websocketpp::connection_hdl hdl){
    std::cout <<"连接异常"<<std::endl;
}
void OnHttp(WebsocketServer* server, websocketpp::connection_hdl hdl){
    std::cout<<"处理Http请求"<<std::endl;
    WebsocketServer::connection_ptr conn = server->get_con_from_hdl(hdl);
    std::stringstream ss ; 
    ss << "<!doctype html><html><head>"
        << "<title>hello websocket</title><body>"
        << "<h1>hello websocketpp</h1>"
        << "</body></head></html>";
    conn->set_body(ss.str());
    conn->set_status(websocketpp::http::status_code::ok);
}
int main(){
    // 使用websocketpp库创建服务器
    WebsocketServer server ;
    // 设置websocketpp库的日志级别.
    server.set_access_channels(websocketpp::log::alevel::none);
    // 初始化asio
    server.init_asio();
    //注册Http请求处理函数
    server.set_http_handler(bind(&OnHttp,&server,::_1));
    // 注册websocket请求的处理函数
    server.set_open_handler(bind(&OnOpen,&server,::_1));
    server.set_close_handler(bind(&OnClose,&server,::_1));
    server.set_message_handler(bind(&OnMessage, &server ,::_1,::_2));
    // 监听8888端口
    server.listen(9000);
    // 开始接收tcp连接
    server.start_accept();
    // 开始运行服务器
    server.run();
    return 0 ; 
}








