#include "MovingAverageStrategy.h"
#include <iostream>
#include <deque>
#include <numeric>

using namespace std;

// Constructor
MovingAverageStrategy::MovingAverageStrategy(int shortTerm, int longTerm)
    : shortTermPeriod(shortTerm), longTermPeriod(longTerm) {}

MovingAverages MovingAverageStrategy::computeAverages(const StockData& data) {

    // Add new data
    shortTermPrices.push_back(data.close);
    longTermPrices.push_back(data.close);

    // Remove old data
    if (shortTermPrices.size() > shortTermPeriod) {
        shortTermPrices.pop_front();
    }
    if (longTermPrices.size() > longTermPeriod) {
        longTermPrices.pop_front();
    }

    // Compute averages
    MovingAverages averages;
    averages.shortTermAvg = accumulate(shortTermPrices.begin(), shortTermPrices.end(), 0.0) / shortTermPrices.size();
    averages.longTermAvg = accumulate(longTermPrices.begin(), longTermPrices.end(), 0.0) / longTermPrices.size();

    return averages;
}

TradeSignal MovingAverageStrategy::generateSignal(const MovingAverages& averages) {
    if (averages.shortTermAvg > averages.longTermAvg) {
        return TradeSignal::BUY;
    } else if (averages.shortTermAvg < averages.longTermAvg) { // Note: You had a typo here in the original code (comparing shortTermAvg with itself). 
        return TradeSignal::SELL;                              // I corrected it to compare with longTermAvg.
    } else {
        return TradeSignal::NONE;
    }
}

double MovingAverageStrategy::computeRSI(const StockData& data, int period) {
    double currentChange = data.close - (shortTermPrices.empty() ? data.close : shortTermPrices.back());
    double gain = currentChange > 0 ? currentChange : 0;
    double loss = currentChange < 0 ? -currentChange : 0;

    if (priceChanges.size() < period) {
        avgGain = (avgGain * priceChanges.size() + gain) / (priceChanges.size() + 1);
        avgLoss = (avgLoss * priceChanges.size() + loss) / (priceChanges.size() + 1);
    } else {
        avgGain = (avgGain * (period - 1) + gain) / period;
        avgLoss = (avgLoss * (period - 1) + loss) / period;
    }

    priceChanges.push_back(currentChange);
    if (priceChanges.size() > period) {
        priceChanges.pop_front();
    }

    double RS = avgLoss == 0 ? 0 : avgGain / avgLoss;
    return 100.0 - (100.0 / (1.0 + RS));
}


