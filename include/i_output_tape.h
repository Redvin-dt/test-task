#pragma once

#include <cstdint>

namespace tape {

class IOutputTape {
  public:
    virtual void write(std::int32_t value) = 0;
    virtual bool go_next() = 0; // TODO: rename, add go_back?
    virtual ~IOutputTape() = default;
};

} // namespace tape
