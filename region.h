#ifndef REGION_H
#define REGION_H
#include <random>
#include "BaseSimulation.h"

class Swamp;
class Region : public SimulationObject{
public:
    //Constructor
    Region( Swamp* swamp, int foodLevel, int foodRegen, int maxFoodLevel) : SimulationObject("Region"), foodLevel_(foodLevel), foodRegen_(foodRegen), maxFoodLevel_(maxFoodLevel) {}

    void update() override; //Regen foodLevel

    void incrementSnailAmount(int amount) { snailsAmount_ += amount;}
    void incrementPredAmount(int amount) { predAmount_ += amount;}
    int getFoodLevel() const { return foodLevel_; }
    int snailEatFood(int foodRequested);
    int predEatFood(int foodRequested);
    bool checkPredation(); //Snail calls to ask if predator

    void setSwamp(Swamp* swamp) {swamp_ = swamp;}

private:
    Swamp* swamp_ = nullptr; // injected
    
    int foodLevel_; 
    int foodRegen_; 
    int maxFoodLevel_;
    int snailsAmount_;
    int predAmount_;

    int eatenSnails_ = 0;

    //Randomization
    std::mt19937 rng_; 
    std::uniform_real_distribution<float> dist_;
};
#endif