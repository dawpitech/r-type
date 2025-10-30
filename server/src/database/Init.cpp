//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Init the database connection
//

#include "Database.hpp"
#include "utils/logger.hpp"
#include <sqlite3.h>

Server::Database::Database()
{
    auto res = sqlite3_open("data.db", &this->_db);

    if (res != SQLITE_OK || this->_db == nullptr) {
        throw DatabaseError("Error while loading database", "Database");
    }
    utils::Logger::debug("Database loaded successfuly");
}

Server::Database::~Database()
{
    if (this->_db != nullptr) {
        sqlite3_close(this->_db);
    }
}
