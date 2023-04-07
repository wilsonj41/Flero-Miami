#ifndef FLEROPP_DB_H
#define FLEROPP_DB_H

#include <memory>
#include "IDatabaseDriver.hpp"

namespace fleropp::db {
    inline thread_local std::shared_ptr<IDatabaseDriver> db_handle{nullptr};
};

#endif