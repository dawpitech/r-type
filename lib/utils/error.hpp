//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Errors
//

#pragma once

#include <exception>
#include <string>
namespace utils
{
    class BaseError : public std::exception
    {
        public:
            explicit BaseError(std::string msg, std::string where) :
                _msg(std::move(msg)), _where(std::move(where)) {};
            ~BaseError() = default;

            [[nodiscard]] const char* what() const noexcept override
            {
                return this->_msg.c_str();
            }

            [[nodiscard]] const char* where() const noexcept
            {
                return this->_where.c_str();
            }

        private:
            std::string _msg;
            std::string _where;
    };
} // namespace utils
