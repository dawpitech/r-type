/*
** EPITECH PROJECT, 2025
** flux
** File description:
** flux.hpp
*/

#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace flux
{
    constexpr std::size_t MAX_COMPONENTS = 64;
    constexpr std::size_t PAGE_SIZE = 1000;

    typedef uint32_t Entity;
    typedef std::bitset<MAX_COMPONENTS> ComponentMask;
    typedef ComponentMask View;

    class ECS
    {
        struct IComponentVector
        {
            virtual ~IComponentVector() = default;
        };

        std::unordered_map<std::type_index, std::size_t> componentMaskOffsetStore;
        std::size_t nextComponentMaskOffsetBit = 0;
        std::vector<ComponentMask> entitiesComponentMask;
        std::unordered_map<ComponentMask, std::vector<Entity>> componentMaskGroups;

        std::unordered_map<std::type_index, std::unique_ptr<IComponentVector>> componentsStore;

        Entity nextEntityID = 0;

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

            void Remove(const Entity id)
            {
                if (const auto it = std::find(entityIDS.begin(), entityIDS.end(), id); it != entityIDS.end()) {
                    size_t idx = std::distance(entityIDS.begin(), it);
                    entityIDS.erase(entityIDS.begin() + static_cast<long>(idx));
                    data.erase(data.begin() + idx);
                }
            }
        };

        template<typename T>
        std::size_t getComponentMaskOffset() const
        {
            const auto it = this->componentMaskOffsetStore.find(typeid(T));
            assert(it != this->componentMaskOffsetStore.end());
            return it->second;
        }

        template<typename T>
        std::size_t getOrRegisterComponentMaskOffset()
        {
            const auto it = this->componentMaskOffsetStore.find(typeid(T));
            if (it == this->componentMaskOffsetStore.end()) {
                assert(this->nextComponentMaskOffsetBit < MAX_COMPONENTS);
                this->componentMaskOffsetStore[typeid(T)] = this->nextComponentMaskOffsetBit++;
                return this->nextComponentMaskOffsetBit - 1;
            }
            return it->second;
        }

        void updateComponentMaskGroup(const Entity entity, const ComponentMask& oldMask, const ComponentMask& newMask)
        {
            if (componentMaskGroups.count(oldMask)) {
                auto& group = componentMaskGroups[oldMask];
                group.erase(std::remove(group.begin(), group.end(), entity), group.end());
                if (group.empty())
                    componentMaskGroups.erase(oldMask);
            }
            if (auto& newGroup = componentMaskGroups[newMask];
                std::find(newGroup.begin(), newGroup.end(), entity) == newGroup.end()) {
                newGroup.push_back(entity);
            }
        }

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

            explicit ECS()
            {
                this->entitiesComponentMask.resize(10 * PAGE_SIZE);
            }

            Entity newEntity()
            {
                const Entity id = nextEntityID++;
                if (entitiesComponentMask.size() <= id)
                    entitiesComponentMask.resize(id + PAGE_SIZE);
                componentMaskGroups[entitiesComponentMask[id]].push_back(id);
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

                if (this->entitiesComponentMask.size() <= entity)
                    this->entitiesComponentMask.resize(entity + PAGE_SIZE);

                const ComponentMask oldMask = entitiesComponentMask[entity];
                const std::size_t bit = this->getOrRegisterComponentMaskOffset<Component>();
                entitiesComponentMask[entity].set(bit);
                const ComponentMask newMask = entitiesComponentMask[entity];

                updateComponentMaskGroup(entity, oldMask, newMask);
            }

            template <typename Component>
            void Add(Entity entity) {
                auto& storePtr = this->componentsStore[typeid(Component)];
                if (!storePtr)
                    storePtr = std::make_unique<ComponentVector<Component>>();
                auto* store = static_cast<ComponentVector<Component>*>(storePtr.get());
                store->Add(entity);

                if (this->entitiesComponentMask.size() <= entity)
                    this->entitiesComponentMask.resize(entity + PAGE_SIZE);

                const ComponentMask oldMask = entitiesComponentMask[entity];
                const std::size_t bit = this->getOrRegisterComponentMaskOffset<Component>();
                entitiesComponentMask[entity].set(bit);
                const ComponentMask newMask = entitiesComponentMask[entity];

                updateComponentMaskGroup(entity, oldMask, newMask);
            }

            template <typename Component>
            void Remove(Entity entity)
            {
                const auto componentStore = this->componentsStore.find(typeid(Component));
                if (componentStore == this->componentsStore.end())
                    return;

                auto* store = static_cast<ComponentVector<Component>*>(componentStore->second.get());
                store->Remove(entity);

                if (entitiesComponentMask.size() > entity) {
                    const ComponentMask oldMask = entitiesComponentMask[entity];
                    const std::size_t bit = this->getComponentMaskOffset<Component>();
                    entitiesComponentMask[entity].reset(bit);
                    const ComponentMask newMask = entitiesComponentMask[entity];
                    updateComponentMaskGroup(entity, oldMask, newMask);
                }
            }

            template <typename Component>
            bool HasComponent(const Entity& entity) const
            {
                if (this->componentMaskOffsetStore.find(typeid(Component)) == this->componentMaskOffsetStore.end())
                    return false;
                if (entity >= entitiesComponentMask.size())
                    return false;
                return this->entitiesComponentMask[entity].test(this->getComponentMaskOffset<Component>());
            }

            template <typename... Components>
            bool HasComponents(const Entity& entity) const
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

            template <typename... Components>
            View GetComponentMaskFor() const
            {
                ComponentMask mask;
                (mask.set(this->getComponentMaskOffset<Components>()), ...);
                return mask;
            }

            const std::vector<Entity>& QueryView(const View& mask) const
            {
                static const std::vector<Entity> empty;
                const auto it = componentMaskGroups.find(mask);
                if (it == componentMaskGroups.end()) return empty;
                return it->second;
            }

            std::vector<Entity> QueryViewNotExclusive(const View& mask) const
            {
                std::vector<Entity> result;
                for (const auto& [groupMask, entities] : componentMaskGroups) {
                    if ((groupMask & mask) == mask) {
                        result.insert(result.end(), entities.begin(), entities.end());
                    }
                }
                return result;
            }
    };
}
