#include "../../Include/Odb.h"
#include <iostream>

#include "Student.h"
#include "Student-odb.hxx"
using namespace MicroSeedODB;
void Select(std::unique_ptr<odb::mysql::database>& handler){
    try{
        //通过句柄构造事务对象
        odb::transaction tx(handler->begin());
        //通过事务对象构造数据库操作句柄
        auto& db = tx.database();
        //开始操作
        typedef odb::query<Student> QueryStu;
        //理解为 Student 对象对应的 WHERE 子句构造器
        auto stu = db.query_one<Student>(QueryStu::sn == 1);
        //用WHERE字句进行查询,查到了,就获取查询结果字段的句柄
        if(!stu){
            std::cout<<"没有找到sn == 1的数据哦!"<<std::endl;
            return;
        }
        //利用这个句柄,对这段数据进行查询
        std::cout << "sn: " << stu->sn() << std::endl;
        std::cout << "name: " << stu->name() << std::endl;
        if (stu->age()) std::cout << "age: " << stu->age().get() << std::endl;
        if (stu->score()) std::cout << "score: " << stu->score().get() << std::endl;
        if (stu->birthday()) std::cout << "birthday: " << 
            boost::posix_time::to_simple_string(stu->birthday().get()) << std::endl;
        //提交事务
        tx.commit();
    }catch(odb::exception& e){
        std::cout<<"ODB ERR:["<<e.what()<<"]"<<std::endl;
    }
}
        std::string host = "dev-mysql";
        std::string user = "root";
        std::string passwd = "123456";
        std::string db;
        std::string cset = "utf8";
        unsigned int port = 3306;
        unsigned int connection_pool_size = 3;

int main(){
    MysqlSettings settings{ .db = "mytest"};
    auto Handler = ODBFactory::CreateOdbMysqlObject(settings);
    Select(Handler);
    return 0;
}