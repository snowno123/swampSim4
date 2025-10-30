#ifndef CONFIG_H
#define CONFIG_H
#include "BaseSimulation.h"
#include <string>
#include "json.hpp"

class Swamp;
class Reporter;

class SnailConfig : public Configure {
public:
    explicit SnailConfig(const std::string& path, int foodRegen);

    void configure() override; //procedure for reading JSON, adding Swamp + Snails, setting up Clock

private:
    std::string path_;
    int foodRegen_;
};

#endif