#include "predator.h"
#include "swamp.h"
#include "region.h"
#include <cstdlib>   // for rand(), srand()
#include <ctime>
#include <random>

Predator::Predator(Swamp* env, Region* region, int initialAge)
: SimulationObject("Predator"), swamp_(env), region_(region), age_(initialAge){
    //Intialize random
    std::random_device rd;
    rng_ = std::mt19937(rd());
    dist_ = std::uniform_real_distribution<float>(0.0f, 1.0f); }

void Predator::update() {
    if (alive_) {
        srand(time(0)); //Random number generator
        // 1) Try to eat, starvation if over the limit
        eat();
        if (!alive_) return;
        // 2) Reproduction if mature
        reproduce();
        // 3) Age up and handle old age death
        ++age_;
        if (age_ >= swamp_->params().predLifespan) {alive_ = false; swamp_->predatorDied(/*3=ageDeath*/3);}
    }
}

void Predator::eat() {
    int foodRequested = 1;
    int foodEaten = region_->predEatFood(foodRequested); //ask region to eat food proportionate to their size
    if (foodEaten >= foodRequested) {
    hunger_ = 0;
    } else {
    ++hunger_;
    if (hunger_ >= swamp_->params().predHungerLimit) {alive_ = false; swamp_->predatorDied(/*1=starvationDeath*/1);}; //Check for starvation
    }
}

void Predator::reproduce() {
    float chance = dist_(rng_);//Reproduce probability
    if (age_ >= swamp_->params().predMaturityAge && chance < swamp_->params().predReproProb) {

        //Hungry Predators produce less children
        int numOfChild = 0;
        if(hunger_ > 0) {
            numOfChild = swamp_->params().predReproMin;
        } else {
            numOfChild = rand() % swamp_->params().predReproMax + swamp_->params().predReproMin; //Random number of children between minChildren and maxChildren
        }
        
        for (int i = 0; i < numOfChild; ++i) {
            swamp_->addPredatortoWorld(new Predator(swamp_, region_, /*initialAge=*/0)); //Predators are born in the same region and size as their parent
        }
}
}

