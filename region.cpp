#include "region.h"
#include <random>
#include <iostream>
#include <fstream>
#include <cassert> //For assert()

void Region::update () {
    foodLevel_ = std::min(maxFoodLevel_, foodLevel_ + foodRegen_); //Regen food, cap at max
    calculatePredation();
}

void Region::calculatePredation() {
    hungryPredAmount_ = predAmount_;
    if (snailsAmount_ > 0) { //What decimal percentage is the number of hungryPred to number of snails, ex. 1 hungry, 5 snails, 20% or 0.2
        predationProb_ = static_cast<float>(hungryPredAmount_) / snailsAmount_;
    }
}

int Region::snailEatFood( int foodRequested) {
    int foodToEat = 0; //Food that the snail may eat, calculated below

    assert (foodRequested > 0 && "Food requested is less than 1");
    if (foodLevel_ >= foodRequested) { foodToEat = foodRequested; 
    } else {
        foodToEat = foodLevel_; // Cap foodToEat at the remaining food level
    }
    foodLevel_ -= foodToEat;
    assert(foodLevel_ >= 0 && "Food level went below 0"); // Ensure foodLevel_ never goes negative

    return foodToEat;
}

int Region::predEatFood( int foodRequested) {
    //Intialize random
    std::random_device rd;
    rng_ = std::mt19937(rd());
    dist_ = std::uniform_real_distribution<float>(0.0f, 1.0f);
    float chance = dist_(rng_);
    if (predationProb_ < 1.0f) {
        return 1; //currently foodRequested is ignored, food returned is 1
    }

    float excessRatio = predationProb_ - 1.0f; //Percentage of predators who will go hungry, ex. 10 predators and 5 snails, excessRatio is 0.5
    if (chance < excessRatio) {
        return 0; // This predator failed to find food
    } else {
        return 1;
    } 
}

bool Region::checkPredation() {
    //Intialize random
    std::random_device rd;
    rng_ = std::mt19937(rd());
    dist_ = std::uniform_real_distribution<float>(0.0f, 1.0f);
    //check predation probability, if predated return true, else, return false
    float chance = dist_(rng_);
    if (chance < predationProb_) {
        return true; //Predated
    }
    return false; //Not predated
}