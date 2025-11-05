#ifndef SWAMP_H
#define SWAMP_H
#include "BaseSimulation.h"

class Snail;
class Predator;
class Region;
class Simulation; // from BaseSimulation.h
class Reporter;

class Swamp { 
public:
    struct Params { //Default parameters
        int maturityAge = 30; // days
        int lifespan = 365; // days
        float reproProb = 0.1; // per-day
        int reproMin = 2;
        int reproMax = 5;
        int hungerLimit = 3; // days without food before starvation

        float predSuccessRate = 0.5;
        int predMaturityAge = 20; // days
        int predLifespan = 500; // days
        float predReproProb = 0.1; // per-day
        int predReproMin = 1;
        int predReproMax = 2;
        int predHungerLimit = 5; // days without food before starvation
    };
    std::vector<Region*> regions; //Collection of regions

    //Constructor
   Swamp(const std::vector<Region*>& regions, const Params& p, Simulation* sim, Reporter* rep, int aliveSnailCount, int alivePredCount);

    // Snails call these
    void addSnailtoWorld(Snail* snail);
    void snailDied(int typeOfDeath);

    //Predators call these
    void addPredatortoWorld(Predator* predator);
    void predatorDied(int typeOfDeath);

    // Utilities / queries
    int getSnailCount() const { return aliveSnailCount_; }
    int getPredCount() const { return alivePredCount_;}
    const Params& params() const { return par_; }

private:
    Simulation* sim_ = nullptr; // injected
    Params par_; //Parameters
    Reporter* rep_ = nullptr; // injected
    int aliveSnailCount_; //set to initial Snail Count
    int alivePredCount_;
};
#endif