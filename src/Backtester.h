#pragma once
#include "StockData.h"
#include "MovingAverageStrategy.h"
#include <vector>
#include <limits>

class Backtester {
private:
    std::vector<StockData> historicalData;
    MovingAverageStrategy& strategy;
    double totalProfit;
    double totalLoss;
    int totalTrades;
    int winningTrades;
    int losingTrades;
    double maxPortfolioValue;
    double minPortfolioValue;
    double maxDrawdown;
    double riskFreeRate;
    std::vector<double> returns; // to calculate the Sharpe Ratio

    double average(const std::vector<double>& data) const;
    double standardDeviation(const std::vector<double>& data) const;

public:
    Backtester(const std::vector<StockData>& data, MovingAverageStrategy& strat, double riskFree = 0.01);
    void run();
    void printStatistics() const;
};
