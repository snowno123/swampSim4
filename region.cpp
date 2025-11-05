#include "region.h"
#include "swamp.h"
#include <random>
#include <iostream>
#include <fstream>
#include <cassert> //For assert()

void Region::update () {
    foodLevel_ = std::min(maxFoodLevel_, foodLevel_ + foodRegen_); //Regen food, cap at max
    eatenSnails_ = 0; //Reset to 0, tracks amount of snailsEaten in each day
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
    
    //calculate amount of snails a predator is able to eat
    int snailsToEat = 0; //Amount of snails that a predator may eat, calculated below
    int avaliableSnails = snailsAmount_ - eatenSnails_; //Amount of snails that are avaliable to eat, total number of snails - number of snails eaten that day

    assert (foodRequested > 0 && "Food requested is less than 1");
    if (avaliableSnails >= foodRequested) { snailsToEat = foodRequested; 
    } else {
        snailsToEat = avaliableSnails; // Cap foodToEat at the remaining food level
    }

    //Predation success rate calculate below, predators may only have a certain chance of eating snails
    if (chance < swamp_->params().predSuccessRate) {
        eatenSnails_ += snailsToEat;
        return snailsToEat;
    } else {
        return 0; //predator failed to catch snails
    }   
}

bool Region::checkPredation() {
    //Intialize random
    std::random_device rd;
    rng_ = std::mt19937(rd());
    dist_ = std::uniform_real_distribution<float>(0.0f, 1.0f);
    float chance = dist_(rng_);

    float predationProb_ = static_cast<float>(eatenSnails_) / snailsAmount_;
    if (chance < predationProb_) {
        return true; //Predated
    }
    return false; //Not predated
}

