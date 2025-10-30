#ifndef BASESIMULATION_H
#define BASESIMULATION_H

#include <string>
#include <vector>

class SimulationObject {
private:
    std::string name;

public:
    explicit SimulationObject(const std::string& _name) : name(_name) {}
    virtual void update() = 0; // Pure virtual
};

class Simulation;

class Configure {
private:
    std::string configFilePath;
protected:
    Simulation* simulation;

public:
    Configure(const std::string& configFilePath, Simulation* simulation = nullptr);
    virtual void configure() = 0; // Pure virtual
    void setSimulation(Simulation* sim) { simulation = sim; }
};

class Clock {
public:
    Clock(int initialTimesteps, int timestepsLimit)
        : timesteps(initialTimesteps), timestepsLimit(timestepsLimit), simulationRunning(true) {}
    int getTimesteps() const { return timesteps; }
    int getTimestepsLimit() const { return timestepsLimit; }
    virtual void update() {
        timesteps++;
        if (timesteps >= timestepsLimit) {
            simulationRunning = false;
        }
    }

    virtual bool checkStop() const { return simulationRunning; }
    

protected:
    int timesteps;
    int timestepsLimit;
    bool simulationRunning;
};

class Simulation {
private:
    std::string configFilePath;
    std::vector<SimulationObject*> simulationObjects;
    Configure* config;
    Clock* clockPTR;

public:
    Simulation(const std::string& config_file_path);
    Simulation();
    void run() {
        config->configure();
        while (clockPTR->checkStop()){
            for (SimulationObject * obj : simulationObjects){
                if (obj == NULL){
                    continue;
                }
                obj->update();
            }
            clockPTR->update();
        }
    }
    void setConfig(Configure* cfg);

    void setClock(Clock* clock){
        clockPTR = clock;
    };

    void addObject(SimulationObject* obj) {
        simulationObjects.push_back(obj);
    }
    std::vector<SimulationObject*> getObjects(){
        return simulationObjects;
    }
};

// Inline definitions to resolve linker issues
inline Configure::Configure(const std::string& configFilePath, Simulation* simulation)
    : configFilePath(configFilePath), simulation(simulation) {}

inline Simulation::Simulation(const std::string& configFilePath)
    : configFilePath(configFilePath), config(nullptr), clockPTR(nullptr) {}


inline void Simulation::setConfig(Configure* cfg) {
    config = cfg;
    if (config) config->setSimulation(this);
}

#endif