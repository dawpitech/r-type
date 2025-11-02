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

        [[nodiscard]] std::vector<LuaSystemCPPRepr>& getLuaSystems() {
            return this->_luaSystems;
        }

    private:
		std::unique_ptr<sol::state> _luaContext = std::make_unique<sol::state>();
        std::vector<LuaSystemCPPRepr> _luaSystems = {};

        LuaContextStore() = default;
        ~LuaContextStore() = default;
};
