#pragma once

#include <cstdint>

namespace tape {

class IInputTape {
  public:
    virtual std::int32_t read() = 0;
    virtual bool go_next() = 0;   // TODO: rename, add go back?
    virtual bool has_value() = 0; // TODO: mb remove
    virtual ~IInputTape() = default;
};

} // namespace tape
