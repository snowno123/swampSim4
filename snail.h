#ifndef SNAIL_H
#define SNAIL_H
#include "BaseSimulation.h"
#include <random>
class Swamp; class Region;

class Snail : public SimulationObject {
public:
    //Constructor
    Snail(Swamp* env, Region* region, int initialAge, int size);

    void update() override; // ageUp: age, eat/starve, predation, reproduce, age death

    bool isAlive() const { return alive_; } //Check if alive

private:
    Swamp* swamp_ = nullptr; // injected
    Region* region_ = nullptr; //injected
    int age_ = 0; // age in days. at start of sim, random age assigned 
    int size_; 
    int hunger_ = 0; // days without food
    bool alive_ = true; //Default = true
    //Randomization
    std::mt19937 rng_; 
    std::uniform_real_distribution<float> dist_; 

    //Called by snail update each day
    void eat();
    void checkPredation();
    void reproduce();

};


#endif