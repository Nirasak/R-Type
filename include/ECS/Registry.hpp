#pragma once

#include <map>
#include <any>
#include <typeindex>
#include <functional>

#include "Entity.hpp"
#include "SparseArray.hpp"
#include "Exception.hpp"

namespace ecs {
class registry {
public:

    /**
     *  register a new component to the registry
     *
     *  @param T type of the new component
     *  @return the new component as sparse_array<T>
     */
    template<typename T>
    sparse_array<T> &register_component() {
        if (!has_component<T>())
            m_map[std::type_index(typeid(T))] = std::make_any<sparse_array<T>>();
        m_deleter.push_back([&](entity ent) {
            std::any_cast<sparse_array<T> &>(m_map[std::type_index(typeid(T))]).erase(ent);
        });
        return std::any_cast<sparse_array<T> &>(m_map[std::type_index(typeid(T))]);
    }

    /**
     *  check if the registry contains a component
     *
     *  @param T type of the component to check
     *  @return true if the component is registered, false otherwise
     */
    template<class T>
    bool has_component() {
        if (m_map.contains(std::type_index(typeid(T))))
            return true;
        return false;
    }

    /**
     *  get a sparse_array of components from the registry
     *
     *  @param T type of the component to get
     *  @return sparse_array of components
     */
    template <typename T>
    sparse_array<T> &get_components() {
        if (!has_component<T>())
            throw exception("registry::get_components");
        return std::any_cast<sparse_array<T> &>(m_map[std::type_index(typeid(T))]);
    }

    /**
     *  get a sparse_array of components from the registry
     *
     *  @param T type of the component to get
     *  @return sparse_array of components
     */
    template <typename T>
    const sparse_array<T> &get_components() const {
        if (!has_component<T>())
            throw exception("registry::get_components");
        return std::any_cast<const sparse_array<T> &>(m_map.at(std::type_index(typeid(T))));
    }

    /**
     *  insert a component at a position
     *
     *  @param T type of the component
     *  @param ent position to insert the component
     *  @param comp component to be insert
     *  @return sparse_array of components
     */
    template <typename T>
    typename sparse_array<T>::reference_type insert_component(const entity &ent, T &&comp) {
        if (!has_component<T>())
            throw exception("registry::insert_component");
        return std::any_cast<sparse_array<T> &>(m_map[std::type_index(typeid(T))]).insert_at(ent, std::forward<T>(comp));
    }

    /**
     *  emplace a component at a position
     *
     *  @param T type of the component
     *  @param ent position to emplace the component
     *  @param params parameters to forward to build a component
     *  @return sparse_array of components
     */
    template <typename T, typename ...Params>
    typename sparse_array<T>::reference_type emplace_component(const entity &ent, Params && ...params) {
        if (!has_component<T>())
            throw exception("registry::emplace_component");
        return std::any_cast<sparse_array<T> &>(m_map[std::type_index(typeid(T))]).emplace_at(ent, std::forward<Params>(params)...);
    }

    /**
     *  remove a component from a sparse_array of component
     *
     *  @param T type of the component
     *  @param ent position to remove the component
     * 
     */
    template <typename T>
    void remove_component(const entity &ent) {
        if (!has_component<T>())
            throw exception("registry::remove_component");
        std::any_cast<sparse_array<T> &>(m_map[std::type_index(typeid(T))]).erease(ent);
    }


    /**
     *  This function is used to spawn a new entity
     * 
     *  @return a new entity
     */
    entity spawn_entity() {
        if (m_ents_available.empty()) {
            m_ent_iter++;
            m_pool.push_back(m_ent_iter);
            return m_ent_iter;
        }
        auto it = m_ents_available.front();
        m_ents_available.erase(m_ents_available.begin());
        m_pool.push_back(it);
        return it;
    }

    /**
     *  This function is used to kill an entity
     * 
     *  @param e entity to kill
     */
    void kill_entity(entity e) {
        auto res = std::find(m_pool.begin(), m_pool.end(), e);
        if (res == std::end(m_pool))
            throw exception("entity_pool::kill_entity");
        m_pool.erase(res);
        m_ents_available.push_back(e);
        for (auto &del : m_deleter)
            del(e);
    }

    /**
     *  get entities from the registry
     *
     *  @return the entity list
     *  
     */
    const std::vector<entity> &get_entities() {
        return m_pool;
    }

    template <typename Function>
    /**
     *  This function is used to add a system
     *
     *  @param f function representing a system
     */
    void add_system(Function && f) {
        m_systems.emplace_back([&]() {
            std::forward<Function>(f)(*this);
        });
    }

    /**
     *  This function is used to run all loaded systems
     *
     */
    void run_systems() {
        for (auto &it : m_systems)
            it();
    }

    /**
     *  This function is used to destroy all data from the registry
     *
     */
    void destroy() {
        m_map.clear();
        m_pool.clear();
        m_ents_available.clear();
        m_deleter.clear();
        m_systems.clear();
        m_ent_iter = 0;
    }

private:
    std::map<std::type_index, std::any> m_map{};
    std::vector<entity> m_pool{};
    std::vector<entity> m_ents_available{};
    std::vector<std::function<void(entity)>> m_deleter{};
    std::vector<std::function<void()>> m_systems{};
    entity m_ent_iter{};
};
}
