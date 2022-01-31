#pragma once

#include <string>
#include "Registry.hpp"

namespace ecs {
class module
{
public:
    virtual ~module() = default;
    /**
     *  spawn a new entity
     *
     *  @param registry the registry to put the new entity on
     */
    virtual ecs::entity spawn(registry &) = 0;
    /**
     *  get the system to manage the entity
     *
     *  @param registry the registry to use with the system
     *  @return the system function as a std::function<void(registry &)>
     */
    virtual std::function<void(registry &)> get_system() = 0;
    /**
     *  @return the entity's label
     */
    virtual std::string get_label() = 0;
};
}