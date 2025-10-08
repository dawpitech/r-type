//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Serialize and unserialize data
//

#pragma once

#include <any>
#include <format>
#include <sstream>
#include <string>
#include <unordered_map>
#include "flux/core/flux.hpp"
#include "utils/logger.hpp"

#define REFLECT(...)                                                                                                   \
    template <typename T>                                                                                              \
    void reflect(T&& function)                                                                                         \
    {                                                                                                                  \
        f(__VA_ARGS__);                                                                                                \
    }

typedef uint32_t ComponentTypeID;

ComponentTypeID getUniqueComponentTypeID()
{
    static ComponentTypeID lastID = -1;
    lastID += 1;
    return lastID;
}

template <typename T>
ComponentTypeID getComponentTypeID()
{
    static ComponentTypeID id = getUniqueComponentTypeID();
    return id;
}

struct ComponentTypeInfo
{
        std::function<std::string(flux::Entity& entity, std::any)> serialize;
        std::function<void(flux::ECS&, flux::Entity&, const std::string& data)> unserialize;
        std::string name;
};

std::unordered_map<ComponentTypeID, ComponentTypeInfo> componentTypeRegistry;

namespace flux
{
    template <typename T>
    class SerializerHandler
    {
        public:
            static std::string serialize(flux::Entity& entity, const T& component)
            {
                std::ostringstream out;
                component.reflect(
                    [&](auto&&... fields)
                    {
                        auto id = getComponentTypeID<T>();
                        out << id << " " << entity << " ";
                        ((out << fields << " "), ...);
                    });
                return out.str();
            }

            static T unserialize(flux::ECS& ecs, flux::Entity& entity, const std::string& data)
            {
                std::istringstream in(data);
                T component{};

                ComponentTypeID typeID;
                component.reflect([&](auto&&... fields) { ((in >> fields), ...); });
                return component;
            }

            static void unserializeSingleComponent(flux::ECS& ecs, const std::string& data)
            {
                std::istringstream in(data);
                ComponentTypeID typeID;
                flux::Entity entity;

                in >> typeID >> entity;

                auto it = componentTypeRegistry.find(typeID);
                if (it == componentTypeRegistry.end())
                    utils::Logger::debug(std::format("Can't find component with id {}", typeID));
                std::string dataLeft = data.substr(in.tellg());
                it->second.unserialize(ecs, entity, dataLeft);
            }
    };
} // namespace network

template <typename T>
void registerComponentType(const std::string& name)
{
    auto id = getComponentTypeID<T>();

    componentTypeRegistry[id] = {[](flux::Entity& entity, T& component) -> std::string
                                 { return flux::SerializerHandler<T>::serialize(entity, component); },
                                 [](flux::ECS& ecs, flux::Entity& entity, const std::string& data)
                                 {
                                     T component = flux::SerializerHandler<T>::unserialize(ecs, entity, data);
                                     // ecs.AddOrReplace(entity, component); // Method should be added in the ECS
                                 },
                                 name};
}
