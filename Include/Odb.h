#include <odb/database.hxx>
#include <odb/exception.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>
#include <odb/query.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>

namespace MicroSeedODB{
    struct MysqlSettings {
        std::string host = "dev-mysql";
        std::string user = "root";
        std::string passwd = "123456";
        std::string db;
        std::string cset = "utf8";
        unsigned int port = 3306;
        unsigned int connection_pool_size = 3;
    };
    class ODBFactory{
    public: 
        static std::unique_ptr<odb::mysql::database> 
            CreateOdbMysqlObject(MysqlSettings& settings);
    };
}