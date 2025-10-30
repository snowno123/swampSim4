#ifndef REPORTER_H
#define REPORTER_H
#include "BaseSimulation.h"
#include <string>
#include <fstream>
class Swamp;

class Reporter : public SimulationObject{
public:
struct Totals {
int births = 0;
int death_starvation = 0;
int death_predation = 0;
int death_age = 0;
int peak_snail_population = 0;
int pred_births = 0;
int pred_death_starvation = 0;
int pred_death_age = 0;
int peak_pred_population;
};

explicit Reporter(const std::string& csvFilePath = "", Swamp* swamp = nullptr);

void update() override; //Calls considerPeak, calculates peak_population


// Counters that other objects call
void incBirths(int n=1) { totals_.births += n; }
void incStarvation(int n=1) { totals_.death_starvation += n; }
void incPredation(int n=1) { totals_.death_predation += n; }
void incAgeDeaths(int n=1) { totals_.death_age += n; }

void incPredBirths(int n=1) { totals_.pred_births += n; }
void incPredStarvation(int n=1) { totals_.pred_death_starvation += n; }
void incPredAgeDeaths(int n=1) { totals_.pred_death_age += n; }

void setSwamp(Swamp* swamp) {swamp_ = swamp;}


void considerSnailPeak(int snailPopulation);
void considerPredPeak(int predPopulation);


// Final write helper
void writeSummary();


const Totals& totals() const { return totals_; }


private:
std::string path_;
Swamp* swamp_ = nullptr;
Totals totals_;
int daysSimulated = 0;
std::vector<std::pair<int, int>> dailyPopulations_;
};


#endif