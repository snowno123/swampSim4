#include "clock.h"
#include "swamp.h"
#include "reporter.h"
#include <iostream>


void SnailClock::update() {
// Called after all SimulationObjects updated for the day
int population = swamp_ ? swamp_->getSnailCount() : 0;

// Advance time and check the standard time-limit stop
Clock::update();

// Early stop on extinction
if (population <= 0) {
    std::cout << "\nEnded early, Population hit 0!\n";
    if (rep_) rep_->writeSummary();
    simulationRunning = false;
    return;
}

// If the time limit was reached inside Clock::update(), finalize summary too
if (!simulationRunning) {
    std::cout << "\nSimulation complete, last timestep reached!\n";
    if (rep_) rep_->writeSummary();
}
}