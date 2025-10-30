//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class for database
//

#pragma once

#include "utils/error.hpp"
#include <memory>
#include <sqlite3.h>

namespace Server {

    class DatabaseError : public utils::BaseError {
       public:
        DatabaseError(const std::string &what, const std::string &where) : BaseError(what, where) {}
    };

    class Database {
       public:
        Database();
        ~Database();

       private:
        sqlite3 *_db = nullptr;
    };
}  // namespace Server
