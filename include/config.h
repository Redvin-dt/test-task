#pragma once

#include "yaml-cpp/yaml.h"

#include <chrono>
#include <memory>
class Config {
  public:
    static Config *getInstance();

    std::chrono::milliseconds getMoveDelay();
    std::chrono::milliseconds getResetTapeDelay();
    std::chrono::milliseconds getReadDelay();
    std::chrono::milliseconds getWriteDelay();

  private:
    Config();
    Config(const Config &) = default;
    Config &operator=(const Config &) = default;
    ~Config() = default;

    YAML::Node config_;
    std::chrono::milliseconds moveDelay_;
    std::chrono::milliseconds resetTapeDelay_;
    std::chrono::milliseconds readDelay_;
    std::chrono::milliseconds writeDelay_;
};