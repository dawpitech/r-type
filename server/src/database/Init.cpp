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
    this->_createTable();
    // this->update("PLAYERS", "UUID", "'123H33'", "SCORE", "256");
    // this->update("PLAYERS", "UUID", "'123-AB6'", "SCORE", "768");
    // this->update("PLAYERS", "UUID", "'123H33'", "SCORE", "8");
}

Server::Database::~Database()
{
    if (this->_db != nullptr) {
        sqlite3_close(this->_db);
    }
}
