#include "Portfolio.h"

Portfolio::Portfolio(double initialBalance) : cashBalance(initialBalance), stockHoldings(1000) {}

void Portfolio::executeOrder(const TradeSignal& signal, const StockData& data, const std::vector<StockData>& recentData) {
    int orderSize = dynamicOrderSize(data, recentData, 0.01);

    switch (signal) {
    case TradeSignal::BUY:
        if (cashBalance > (data.close * orderSize)) {
            stockHoldings += orderSize;
            cashBalance -= data.close * orderSize;
        }
        break;
    case TradeSignal::SELL:
        if (stockHoldings >= orderSize) {
            stockHoldings -= orderSize;
            cashBalance += data.close * orderSize;
        }
        break;
    case TradeSignal::NONE:
        // Do nothing
        break;
    }
}

double Portfolio::getBalance() const {
    return cashBalance;
}

double Portfolio::getHoldings() const {
    return stockHoldings;
}

double Portfolio::getTotalValue(const StockData& currentData) const {
    return cashBalance + stockHoldings * currentData.close;
}

double Portfolio::computeATR(const std::vector<StockData>& data, const int period) const {
    if (data.size() < period + 1) {
        throw std::runtime_error("Not enough data points for ATR calculation.");
    }

    std::vector<double> trValues;

    for (int i = 1; i < data.size(); i++) {
        double highLow = data[i].high - data[i].low;
        double highClose = std::abs(data[i].high - data[i - 1].close);
        double lowClose = std::abs(data[i].low - data[i - 1].close);

        double tr = std::max({highLow, highClose, lowClose});
        trValues.push_back(tr);
    }

    double sum = 0.0;
    for (int i = 0; i < period; i++) {
        sum += trValues[i];
    }

    double smoothedATR = sum / period;

    // Smoothening ATR incrementally
    for (int i = period; i < trValues.size(); i++) {
        smoothedATR = ((period - 1) * smoothedATR + trValues[i]) / period;
    }

    return smoothedATR;
}


int Portfolio::dynamicOrderSize(const StockData& currentData, const std::vector<StockData>& recentData, double riskPercentage) {
    if(recentData.size() < 15) { // assuming period = 14 for ATR
        throw std::runtime_error("Not enough data points for ATR calculation.");
    }

    double atr = computeATR(recentData, 14);
    double portfolioValue = getTotalValue(currentData);

    double riskAmount = portfolioValue * riskPercentage;

    int orderSize = static_cast<int>(riskAmount / atr);

    const int MAX_ORDER_SIZE = 100;
    orderSize = std::min(orderSize, MAX_ORDER_SIZE);

    return orderSize;
}

void Portfolio::updateRecentDataWindow(const StockData& data) {
    if (recentDataWindow.size() == WINDOW_SIZE) {
        recentDataWindow.pop_front(); // remove oldest data
    }
    recentDataWindow.push_back(data); // add new data
}

bool Portfolio::isRecentDataConsistent() const {
    if (recentDataWindow.size() < 2) return true; // Not enough data to compare

    // Assuming StockData has a member 'date' of type std::string in "YYYY-MM-DD" format.
    // This is a simple check and assumes data does not have weekends/holidays. You might want a more rigorous check depending on your data.
    std::string lastDate = recentDataWindow.back().date;
    std::string secondLastDate = recentDataWindow[recentDataWindow.size() - 2].date;

    // Convert strings to integers for year, month, and day and check if they are consecutive.
    int lastYear = std::stoi(lastDate.substr(0, 4));
    int lastMonth = std::stoi(lastDate.substr(5, 2));
    int lastDay = std::stoi(lastDate.substr(8, 2));

    int secondLastYear = std::stoi(secondLastDate.substr(0, 4));
    int secondLastMonth = std::stoi(secondLastDate.substr(5, 2));
    int secondLastDay = std::stoi(secondLastDate.substr(8, 2));

    // Simple check: (does not account for month/year changes or weekends/holidays)
    return (lastDay - secondLastDay == 1) && (lastMonth == secondLastMonth) && (lastYear == secondLastYear);
}
