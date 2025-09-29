/*
** EPITECH PROJECT, 2025
** flux
** File description:
** flux.hpp
*/

#pragma once

#include <algorithm>
#include <bitset>
#include <memory>
#include <cstdint>
#include <queue>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace flux
{
    typedef uint32_t Entity;

    constexpr size_t MAX_COMPONENTS = 64;

    class ECS
    {
        private:
            struct IComponentVector
            {
                virtual ~IComponentVector() = default;
            };

            template <typename T>
            struct ComponentVector final : IComponentVector
            {
                std::vector<T> data;
                std::vector<Entity> entityIDS;

                void Add(const Entity id, T&& value)
                {
                    entityIDS.push_back(id);
                    data.push_back(std::move(value));
                }

                void Add(const Entity id, const T& value)
                {
                    entityIDS.push_back(id);
                    data.push_back(value);
                }

                void Add(const Entity id)
                {
                    entityIDS.push_back(id);
                    data.push_back(T{});
                }
            };

            typedef std::bitset<MAX_COMPONENTS> ComponentMask;

            std::tuple<> _componentsType;

            Entity nextEntityID = 0;

            std::unordered_map<std::type_index, std::unique_ptr<IComponentVector>> componentsStore;

        public:
            // ReSharper disable CppClassCanBeFinal
            class FluxException : std::runtime_error
            {
                public:
                    explicit FluxException(const std::string& err) : std::runtime_error(err) {}
            };
            class InvalidComponentQuery : FluxException
            {
                public:
                    explicit InvalidComponentQuery() : FluxException("Component not yet registered cannot be used as query") {}
            };
            class NoComponentFoundPanic : FluxException
            {
                public:
                    explicit NoComponentFoundPanic() : FluxException("Component is not present is the given entity") {}
            };
            // ReSharper restore CppClassCanBeFinal

            Entity newEntity()
            {
                const Entity id = nextEntityID++;
                return id;
            }

            template <typename Component, typename... Args>
            void Add(Entity entity, Args&&... args)
            {
                auto& storePtr = this->componentsStore[typeid(Component)];
                if (storePtr == nullptr)
                    storePtr = std::make_unique<ComponentVector<Component>>();
                auto* store = static_cast<ComponentVector<Component>*>(storePtr.get());
                store->Add(entity, Component{std::forward<Args>(args)...});
            }

            template <typename Component>
            void Add(Entity entity) {
                auto& storePtr = this->componentsStore[typeid(Component)];
                if (!storePtr)
                    storePtr = std::make_unique<ComponentVector<Component>>();
                auto* store = static_cast<ComponentVector<Component>*>(storePtr.get());
                store->Add(entity);
            }

            template <typename Component>
            bool HasComponent(Entity entity)
            {
                const auto componentStore = this->componentsStore.find(typeid(Component));
                if (componentStore == this->componentsStore.end())
                    return false;
                auto* store = static_cast<ComponentVector<Component>*>(componentStore->second.get());
                return std::find(store->entityIDS.begin(), store->entityIDS.end(), entity) != store->entityIDS.end();
            }

            template <typename... Components>
            bool HasComponents(const Entity entity)
            {
                return (this->HasComponent<Components>(entity) && ...);
            }

            template <typename Component>
            Component& GetComponent(const Entity entity)
            {
                const auto componentStore = this->componentsStore.find(typeid(Component));
                if (componentStore == this->componentsStore.end())
                    throw InvalidComponentQuery();

                auto* store = static_cast<ComponentVector<Component>*>(componentStore->second.get());
                auto it = std::find(store->entityIDS.begin(), store->entityIDS.end(), entity);

                if (it == store->entityIDS.end())
                    throw NoComponentFoundPanic();

                size_t idx = std::distance(store->entityIDS.begin(), it);
                return store->data[idx];
            }
    };
}
