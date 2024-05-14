#pragma once

#include <cstdint>

namespace tape {

class IInputTape {
  public:
    virtual std::int32_t read() = 0;
    virtual bool moveForward() = 0;
    virtual bool moveBackward() = 0;
    virtual bool hasNext() = 0;
    virtual ~IInputTape() = default;
};

} // namespace tape
