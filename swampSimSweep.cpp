#include <iostream>
#include <cstdlib>   // for std::system
#include <string>
#include <fstream> //writing CSV
#include <sstream> //writing summaries
#include <cstdio> //std::rename
#include <filesystem>
#include <cmath>
#include <vector>

int daysToEquilibrium( const std::string& csvFile, int window = 10, float threshold = 0.1) { //window = # of days within threshold of stability; threshold = max amount of change in percentage
    
    std::ifstream f(csvFile); 
    //Skip first two lines of csv file ( header + peak populations )
    std::string line; std::getline(f, line); std::getline(f, line);

    std::vector<int> snailDailyPopulations; int day, snail, pred; char comma;

    // Create collection of daily snail populations
    while ( f >> day >> comma >> snail >> comma >> pred ) {
        snailDailyPopulations.push_back(snail);
    }
    f.close();

    int eqCount = 0;
    //set to total number of days
    int daysToEq = snailDailyPopulations.size(); //value to be returned if snail population never reaches equilibrium

    //Find how many days snail population takes to achieve equilibrium
    for (int day = 1; day < snailDailyPopulations.size(); day++) {
        //Calculate change in percentage
        float previous = 0;
        if (float(snailDailyPopulations[i-1]) == 0) {
            previous = float(snailDailyPopulations[i-1]) + 1; //Avoid dividing by 0
        } else {
            previous = float(snailDailyPopulations[i-1]);
        }
        float change = std::abs(snailDailyPopulations[i] - snailDailyPopulations[i-1]) / previous;

        //Check change in percentage against threshold 
        if (change < threshold) {
            eqCount++;
            if (eqCount >= window) daysToEq = day; //return current day as day the snail population reached equilibrium
        } else {
            eqCount = 0;
        }
    }
    return daysToEq;
}

int main(int argc, char** argv) {
    //If not proper amount of arguments
    if (argc < 5) { std::cerr << "call ./swampSimSweep regenStart,regenEnd regenStep predStart,predEnd predStep configFile\n"; return 1;}

    //Parse arguments with comma
    auto parseRangeInt = [](const std::string& s, int& start, int& end) {
        size_t comma = s.find(',');
        start = std::stoi(s.substr(0, comma));
        end   = std::stoi(s.substr(comma + 1));
    };
    auto parseRangeFloat = [](const std::string& s, float& start, float& end) {
        size_t comma = s.find(',');
        start = std::stof(s.substr(0, comma));
        end   = std::stof(s.substr(comma + 1));
    };

    int foodStart, foodEnd;
    float predStart, predEnd;

    parseRangeInt(argv[1], foodStart, foodEnd);
    int foodStep = std::stoi(argv[2]);
    parseRangeFloat(argv[3], predStart, predEnd);
    float predStep = std::stof(argv[4]);
    std::string config = (argc > 5) ? argv[5] : "swampSim.json";

    // Prepare CSV file
    std::ofstream csv("parameterSweepResults.csv");
    csv << "foodRegen,predSuccessRate,daysToEquilibrium,peakSnail,peakPred\n";  // header

    //run simulation with every combination of foodRegen & predSuccessRate
    for (int food = foodStart; food <= foodEnd; food += foodStep) {
        for (float pred = predStart; pred <= predEnd; pred += predStep) {
            //Build csv file name
            std::ostringstream csvName;
            csvName << "runFood" << food << "pred" << pred << ".csv";

            //Build command
            std::ostringstream cmd;
            cmd << "./swampSim4" << " " << config << " " << food << " " << pred << " " << csvName.str();

            //Run command in terminal
            std::cout << "\nRunning" << cmd.str() <<std::endl;
            std::system(cmd.str().c_str());

            //Find peak populations, days to Equilibrium
            std::ifstream inFile(csvName.str());
            std::string line; std::getline(inFile, line); std::istringstream peakStream(line);
            int peakSnail = 0; int peakPred = 0;
            peakStream >> peakSnail >> peakPred;
            int daysToEq = daysToEquilibrium(csvName.str());

            //Append to master csv file
            csv << food << "," << pred << "," << daysToEq << "," << peakSnail << "," << peakPred << "\n";
        }
    }
    return 0;
}
