#pragma once

#include <cstdint>

namespace tape {

class IOutputTape {
  public:
    virtual void write(std::int32_t value) = 0;
    virtual bool moveForward() = 0;
    virtual bool moveBackward() = 0; // TODO: mb add reset tape
    virtual ~IOutputTape() = default;
};

} // namespace tape
