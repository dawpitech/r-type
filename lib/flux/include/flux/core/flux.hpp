/*
** EPITECH PROJECT, 2025
** flux
** File description:
** flux.hpp
*/

#pragma once

#include <algorithm>
#include <any>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <thread>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "flux/core/Serialization.hpp"
#include "utils/logger.hpp"

namespace flux
{
    constexpr std::size_t MAX_COMPONENTS = 64;
    constexpr std::size_t PAGE_SIZE = 1000;
    constexpr std::size_t DEFAULT_INIT_PAGE_MULTIPLIER = 10;

    typedef uint32_t Entity;
    typedef std::bitset<MAX_COMPONENTS> ComponentMask;
    typedef ComponentMask View;
    typedef uint32_t ComponentTypeID;

    struct ComponentTypeInfo
    {
        std::function<std::string(Entity& entity, std::any)> serialize;
        std::function<void(Entity&, const std::string& data)> unserialize;
        std::string name;
    };

    enum class systemType
    {
        LOGIC,
        RENDER,
    };

    struct runtimeHooks
    {
        std::optional<std::function<void()>> hookBeforeLogic;
        std::optional<std::function<void()>> hookAfterLogic;
        std::optional<std::function<void()>> hookBeforeRender;
        std::optional<std::function<void()>> hookAfterRender;
        std::optional<std::function<void(ECS &)>> hooksNetwork;
    };

    template<typename Fn, typename... Args>
    std::function<void()> make_hook(Fn&& fn, Args&&... args)
    {
        using FnT = std::decay_t<Fn>;
        auto callable = FnT(std::forward<Fn>(fn));
        auto tup = std::make_tuple(std::forward<Args>(args)...);
        return [callable = std::move(callable), tup = std::move(tup)]() mutable {
            std::apply([&callable](auto&&... a){
                std::invoke(callable, std::forward<decltype(a)>(a)...);
            }, tup);
        };
    }

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

        std::vector<std::tuple<std::function<void(ECS& ecs, const std::vector<Entity>& entities)>, View>> systemsLogicList;
        std::vector<std::tuple<std::function<void(ECS& ecs, const std::vector<Entity>& entities)>, View>> systemsRenderList;

