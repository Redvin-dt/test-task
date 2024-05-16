#pragma once

#include <cstdint>

namespace tape {

class ITape {
  public:
    virtual std::int32_t read() = 0;
    virtual void write(std::int32_t value) = 0;
    virtual void moveForward() = 0;
    virtual void moveBackward() = 0;
    virtual void resetTape() = 0;
    virtual bool hasNext() = 0;
    virtual ~ITape() = default;
};

} // namespace tape
