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

        void update(const std::string &table, const std::string &primaryKeyName,
            const std::string &primaryKeyValue, const std::string &field, const std::string &value);

       private:
        sqlite3 *_db = nullptr;

        void _createTable();
    };
}  // namespace Server
