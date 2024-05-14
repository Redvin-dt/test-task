#include "config.h"
#include <chrono>

Config *Config::getInstance() {
    static Config instance;
    return &instance;
}

Config::Config() : config_(YAML::LoadFile("config.yaml")) {
    moveDelay_ = std::chrono::milliseconds(config_["moveDelayMs"].as<int>());
    resetTapeDelay_ =
        std::chrono::milliseconds(config_["resetTapeDelayMs"].as<int>());
    readDelay_ = std::chrono::milliseconds(config_["readDelayMs"].as<int>());
    writeDelay_ = std::chrono::milliseconds(config_["writeDelayMs"].as<int>());
}

std::chrono::milliseconds Config::getMoveDelay() { return moveDelay_; }

std::chrono::milliseconds Config::getResetTapeDelay() {
    return resetTapeDelay_;
}

std::chrono::milliseconds Config::getReadDelay() { return readDelay_; }

std::chrono::milliseconds Config::getWriteDelay() { return writeDelay_; }
