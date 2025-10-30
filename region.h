#ifndef REGION_H
#define REGION_H
#include <random>
#include "BaseSimulation.h"

class Region : public SimulationObject{
public:
    //Constructor
    Region( int foodLevel, int foodRegen, int maxFoodLevel) : SimulationObject("Region"), foodLevel_(foodLevel), foodRegen_(foodRegen), maxFoodLevel_(maxFoodLevel) {}

    void update() override; //Regen foodLevel
    void calculatePredation();

    void incrementSnailAmount(int amount) { snailsAmount_ += amount;}
    void incrementPredAmount(int amount) { predAmount_ += amount;}
    void incrementHundryPredAmount(int amount) {hungryPredAmount_ += amount;}
    int getFoodLevel() const { return foodLevel_; }
    float getPredProb() const { return predationProb_; }
    int snailEatFood(int foodRequested);
    int predEatFood(int foodRequested);
    bool checkPredation(); //Snail calls to ask if preda

private:
    //Set in config.cpp
    int foodLevel_; 
    int foodRegen_; 
    int maxFoodLevel_;
    int snailsAmount_;
    int predAmount_;

    float predationProb_ = 0.0;
    int hungryPredAmount_ = 0;

    //Randomization
    std::mt19937 rng_; 
    std::uniform_real_distribution<float> dist_;
};
#endif