#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct StockData {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    unsigned long volume;
};

std::vector<StockData> load_csv(const std::string& filepath) {
    std::vector<StockData> data;
    std::ifstream file(filepath);
    
    std::string line;
    // Skip header line
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        StockData dayData;
        
        std::getline(ss, dayData.date, ',');
        ss >> dayData.open;   ss.ignore();
        ss >> dayData.high;   ss.ignore();
        ss >> dayData.low;    ss.ignore();
        ss >> dayData.close;  ss.ignore();
        ss >> dayData.volume;
        
        data.push_back(dayData);
    }
    
    return data;
}
