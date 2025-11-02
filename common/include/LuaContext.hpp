/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LuaContextStore.hpp
*/

#pragma once

#include <memory>
#include <sol/sol.hpp>

class LuaContextStore {
    public:
        struct LuaSystemCPPRepr {
            sol::function fn;
            std::vector<std::string> componentsRequired;
        };

        static LuaContextStore& getInstance() {
            static LuaContextStore instance;
            return instance;
        }

        LuaContextStore(const LuaContextStore&) = delete;
        LuaContextStore& operator=(const LuaContextStore&) = delete;

        [[nodiscard]] sol::state* getLuaContext() const {
			return this->_luaContext.get();
        }

        [[nodiscard]] const std::vector<LuaSystemCPPRepr>& getLuaSystems() const {
            return this->_luaSystems;
        }

        [[nodiscard]] std::vector<LuaSystemCPPRepr>& getLuaSystemsMutable() {
            return this->_luaSystems;
        }

        [[nodiscard]] bool isInitialized() const {
            return this->_initialized;
        }

        void setInitialized(bool initialized) {
            this->_initialized = initialized;
        }

    private:
		std::unique_ptr<sol::state> _luaContext = std::make_unique<sol::state>();
        std::vector<LuaSystemCPPRepr> _luaSystems = {};
        bool _initialized = false;

        LuaContextStore() = default;
        ~LuaContextStore() = default;
};
