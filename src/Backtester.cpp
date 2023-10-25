#include "Backtester.h"
#include "Portfolio.h"
#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>

Backtester::Backtester(const std::vector<StockData>& data, MovingAverageStrategy& strat, double riskFree)
    : historicalData(data), strategy(strat), riskFreeRate(riskFree) {
    maxPortfolioValue = 10000;  // Start with the initial balance.
}


double Backtester::average(const std::vector<double>& data) const{
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double Backtester::standardDeviation(const std::vector<double>& data) const{
    double avg = average(data);
    double variance = std::accumulate(data.begin(), data.end(), 0.0, [avg](double accum, double value) {
        return accum + (value - avg) * (value - avg);
    }) / data.size();
    return std::sqrt(variance);
}

void Backtester::run() {
    std::vector<StockData> recentData;
    Portfolio portfolio(10000);  // Start with a balance of $10,000
    TradeSignal previousSignal = TradeSignal::NONE;

    for (const auto& dayData : historicalData) {

        recentData.push_back(dayData);
        if (recentData.size() < 15) { // assuming 14-day ATR, so we need at least 15 days
            continue;  // skip trading for the first 14 days
        }
        if (recentData.size() > 15) { // maintain only 15 days of data
            recentData.erase(recentData.begin());
        }

        MovingAverages averages = strategy.computeAverages(dayData);
        double rsi = strategy.computeRSI(dayData);
        TradeSignal currentSignal = strategy.generateSignal(averages);


        if (rsi < 40 && previousSignal != TradeSignal::SELL) {
            currentSignal = TradeSignal::SELL;
        } else if (rsi > 60 && previousSignal != TradeSignal::BUY) {
            currentSignal = TradeSignal::BUY;
        } else {
            currentSignal = strategy.generateSignal(averages);
        }

        if (currentSignal != previousSignal) {  // Only act on changes in signals
            portfolio.executeOrder(currentSignal, dayData, recentData);
            previousSignal = currentSignal;
        }

        
        double currentPortfolioValue = portfolio.getTotalValue(dayData);
        double dailyReturn = (currentPortfolioValue - maxPortfolioValue) / maxPortfolioValue; // calculate the day's return
        returns.push_back(dailyReturn);

        if (dailyReturn > 0) {
            winningTrades++;
            totalProfit += dailyReturn * currentPortfolioValue; // not exact, but a proxy
        } else if (dailyReturn < 0) {
            losingTrades++;
            totalLoss += std::abs(dailyReturn) * currentPortfolioValue; // same as above
        }

        totalTrades++;
        
        if (currentPortfolioValue > maxPortfolioValue) {
            maxPortfolioValue = currentPortfolioValue;
        }
        if (currentPortfolioValue < minPortfolioValue) {
            minPortfolioValue = currentPortfolioValue;
        }
        double drawdown = maxPortfolioValue - minPortfolioValue;
        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }
}

void Backtester::printStatistics() const{
    double winRate = 100.0 * winningTrades / (winningTrades + losingTrades);
    double avgWin = totalProfit / winningTrades;
    double avgLoss = totalLoss / losingTrades;

    std::cout << "Net Profit/Loss: $" << (totalProfit - totalLoss) << std::endl;
    std::cout << "Sharpe Ratio: " << (average(returns) - riskFreeRate) / standardDeviation(returns) << std::endl;
    std::cout << "Max Drawdown: $" << maxDrawdown << std::endl;
    std::cout << "Win Rate: " << winRate << "%" << std::endl;
    std::cout << "Average Win: $" << avgWin << std::endl;
    std::cout << "Average Loss: $" << avgLoss << std::endl;
}




    



