#pragma once
#include "StockData.h"
#include <deque>
#include <numeric>

enum class TradeSignal {
    BUY,
    SELL,
    NONE
};
struct MovingAverages {
    double shortTermAvg;
    double longTermAvg;
};

class MovingAverageStrategy {
private:
    std::deque<double> shortTermPrices;
    std::deque<double> longTermPrices;
    std::deque<double> priceChanges;
    double avgGain = 0.0;
    double avgLoss = 0.0;

    const int shortTermPeriod;
    const int longTermPeriod;

public:
    double computeRSI(const StockData& data, int period = 14);
    MovingAverageStrategy(int shortTerm, int longTerm);
    MovingAverages computeAverages(const StockData& data);
    TradeSignal generateSignal(const MovingAverages& averages);
};
