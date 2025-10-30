//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Handle the main table for the datas
//

#include "Database.hpp"
#include <format>
#include <iostream>
#include <sqlite3.h>

void Server::Database::_createTable()
{
    if (this->_db == nullptr)
        throw DatabaseError("Database not initialized", "create table");
    std::string query = "CREATE TABLE PLAYERS("
                        "UUID TEXT PRIMARY KEY NOT NULL, "
                        "USERNAME TEXT NOT NULL, "
                        "SURNAME TEXT NOT NULL, "
                        "SCORE INT NOT NULL); ";

    char *errorMessage = nullptr;
    auto res = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (res != SQLITE_OK) {
        std::string errorStr = errorMessage ? errorMessage : "Unknown error";
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        if (errorStr.find("already exists") != errorStr.npos)
            return;
        throw DatabaseError(std::format("Can't create new database: {}", errorStr), "create table");
    }
}
