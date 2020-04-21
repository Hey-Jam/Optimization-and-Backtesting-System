/*
 StockPool object records price data and transaction data for your individual stocks list, from a database.
 
 For now, since we haven't finished debugging yahooFinance C++ API, we struct our database by mannually download .csv fils into the same folder
 */

#ifndef StockPool_hpp
#define StockPool_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <utility>
#include "Asset.h"
#include <boost/date_time/gregorian/gregorian.hpp>

extern std::string dataFolder;

void setDataFolder(std::string new_path) dataFolder=new_path;

// Extract dates and Adj Close from csv file
void parse_csv(std::string csvPath, std::map<boost::gregorian::date,double>& series) {
    std::ifstream csv(csvPath);
    if (csv.fail()) std::cerr << "cannot open: " << csvPath << std::endl;
    
    std::stringstream readline;
    std::string s,date;
    double px;
    
    std::getline(csv,readline); //header
    while(std::getline(csv,readline)) {
        std::getline(readline,date,',');
        for (int i=0;i<5;++i) std::getline(readline,s,',');
        std::stringstream(s) >> px;
        
        series.insert(std::pair<boost::gregorian::date,double>(boost::gregorian::from_us_string(date),px));
    }
    csv.close();
}


// Struct StockPool object given a ticker list
class StockPool {
private:
    std::map< std::string, asset* > stocks; // Individual stocks
    int size; // number of stocks
public:
    StockPool(std::vector<std::string> tickerList): size{tickerList.size()} {
        
        for (auto itr=tickerList.begin();itr!=tickerList.end();++itr) {
            
            asset* p=new asset;
            
            std::string path=dataFolder;
            
            std::map<boost::gregorian::date,double> thisPxData;
            
            parse_csv(path.append(*itr).append(".csv"), thisPxData);
            
            p->set_price(thisPxData);
            stocks.insert(std::pair<std::string, asset*>(*itr,p));
            
        }
        
    }
    
    ~StockPool() {
        for (auto itr=stocks.begin();itr!=stocks.end();++itr) {
            asset* p= itr->second;
            delete p;
        }
    }
    
    int getSize() const { return size;}
    
    // get stocks name list in the StockPool as a vector<string>
    std::vector<std::string> getStockList() const {
        std::vector<std::string> stockList;
        for (auto cit=stocks.cbegin();cit!=stocks.cend();++cit) stockList.push_back(cit->first);
        return stockList;
    }
    
    // get specific asset object of a given ticker as an pointer
    const asset* getStock(std::string ticker) const {
        return this->stocks.at(ticker)
    }
    
};


#endif /* StockPool_hpp */
