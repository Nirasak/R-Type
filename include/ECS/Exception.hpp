#pragma once

#include <exception>
#include <string>
#include <utility>

namespace ecs {
class exception : public std::exception
{
private:
    std::string m_err;
public:
    explicit exception(std::string err) : m_err(std::move(err)) {}
    template<typename ...T>
    exception(T &&...err) : m_err() {m_err.append(std::forward<T>(err)...);}
    ~exception() override = default;
    [[nodiscard]] inline const char *what() const noexcept override {return m_err.c_str();}
};
}