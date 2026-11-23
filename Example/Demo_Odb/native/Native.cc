#include <odb/database.hxx>
#include <odb/exception.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>
#include <odb/query.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>

#include "Student.h"
#include "Student-odb.hxx"

const std::string HOST = "dev-mysql";
const std::string USER = "root";
const std::string PASS = "123456";
const std::string DBNAME = "mytest";
const unsigned int PORT = 3306;
const std::string CSET = "utf8";

void NativeSelect(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        typedef odb::query<NativeCount> Query;
        typedef odb::result<NativeCount> Result;
        //原生语句中的字段记得以数据表中的字段为准,
        //struct NativeCount中定义的那个别名,
        //不能被直接用作原生语句中,原生语句必须手动设置,并且必须一致
        Query q("select classid ,name ,score as order_field from StudentTable \
            order by order_field desc limit 5 offset 15");
            //对的，实验过了，那边没有别名这里设计别名也能跑
        Result res(db.query(q));
        for(auto it = res.begin() ; it != res.end() ; it ++){
            std::cout<<"班级ID:"<<it->classid<<"\t";
            std::cout<<"学生名称:"<<it->name<<"\t";
            std::cout<<"学生成绩:"<<it->score<<std::endl;
        }
        tx.commit();
    }catch(odb::exception& e){
        std::cout<<"ODB ERR:["<<e.what()<<"]"<<std::endl;
    }
}

int main(){
    //创建连接池
    std::unique_ptr<odb::mysql::connection_factory> pool(
        new odb::mysql::connection_pool_factory(5));
    //创建数据库连接句柄
    auto handler = std::make_unique<odb::mysql::database>(
        USER,PASS,DBNAME,
        HOST,PORT,nullptr,//额外的连接选项
        CSET,0,std::move(pool)//数据库对象持有连接池工厂，并通过它获取和管理数据库连接。
    );
    NativeSelect(handler);
    return 0;
}