#include "StockData.h"
#include "MovingAverageStrategy.h"
#include <iostream>
#include "Backtester.h"

using namespace std;

std::vector<StockData> load_csv(const std::string& filename);

int main() {
    std::cout << "HFT System Initialized!" << std::endl;

    auto historicalData = load_csv("../historical_data/AAPL.csv");
    
    MovingAverageStrategy strategy(50, 200);
    Backtester backtester(historicalData, strategy);
    
    backtester.run();
    backtester.printStatistics();

    return 0;
}