        bool _running = true;

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
                this->entitiesComponentMask.resize(DEFAULT_INIT_PAGE_MULTIPLIER * PAGE_SIZE);
            }

            /**
             * Generate a new entity
             * @return A new allocated entity
             */
            Entity newEntity()
            {
                const Entity id = nextEntityID++;
                if (entitiesComponentMask.size() <= id)
                    entitiesComponentMask.resize(id + PAGE_SIZE);
                componentMaskGroups[entitiesComponentMask[id]].push_back(id);
                return id;
            }

            /**
             * Add a component to an entity while passing params to component constructor
             * @tparam Component The component to add
             * @param entity The entity to add to
             */
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

            /**
             * Add a component to an entity using its default constructor
             * @tparam Component The component to add
             * @param entity The entity to add to
             */
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

            /**
             * Remove a component from an entity
             * @tparam Component The component type to remove
             * @param entity The entity to remove from
             */
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

            /**
             * Verify the presence or not of the given component in the entity
             * @tparam Component Component to check for it's presence
             * @param entity The entity to check against
             * @return True if the component is present
             */
            template <typename Component>
            bool HasComponent(const Entity& entity) const
            {
                if (this->componentMaskOffsetStore.find(typeid(Component)) == this->componentMaskOffsetStore.end())
                    return false;
                if (entity >= entitiesComponentMask.size())
                    return false;
                return this->entitiesComponentMask[entity].test(this->getComponentMaskOffset<Component>());
            }

            /**
             * Verify the presence or not of the components in the entity
             * @tparam Components Components to check for their presence
             * @param entity The entity to check against
             * @return True if all components are present
             */
            template <typename... Components>
            bool HasComponents(const Entity& entity) const
            {
                return (this->HasComponent<Components>(entity) && ...);
            }

            /**
             * Return the selected component from the given entity
             * @tparam Component Wanted component
             * @param entity Entity to query
             * @return The wanted component
             * @throws NoComponentFoundPanic if the component is not present in that entity
             */
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

            /**
             * Generate a filtering view from a list of components
             * @tparam Components Component to filter from
             * @return Created view matching the components
             */
            template <typename... Components>
            View GenerateViewFromComponents() const
            {
                ComponentMask mask;
                (mask.set(this->getComponentMaskOffset<Components>()), ...);
                return mask;
            }

            /**
             * Query the ECS to retrieve the list of entities matching the view
             * @note This variant is Exclusive, return entities are assured to have only the required components
             * @param mask View to apply
             * @return List of entities that match the view
             */
            const std::vector<Entity>& QueryView(const View& mask) const
            {
                static const std::vector<Entity> empty;
                const auto it = componentMaskGroups.find(mask);
                if (it == componentMaskGroups.end()) return empty;
                return it->second;
            }

            /**
             * Query the ECS to retrieve the list of entities matching the view
             * @warning This variant is Not Exclusive, return entities are assured to have at least the required components but might have more
             * @param mask View to apply
             * @return List of entities that match the view
             */
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

            void registerSystem(std::function<void(ECS& ecs, const std::vector<Entity>& entities)> handler, const View& view, const systemType& type)
            {
                if (type == systemType::LOGIC)
                    this->systemsLogicList.emplace_back(std::make_tuple(handler, view));
                else
                    this->systemsRenderList.emplace_back(std::make_tuple(handler, view));
            }

            void handExecution(const std::optional<runtimeHooks>& hooks = std::nullopt)
            {
                constexpr double LOGIC_STEP = 0.01;
                double accumulator = 0.0;
                auto prev = std::chrono::high_resolution_clock::now();

                while (this->_running) {
                    auto now = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> frameTime = now - prev;
                    prev = now;
                    accumulator += frameTime.count();

                    while (accumulator > LOGIC_STEP) {
                        if (hooks && hooks->hookBeforeLogic)
                            hooks->hookBeforeLogic.value()();
                        for (const auto& [handler, view] : this->systemsLogicList)
                                handler(*this, this->QueryViewNotExclusive(view));
                        if (hooks && hooks->hookAfterLogic)
                            hooks->hookAfterLogic.value()();
                        accumulator -= LOGIC_STEP;
                    }

                    if (hooks && hooks->hookBeforeRender)
                        hooks->hookBeforeRender.value()();
                    for (const auto& [handler, view] : this->systemsRenderList)
                            handler(*this, this->QueryViewNotExclusive(view));
                    if (hooks && hooks->hookAfterRender)
                        hooks->hookAfterRender.value()();

                    std::this_thread::sleep_for(std::chrono::nanoseconds(100));
                }
            }

            bool& getMasterRunState()
            {
                return this->_running;
            }

            std::unordered_map<ComponentTypeID, ComponentTypeInfo> componentTypeRegistry;

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

            template <typename T>
            void registerComponentType(const std::string& name)
            {
                auto id = getComponentTypeID<T>();

                componentTypeRegistry[id] = {[](flux::Entity& entity, T& component) -> std::string
                                             { return flux::SerializerHandler<T>::serialize(entity, component); },
                                             [](flux::ECS& ecs, flux::Entity& entity, const std::string& data)
                                             {
                                                 T component = SerializerHandler<T>::unserialize(ecs, entity, data);
                                                 // ecs.AddOrReplace(entity, component); // Method should be added in the ECS
                                             },
                                             name};
            }

            void unserializeSingleComponent(const std::string& data)
            {
                std::istringstream in(data);
                ComponentTypeID typeID;
                flux::Entity entity;

                in >> typeID >> entity;

                auto it = this->componentTypeRegistry.find(typeID);
                if (it == this->componentTypeRegistry.end())
                    utils::Logger::debug(std::format("Can't find component with id {}", typeID));
                std::string dataLeft = data.substr(in.tellg());
                it->second.unserialize(entity, dataLeft);
            }
    };
}
