#include <iostream>
#include <cstdlib>   // for std::system
#include <string>
#include <fstream> //writing CSV
#include <sstream> //writing summaries
#include <cstdio> //std::rename
#include <filesystem>
#include <cmath>
#include <vector>

int checkEquilibrium( const std::string& csvFile, int window = 10, float threshold = 0.1) { //window = # of days within threshold of stability; threshold = max amount of change in percentage
    
    std::ifstream f(csvFile);
    std::string line;
    //Skip first two lines
    std::getline(f, line); std::getline(f, line);

    //Define variables
    std::vector<int> snails; int day, snail, pred; char comma;

    //Iterate through snails to add to "snails" collection
    while ( f >> day >> comma >> snail >> comma >> pred ) {
        snails.push_back(snail);
    }
    f.close();

    //Calculate daysToEq
    if (snails.size() < window) return snails.size();
    int stableCount = 0;
    for (int i = 1; i < snails.size(); i++) {
        //Calculate change in percentage
        float previous = 0;
        if (float(snails[i-1]) == 0) {
            previous = float(snails[i-1]) + 1; //Avoid dividing by 0
        } else {
            previous = float(snails[i-1]);
        }
        float change = std::abs(snails[i] - snails[i-1]) / previous;

        //Check change against threshold
        if (change < threshold) {
            stableCount++;
            if (stableCount >= window) return i;
        } else {
            stableCount = 0;
        }
    }
    //If never stable, return total number of days
    return snails.size();
}

int main(int argc, char** argv) {
    //If not proper amount of arguments
    if (argc < 5) { std::cerr << "call ./swampSimSweep regenStart,regenEnd regenStep predStart,predEnd predStep configFile\n"; return 1;}

    //Parse arguments with commac
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

    //iterate through the range of values, incrementing by step amount
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

            //Find peak pops, days to equalibrium
            std::ifstream inFile(csvName.str());
            std::string line; std::getline(inFile, line); std::istringstream peakStream(line);
            int peakSnail = 0; int peakPred = 0;
            peakStream >> peakSnail >> peakPred;
            int daysToEq = checkEquilibrium(csvName.str());

            //Append to master csv file
            csv << food << "," << pred << "," << daysToEq << "," << peakSnail << "," << peakPred << "\n";
        }
    }
    return 0;
}
