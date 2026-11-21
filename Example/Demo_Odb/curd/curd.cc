#include <boost/date_time/posix_time/time_parsers.hpp>
#include <odb/database.hxx>
#include <odb/exception.hxx>
#include <odb/mysql/connection.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>
#include <odb/query.hxx>
#include <odb/transaction.hxx>

#include "Student.h"
#include "Student-odb.hxx"

const std::string HOST = "dev-mysql";
const std::string USER = "root";
const std::string PASS = "123456";
const std::string DBNAME = "mytest";
const unsigned int PORT = 3306;
const std::string CSET = "utf8";

void Insert(std::unique_ptr<odb::mysql::database>& handler){
    try{
        //通过句柄构造事务对象
        odb::transaction tx(handler->begin());
        //通过事务对象构造数据库操作句柄
        auto& db = tx.database();
        //开始操作
        Student stu("Mr.Josn");
        stu.set_age(199);
        stu.set_birthday(boost::posix_time::time_from_string ("2035-02-22 12:45:56"));
        db.persist(stu);
        tx.commit();
    }catch(odb::exception& e){
        std::cout<<"ODB ERR:["<<e.what()<<"]"<<std::endl;
    }
}
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
void Update(std::unique_ptr<odb::mysql::database> &handler) {
    try {
        odb::transaction tx(handler->begin());
        auto &db = tx.database();
        typedef odb::query<Student> Query;
        auto stu = db.query_one<Student>(Query::sn == 2);
        if (!stu) {
            std::cout << "not found" << std::endl;
            return;
        }//你必须查询到这个数据,你才能修改这个数据
        stu->set_score(99.99);
        db.update(stu);
        //4. 提交事务
        tx.commit();
    }catch (odb::exception &e) {
        std::cout << "odb error: " << e.what() << std::endl;
    }
}
void Select_all(std::unique_ptr<odb::mysql::database> &handler) {
    try {
        odb::transaction tx(handler->begin());
        auto &db = tx.database();
        typedef odb::query<Student> Query;
        typedef odb::result<Student> Result;
        Result ret(db.query<Student>());
        for(auto it = ret.begin(); it != ret.end(); it ++){
            std::cout << "sn: " << it->sn() << std::endl;
            std::cout << "name: " << it->name() << std::endl;
            if (it->age()) std::cout << "age: " << it->age().get() << std::endl;
            if (it->score()) std::cout << "score: " << it->score().get() << std::endl;
            if (it->birthday()) std::cout << "birthday: " << boost::posix_time::to_simple_string(it->birthday().get()) << std::endl;
        }
        tx.commit();
    }catch (odb::exception &e) {
        std::cout << "odb error: " << e.what() << std::endl;
    }
}
void Remove(std::unique_ptr<odb::mysql::database> &handler) {
    try {
        odb::transaction tx(handler->begin());
        auto &db = tx.database();
        typedef odb::query<Student> Query;
        //就是根据 WHERE 条件删除符合条件的记录
        db.erase_query<Student>(Query::sn == 1);
        //4. 提交事务
        tx.commit();
    }catch (odb::exception &e) {
        std::cout << "odb error: " << e.what() << std::endl;
    }
}
int main(){
    //创建连接池
    std::unique_ptr<odb::mysql::connection_factory> pool(
        new odb::mysql::connection_pool_factory(5));
    //创建数据库连接句柄
    auto handler = std::make_unique<odb::mysql::database>(
        USER,
        PASS,
        DBNAME,
        HOST,
        PORT,
        nullptr,//额外的连接选项
        CSET,
        0,
        std::move(pool)//数据库对象持有连接池工厂，并通过它获取和管理数据库连接。
    );
    Insert(handler);
    Select(handler);
    Update(handler);
    Select_all(handler);
    Remove(handler);
    return 0;
}