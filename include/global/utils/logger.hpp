//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Logger
//

#pragma once

#include <iostream>
#include <string>

namespace utils
{
    class Logger
    {
        public:
            static void setDebug(bool debug)
            {
                instance()._isDebugEnable = debug;
            }

            static void debug(const std::string& msg)
            {
                if (instance()._isDebugEnable)
                    std::cout << msg << std::endl;
            };

        private:
            bool _isDebugEnable = false;
            static Logger& instance()
            {
                static Logger logger;
                return logger;
            }
    };
} // namespace utils
