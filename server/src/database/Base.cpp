//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Handle the main table for the datas
//

#include "Database.hpp"
#include "utils/logger.hpp"
#include <format>
#ifdef _WIN32
    #include <winsqlite/winsqlite3.h>
#else
    #include <sqlite3.h>
#endif

void Server::Database::update(const std::string &table, const std::string &primaryKeyName,
    const std::string &primaryKeyValue, const std::string &field, const std::string &value)
{
    if (this->_db == nullptr)
        throw DatabaseError("Database not initialized", "update table");
    std::string query = std::format("INSERT INTO {} ({}, {}) VALUES ({}, {})"
                                    "ON CONFLICT({}) DO UPDATE SET {} = {}",
        table, field, primaryKeyName, value, primaryKeyValue, primaryKeyName, field, value);

    char *errorMessage = nullptr;
    auto res = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (res != SQLITE_OK) {
        std::string errorStr = errorMessage ? errorMessage : "Unknown error";
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        utils::Logger::debug(std::format("Can't update database: {}", errorStr));
    }
}

int Server::Database::selectInt(const std::string &table, const std::string &primaryKeyName,
    const std::string &primaryKeyValue, const std::string &field)
{
    if (this->_db == nullptr)
        throw DatabaseError("Database not initialized", "select int");
    auto query =
        std::format("SELECT {} FROM {} WHERE {} = ?;", field, table, primaryKeyName, primaryKeyValue);

    sqlite3_stmt *statement = nullptr;
    int res = sqlite3_prepare_v2(this->_db, query.c_str(), -1, &statement, nullptr);
    if (res != SQLITE_OK) {
        throw DatabaseError(sqlite3_errmsg(this->_db), "select int");
    }
    sqlite3_bind_text(statement, 1, primaryKeyValue.c_str(), -1, SQLITE_TRANSIENT);
    res = sqlite3_step(statement);
    
    if (res != SQLITE_ROW) {
        sqlite3_finalize(statement);
        return 0;
    }

    int result = sqlite3_column_int(statement, 0);
    sqlite3_finalize(statement);
    return result;
}

void Server::Database::_createTable()
{
    if (this->_db == nullptr)
        throw DatabaseError("Database not initialized", "create table");
    std::string query = "CREATE TABLE PLAYERS("
                        "USERPASS TEXT PRIMARY KEY NOT NULL, "
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
