#pragma once
#include <vector>
#include <string>

// Define the StockData structure
struct StockData {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    unsigned long volume;
};

// Declare the load_csv function
std::vector<StockData> load_csv(const std::string& filepath);
