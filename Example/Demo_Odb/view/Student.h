#pragma once
#include <string>
#include <cstddef> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/nullable.hxx>
#include <odb/core.hxx>
//这是在声明一个 ODB 持久化对象,并指定它在 MySQL 中对应的数据表名称.
//声明的表的字段正式我接下来这个类的成员.
#pragma db object table("StudentTable")
class Student {
    public:
        Student() {}
        Student(const std::string &name): _name(name) {}

        size_t sn() const { return _sn; }
        void set_sn(size_t sn) { _sn = sn; }

        size_t classid() const {return _classid; }
        void set_classid(const size_t& classid){ _classid = classid;}

        const std::string &name() const { return _name; }
        void set_name(const std::string &name) { _name = name; }

        odb::nullable<int> age() const { return _age; }
        void set_age(odb::nullable<int> age) { _age = age; }

        odb::nullable<double> score() const { return _score; }
        void set_score(odb::nullable<double> score) { _score = score; }

        odb::nullable<boost::posix_time::ptime> birthday() const { return _birthday; }
        void set_birthday(const boost::posix_time::ptime &birthday) { _birthday = birthday; }
    private:
        //ODB 在生成数据库映射代码时，需要访问你的 C++ 类中的成员变量.
        friend class odb::access;
        #pragma db id auto
        size_t _sn;
        #pragma db index
        size_t _classid;
        std::string _name;
        //odb::nullable<int>是ODB的一种语法,用来标记某个字段可以为空
        odb::nullable<int> _age;
        odb::nullable<double> _score;
        //boost::posix_time::ptime 是 Boost.Date_Time 库提供的时间类型
        //ptime:表示日期与时间的类型(Point in Time)
        odb::nullable<boost::posix_time::ptime> _birthday;
};

#pragma db object table("ClassTable")
class Classes{
    public:
        ~Classes(){}
        Classes() = default;
        Classes(std::string name):_name(name){}
        size_t classid(){return _classid;}
        std::string name(){return _name;}
        void set_name(std::string name){_name = name;}
    private:
        friend class odb::access;
        #pragma db id auto // id = primary key auto = auto increment
        size_t _classid;
        #pragma db unique type("VARCHAR(32)") 
        std::string _name;
};


// 这是在创建视图。
//  为什么要创建两个视图？前者的语义为：查询某个班级的学生，后者的语义为查询某个学生所在之班级
// 过滤条件： Query::Classes::name == "一年级一班"  
#pragma db view object(Classes) \
                object(Student : Student::_classid == Classes::_classid) \
                query((?))
            //两个对象。然后两个对象做笛卡尔积，要求条件为Student::_classid == Classes::_classid
            //我还可以提供一个查询条件。？是占位符号
//表示我们查询出来的东西应当由什么组成：
struct ClassesStudent{
    std::shared_ptr<Student> student;
    std::shared_ptr<Classes> classes;
};

// 过滤条件： Query::Student::name == "zhangsan"  
#pragma db view object(Student) \
                object(Classes : Student::_classid == Classes::_classid) \
                query((?))
struct DetailStudent {
    //声明查询结果会有两列，每列之元素来源于此二类对象之成员
    std::shared_ptr<Student> student;
    std::shared_ptr<Classes> classes;
};