//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Serialize and unserialize data
//

#pragma once

#include <cstdint>
#include <sstream>
#include <string>

#define REFLECT(...)                                                                                                   \
    template <typename T>                                                                                              \
    void reflect(T&& function)                                                                                         \
    {                                                                                                                  \
        f(__VA_ARGS__);                                                                                                \
    }

namespace flux
{
    typedef std::uint32_t Entity;
    typedef uint32_t ComponentTypeID;

    class ECS;

    template <typename T>
    class SerializerHandler
    {
        public:
            static std::string serialize(flux::ECS& ecs, flux::Entity& entity, const T& component);

            static T unserialize(flux::ECS& ecs, flux::Entity& entity, const std::string& data);
    };
} // namespace flux
