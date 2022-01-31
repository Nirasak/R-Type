#pragma once

#include <optional>
#include <vector>
#include <utility>

namespace ecs {
template <typename T>
class sparse_array {
public:
    using value_type = std::optional<T>;
    using reference_type = value_type &;
    using const_reference_type = const value_type &;
    using container_t = std::vector<value_type>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    sparse_array() = default;
    sparse_array(sparse_array const &) = default;
    sparse_array(sparse_array &&) noexcept = default;
    ~sparse_array() = default;
    sparse_array &operator=(sparse_array const &) = default;
    sparse_array &operator=(sparse_array &&) noexcept = default;
    iterator begin() {return m_data.begin();}
    const_iterator begin() const {return m_data.begin();}
    const_iterator cbegin() const {return m_data.cbegin();}
    iterator end() {return m_data.end();}
    const_iterator end() const {return m_data.end();}
    const_iterator cend() const {return m_data.cend();}
    size_type size() const {return m_data.size();}
    reference_type operator[](size_t idx) {
        if (idx >= size())
            m_data.resize(idx + 1);
        return m_data[idx];
    }
    const_reference_type operator[](size_t idx) const {
        if (idx >= size())
            m_data.resize(idx + 1);
        return m_data[idx];
    }

    /**
     *  insert the component at the position pos
     *
     *  @param pos position to insert the component
     *  @param comp component to insert
     * 
     *  @return the component just inserted
     */
    reference_type insert_at(size_type pos, const T &comp) {
        if (pos >= size())
            m_data.resize(pos + 1);
        m_data[pos] = std::make_optional<T>(comp);
        return m_data[pos];
    }

    /**
     *  insert the component at the position pos
     *
     *  @param pos position to insert the component
     *  @param comp component to insert
     * 
     *  @return the component just inserted
     */
    reference_type insert_at(size_type pos, T &&comp) {
        if (pos >= size())
            m_data.resize(pos + 1);
        m_data[pos] = std::make_optional<T>(std::forward<T>(comp));
        return m_data[pos];
    }

    /**
     *  emplace the component at the position pos
     *
     *  @param pos position to emplace the component
     *  @param params parameters to forward to build the component
     * 
     *  @return the component just emplaced
     */
    template <class ...Params>
    reference_type emplace_at(size_type pos, Params &&...params) {
        if (pos >= size())
            m_data.resize(pos + 1);
        m_data[pos] = std::make_optional<T>(std::forward<T>(params)...);
        return m_data[pos];
    }

    /**
     *  erase a component at a position
     *
     *  @param pos position where to erease a component
     * 
     */
    void erase(size_type pos) {
        if (pos < m_data.size())
            m_data[pos] = std::nullopt;
    }

    /**
     *  get the index from a value
     *
     *  @param val value 
     *  @return the index matching the value 
     * 
     */
    size_type get_index(value_type const &val) const {
        for (size_t i = 0; i < m_data.size(); i++) {
            if (m_data[i] == val)
                return i;            
        }
        return -1;
    }
private:
    mutable container_t m_data{};
};
}