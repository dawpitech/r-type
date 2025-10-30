//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Handle the main table for the datas
//

#include "Database.hpp"
#include "utils/logger.hpp"
#include <format>
#include <sqlite3.h>

void Server::Database::update(const std::string &table, const std::string &primaryKeyName,
    const std::string &primariKeyValue, const std::string &field, const std::string &value)
{
    if (this->_db == nullptr)
        throw DatabaseError("Database not initialized", "update table");
    std::string query = std::format("INSERT INTO {} ({}, {}) VALUES ({}, {})"
                                    "ON CONFLICT({}) DO UPDATE SET {} = {}",
                                    table, field, primaryKeyName, value, primariKeyValue,
                                    primaryKeyName, field, value);


    char *errorMessage = nullptr;
    auto res = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (res != SQLITE_OK) {
        std::string errorStr = errorMessage ? errorMessage : "Unknown error";
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        utils::Logger::debug(std::format("Can't create new database: {}", errorStr));
    }
}

void Server::Database::_createTable()
{
    if (this->_db == nullptr)
        throw DatabaseError("Database not initialized", "create table");
    std::string query = "CREATE TABLE PLAYERS("
                        "UUID TEXT PRIMARY KEY NOT NULL, "
                        "USERNAME TEXT, "
                        "PASSWORD TEXT, "
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
