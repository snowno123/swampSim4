#ifndef PREDATOR_H
#define PREDATOR_H
#include "BaseSimulation.h"
#include <random>

class Swamp; class Region;

class Predator : public SimulationObject {
public:
    //Constructor
    Predator(Swamp* env, Region* region, int initialAge);

    void update() override; // 

    bool isAlive() const { return alive_; } //Check if alive

private:
    Swamp* swamp_ = nullptr; // injected
    Region* region_ = nullptr; //injected
    int age_ = 0; // age in days. at start of sim, random age assigned 
    int hunger_ = 0; // days without food
    bool alive_ = true; //Default = true
    //Randomization
    std::mt19937 rng_; 
    std::uniform_real_distribution<float> dist_; 

    //Called by update each day
    void eat();
    void reproduce();

};


#endif