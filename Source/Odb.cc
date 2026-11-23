#include "../Include/Odb.h"


namespace MicroSeedODB{
    std::unique_ptr<odb::mysql::database> 
        ODBFactory::CreateOdbMysqlObject(MysqlSettings& settings){
            std::unique_ptr<odb::mysql::connection_factory> pool(
                new odb::mysql::connection_pool_factory(settings.connection_pool_size));
                auto handler = std::make_unique<odb::mysql::database>(
                    settings.user,
                    settings.passwd,
                    settings.db,
                    settings.host,
                    settings.port,
                    nullptr,
                    settings.cset,
                    0,
                    std::move(pool)
                );
                return handler;
        }
}
