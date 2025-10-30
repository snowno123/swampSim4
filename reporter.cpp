#include "reporter.h"
#include "swamp.h"
#include <iostream>
#include <fstream>

Reporter::Reporter(const std::string& csvFilePath, Swamp* swamp)
    : SimulationObject("Reporter"), path_(csvFilePath), swamp_(swamp) {} 

void Reporter::update() {
    int snailPopulation = swamp_ ? swamp_->getSnailCount() : 0;
    int predPopulation = swamp_ ? swamp_->getPredCount() : 0;
    daysSimulated += 1;
    dailyPopulations_.push_back({snailPopulation, predPopulation});
    considerSnailPeak(snailPopulation);
    considerPredPeak(predPopulation);
}

void Reporter::considerSnailPeak(int snailPopulation) { 
    if (snailPopulation > totals_.peak_snail_population) totals_.peak_snail_population = snailPopulation;
}

void Reporter::considerPredPeak(int predPopulation) { 
    if (predPopulation > totals_.peak_pred_population) totals_.peak_pred_population = predPopulation;
}

void Reporter::writeSummary() {

    int finalSnailPop = swamp_ ? swamp_->getSnailCount() : 0;
    int finalPredPop = swamp_ ? swamp_->getPredCount() : 0;

    // Write daily log to CSV
    std::ofstream outFile(path_);
    if (outFile.is_open()) {
        std::cout << "\nDay,SnailPopulation, PredPopulation\n"; //Print out to terminal
        outFile << "Day,SnailPopulation, PredPopulation\n";
        int day = 1;
        for (const auto& [spop, ppop] : dailyPopulations_) {
            std::cout << "Days, " << "Snails: " << "Predators: " << day << ", " << spop << ", " << ppop << "\n"; //Print out to terminal
            outFile << day << "," << spop << ", " << ppop << "\n";
            day +=1 ;
        }
        outFile.close();
    }

    std::cout << "\nSimulation complete!\n";
    std::cout << "Days simulated: " << daysSimulated << "\n";
    std::cout << "Final snail population: " << finalSnailPop << "\n";
    std::cout << "Peak snail population: " << totals_.peak_snail_population << "\n";
    std::cout << "Total snail births: " << totals_.births << "\n";
    std::cout << "Deaths - snail - starvation: " << totals_.death_starvation
    << ", snail - predation: " << totals_.death_predation
    << ", snail - age: " << totals_.death_age << "\n";
    std::cout << "Final pred population: " << finalPredPop << "\n";
    std::cout << "Peak pred population: " << totals_.peak_pred_population << "\n";
    std::cout << "Total pred births: " << totals_.pred_births << "\n";
    std::cout << "Deaths - pred - starvation: " << totals_.pred_death_starvation
    << ", pred - age: " << totals_.pred_death_age << "\n";
}