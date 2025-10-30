#include "snail.h"
#include "swamp.h"
#include "region.h"
#include <cstdlib>   // for rand(), srand()
#include <ctime>
#include <random>

//Constructor
Snail::Snail(Swamp* env, Region* region, int initialAge, int size)
: SimulationObject("Snail"), swamp_(env), region_(region), age_(initialAge),  size_(size) {
    //Intialize random
    std::random_device rd;
    rng_ = std::mt19937(rd());
    dist_ = std::uniform_real_distribution<float>(0.0f, 1.0f); }

void Snail::update() {
    if (alive_) {
        srand(time(0)); //Random number generator
        // 1) Try to eat, starvation if over the limit
        eat();
        if (!alive_) return;
        // 2) Predation check
        checkPredation();
        if (!alive_) return;
        // 3) Reproduction if mature
        reproduce();
        // 4) Age up and handle old age death
        ++age_;
        if (age_ >= swamp_->params().lifespan) {alive_ = false; region_->incrementSnailAmount(-1)/*snail died, remove amount*/;swamp_->snailDied(/*3=ageDeath*/3);}
    }
}

void Snail::eat() {
    int foodRequested = size_;
    int foodEaten = region_->snailEatFood(foodRequested); //ask region to eat food proportionate to their size
    if (foodEaten >= foodRequested) {
    hunger_ = 0;
    } else {
    ++hunger_;
    if (hunger_ >= swamp_->params().hungerLimit) {alive_ = false; region_->incrementSnailAmount(-1); swamp_->snailDied(/*1=starvationDeath*/1);}; //Check for starvation
    }
}

void Snail::checkPredation() {
    if (region_->checkPredation()) {alive_ = false; region_->incrementSnailAmount(-1); swamp_->snailDied(/*2=predDeath*/2);};
}

void Snail::reproduce() {
    float chance = dist_(rng_);//Reproduce probability
    if (age_ >= swamp_->params().maturityAge && chance < swamp_->params().reproProb) {

        //Hungry snails produce less children
        int numOfChild = 0;
        if(hunger_ > 0) {
            numOfChild = swamp_->params().reproMin;
        } else {
            numOfChild = rand() % swamp_->params().reproMax + swamp_->params().reproMin; //Random number of children between minChildren and maxChildren
        }
        
        for (int i = 0; i < numOfChild; ++i) {
            region_->incrementSnailAmount(1);
            swamp_->addSnailtoWorld(new Snail(swamp_, region_, /*initialAge=*/0, size_)); //Snails are born in the same region and size as their parent
        }
}
}

