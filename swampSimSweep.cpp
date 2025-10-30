#include <iostream>
#include <cstdlib>   // for std::system
#include <string>
#include <fstream> //writing CSV
#include <sstream> //writing summaries
#include <cstdio> //std::rename
#include <filesystem>

int main(int argc, char** argv) {
    //If not proper amount of arguments
    if (argc < 4) { std::cerr << "call ./swampSimSweep start,end step configFile\n"; return 1;}

    std::string rangeString = argv[1];
    //find comma position
    size_t commaPos = rangeString.find(',');

    int start = std::stoi(rangeString.substr(0, commaPos));
    int end = std::stoi(rangeString.substr(commaPos + 1));
    int step = (argc > 2) ? argv[2] : 1;
    std::string configFile = (argc > 3) ? argv[3] : "swampSim.json";

    // Prepare CSV file
    std::ofstream csv("swampSimSweep.csv");
    csv << "foodRegen,finalPopulation\n";  // header

    //iterate through the range of values, incrementing by step amount
    for (int i = start; value <= end; value += step) {
        std::cout << "\n=== Running simulation with foodRegen = " << value << " ===\n";

        //define command to be called
        std::string command = "./swampSim4 " + configFile + " " + std::to_string(value);
        int result = std:system(command.c_str());
    }

}