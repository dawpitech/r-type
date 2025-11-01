//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Init the database connection
//

#include "Database.hpp"
#include "utils/logger.hpp"
#ifdef _WIN32
    #include <winsqlite/winsqlite3.h>
#else
    #include <sqlite3.h>
#endif

Server::Database::Database()
{
    auto res = sqlite3_open("data.db", &this->_db);

    if (res != SQLITE_OK || this->_db == nullptr) {
        throw DatabaseError("Error while loading database", "Database");
    }
    utils::Logger::debug("Database loaded successfuly");
    this->_createTable();
}

Server::Database::~Database()
{
    if (this->_db != nullptr) {
        sqlite3_close(this->_db);
    }
}
