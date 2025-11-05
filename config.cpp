#include "config.h"
#include "swamp.h"
#include "predator.h"
#include "snail.h"
#include "clock.h"
#include "reporter.h"
#include "region.h"
#include <fstream>
#include <iostream>
#include <cstdlib>   // for rand(), srand()
#include <ctime>

SnailConfig::SnailConfig(const std::string& configPath, int foodRegen, float predSuccessRate, const std::string& csvPath) : Configure(configPath), configPath_(configPath), foodRegen_(foodRegen), predSuccessRate_(predSuccessRate), csvPath_(csvPath) {}

void SnailConfig::configure() {
    srand(time(0)); //Random number generator
    // Defaults
    int timesteps = 365;
    int initialSnails = 100;
    int initialPred = 10;
    int maxAge = 365;
    int predMaxAge = 500;
    int sizeMin = 1;
    int sizeMax = 5;
    int initialFood = 1000;
    int maxFoodLevel = 1000;
    Swamp::Params par; // defaults already set in struct
    std::vector<Region*> regions; //array of regions

    // Read overrides from JSON if present
    std::ifstream file(configPath_);
    if (file.is_open()) {
            nlohmann::json config;
            file >> config;

            timesteps = config.value("timesteps", timesteps);
            initialSnails = config.value("initial_snails", initialSnails);
            initialPred = config.value("initial_pred", initialPred);
            sizeMin = config.value("size_min", sizeMin); 
            sizeMax = config.value("size_max", sizeMax);
            initialFood = config.value("initial_food", initialFood);

            //snail values to read to parameters collection for swamp
            par.maturityAge = config.value("maturity_age", par.maturityAge);
            par.lifespan = config.value("lifespan", par.lifespan);
            maxAge = par.lifespan;
            par.reproProb = config.value("repro_prob", par.reproProb);
            par.reproMin = config.value("offspring_min", par.reproMin);
            par.reproMax = config.value("offspring_max", par.reproMax);
            par.hungerLimit = config.value("hunger_limit", par.hungerLimit);

            //pred values to to read to parameters collection for swamp
            par.predSuccessRate = predSuccessRate_;
            par.predMaturityAge = config.value("pred_maturity_age", par.predMaturityAge);
            par.predLifespan = config.value("pred_lifespan", par.predLifespan);
            predMaxAge = par.predLifespan;
            par.predReproProb = config.value("pred_repro_prob", par.predReproProb);
            par.predReproMin = config.value("pred_offspring_min", par.predReproMin);
            par.predReproMax = config.value("pred_offspring_max", par.predReproMax);
            par.predHungerLimit = config.value("pred_hunger_limit", par.predHungerLimit);

            // Read regions from JSON
            if (config.contains("regions")) {
                for (const auto& regionData : config["regions"]) {
                    initialFood = regionData.value("initial_food", initialFood);
                    if (foodRegen_ != -1) { //Checks if argument was passed in or not
                        //Uses argument passed in from main
                    } else { //no argument passed in, use value from json file
                        foodRegen_ = regionData.value("regen_per_day", foodRegen_);
                    }
                    maxFoodLevel = regionData.value("max_food", maxFoodLevel);

                    Region* r = new Region(/*swamp=*/nullptr, initialFood, foodRegen_, maxFoodLevel);
                    simulation->addObject(r);
                    regions.push_back(r);
                }
            }
            file.close();
    }

    // Create Reporter
    auto* reporter = new Reporter(csvPath_, /*swamp, set later*/ nullptr);
    simulation->addObject(reporter);
     // Create Swamp
    auto* swamp = new Swamp(regions, par, /*sim*/ simulation, reporter, /*intialAmountOfSnails*/ initialSnails, initialPred);

    reporter->setSwamp(swamp);

    //Add swamp to regions
    for (int i = 0; i < regions.size(); i++) {
        regions[i]->setSwamp(swamp);
    }

    //Error checking
    if (sizeMin < 1) {
         std::cout << "\nsizeMin is too small\n";
    }
    if (sizeMax < sizeMin) {
        std::cout << "\nsizeMax is smaller than sizeMin\n";
    }
    if (regions.empty()) {
        std::cout << "\nno regions\n";
    }
    
    //Create initial predators
    for (int i =0; i <initialPred; i++) {
        int age = rand() % predMaxAge + 0;
        int regionIndex = rand() % regions.size();

        regions[regionIndex]->incrementPredAmount(1);
        simulation->addObject(new Predator(swamp, regions[regionIndex], age));
    }

    // Create initial snails
    for (int i = 0; i < initialSnails; ++i) {
        int age = rand() % maxAge + 0; //Randomly generate age between 0 and maxAge
        int regionIndex = rand() % regions.size(); //Randomly choose region
        int size = rand() % sizeMax + sizeMin; //Randomly assign size

        regions[regionIndex]->incrementSnailAmount(1);
        simulation->addObject(new Snail(swamp, regions[regionIndex], age, size));
    }


    // Attach the custom clock (day starts at 0)
    auto* clock = new SnailClock(/*initial*/0, /*limit*/timesteps, swamp, reporter);
    simulation->setClock(clock);
}