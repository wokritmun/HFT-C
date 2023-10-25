#pragma once
#include "StockData.h"
#include "MovingAverageStrategy.h"  // Assuming MarketData and TradeSignal are defined there, else include relevant headers.

class Portfolio {
private:
    double cashBalance;
    double stockHoldings;

public:
    Portfolio(double initialBalance);
    std::deque<StockData> recentDataWindow;
    const size_t WINDOW_SIZE = 15; // or whatever size you want

    void updateRecentDataWindow(const StockData& data);
    void executeOrder(const TradeSignal& signal, const StockData& data, const std::vector<StockData>& recentData);
    double getBalance() const;
    double getHoldings() const;
    bool isRecentDataConsistent() const;
    double getTotalValue(const StockData& currentData) const;
    double computeATR(const std::vector<StockData>& data,const int period) const;
    int dynamicOrderSize(const StockData& currentData, const std::vector<StockData>& recentData, double riskPercentage);
};
