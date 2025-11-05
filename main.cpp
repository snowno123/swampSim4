#include "BaseSimulation.h"
#include "config.h"   // SnailConfig class
#include <iostream>

int main(int argc, char** argv) {

    std::string configPath = (argc > 1) ? argv[1] : "swampSim.json";
    int foodRegen = (argc > 2) ? std::stoi(argv[2]) : -1; //currently if passed in via arguments, all regions assigned same foodRegen. if no argument passed in, assign negative value as to check later
    float predSuccessRate = (argc > 3) ? std::stoi(argv[3]) : 0.5;
    std::string csvPath = (argc > 4) ? argv[4]: "swampSim.csv";

    Simulation sim(configPath);                         // engine
    SnailConfig config(configPath, foodRegen, predSuccessRate, csvPath);                        // builder
    sim.setConfig(&config);                             // will be invoked inside run()
    sim.run();                                       // calls cfg.configure(), then runs the loop

    return 0;
}
