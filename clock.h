#ifndef CLOCK_H
#define CLOCK_H
#include "BaseSimulation.h"
class Swamp; class Reporter;


class SnailClock : public Clock {
public:
SnailClock(int initialTimesteps, int timestepsLimit, Swamp* swamp, Reporter* rep)
: Clock(initialTimesteps, timestepsLimit), swamp_(swamp), rep_(rep) {}


void update() override; // increments time, logs day, and can early-stop on extinction


private:
Swamp* swamp_ = nullptr; //Avoid undefined behavior
Reporter* rep_ = nullptr;
};


#endif