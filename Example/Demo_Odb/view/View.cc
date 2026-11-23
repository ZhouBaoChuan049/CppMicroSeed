#include <odb/database.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>

#include "Student.h"
#include "Student-odb.hxx"

const std::string HOST = "dev-mysql";
const std::string USER = "root";
const std::string PASS = "123456";
const std::string DBNAME = "mytest";
const unsigned int PORT = 3306;
const std::string CSET = "utf8";


void GetStudentFromClasses(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        typedef odb::query<ClassesStudent> Query;
        typedef odb::result<ClassesStudent> Result;
        Result res(db.query<ClassesStudent>(Query::Classes::name == "Grade 1 Class 1"));
        for(auto it = res.begin() ; it != res.end() ; it++){
            if(it->classes){
                std::cout<<"班级:"<<it->classes->name() << "\t";
            }
            if(it->student){
                std::cout<<"姓名:"<<it->student->name() << std::endl;
            }
        }
        tx.commit();
    }catch(odb::exception& e){
        std::cout<<"ODB ERR:["<<e.what()<<"]"<<std::endl;
    }
}
void GetClassesFromStudent(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        typedef odb::query<DetailStudent> Query;
        typedef odb::result<DetailStudent> Result;
        Result res(db.query<DetailStudent>(Query::Student::name == "Emily Johnson"));
        for(auto it = res.begin() ; it != res.end() ; it ++){
            std::cout<<"班级:"<<it->classes->name()<<std::endl;
            std::cout<<"姓名:"<<it->student->name()<<std::endl;
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
    GetStudentFromClasses(handler);
    GetClassesFromStudent(handler);
    return 0;
}