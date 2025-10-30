#include "swamp.h"
#include "reporter.h"
#include "snail.h"
#include "predator.h"
#include "region.h"
#include "reporter.h"
#include <algorithm>
#include <cstdlib>   // for rand(), srand()
#include <ctime>
#include <iostream>
#include <random>

//Constructor
Swamp::Swamp(const std::vector<Region*>& regions, const Params& p, Simulation* sim, Reporter* rep, int aliveSnailCount, int alivePredCount)
: regions(regions), sim_(sim), par_(p), rep_(rep), aliveSnailCount_(aliveSnailCount), alivePredCount_(alivePredCount) {}

void Swamp::addSnailtoWorld(Snail* snail) {
    aliveSnailCount_ += 1;
    sim_->addObject(snail);
    rep_->incBirths(1);
}

void Swamp::snailDied(int typeOfDeath) {
    aliveSnailCount_ -= 1;
    if (typeOfDeath == 1) {rep_->incStarvation(1);}
    if (typeOfDeath == 2) {rep_->incPredation(1);}
    if (typeOfDeath == 3) {rep_->incAgeDeaths(1);}
}

void Swamp::addPredatortoWorld(Predator* predator) {
    alivePredCount_ += 1;
    sim_->addObject(predator);
    rep_->incPredBirths(1); 
}

void Swamp::predatorDied(int typeOfDeath) {
    alivePredCount_ -= 1;
    if (typeOfDeath == 1) {rep_->incPredStarvation(1);}
    if (typeOfDeath == 3) {rep_->incPredAgeDeaths(1);}
}
