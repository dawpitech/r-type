//
// EPITECH PROJECT, 2025
// r-type
// File description:
// implementations for serializations functions
//

#include "Serialization.hpp"
#include "flux.hpp"

template <typename T>
std::string flux::SerializerHandler<T>::serialize(flux::ECS& ecs, flux::Entity& entity, const T& component)
{
    std::ostringstream out;
    component.reflect(
        [&](auto&&... fields)
        {
            auto id = ecs.getComponentTypeID<T>();
            out << id << " " << entity << " ";
            ((out << fields << " "), ...);
        });
    return out.str();
}

template <typename T>
T flux::SerializerHandler<T>::unserialize(flux::ECS& ecs, flux::Entity& entity, const std::string& data)
{
    std::istringstream in(data);
    T component{};

    ComponentTypeID typeID;
    component.reflect([&](auto&&... fields) { ((in >> fields), ...); });
    return component;
}
