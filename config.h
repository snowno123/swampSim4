#ifndef CONFIG_H
#define CONFIG_H
#include "BaseSimulation.h"
#include <string>
#include "json.hpp"

class Swamp;
class Reporter;

class SnailConfig : public Configure {
public:
    explicit SnailConfig(const std::string& configPath, int foodRegen, float predSuccessRate, const std::string& csvPath);

    void configure() override; //procedure for reading JSON, adding Swamp + Snails, setting up Clock

private:
    std::string configPath_;
    int foodRegen_;
    float predSuccessRate_;
    std::string csvPath_;
};

#endif