//
//  StockPool.hpp
//  DataProcessing
//
//  Created by Jam on 4/10/20.
//  Copyright © 2020 Jam. All rights reserved.
//

#ifndef StockPool_hpp
#define StockPool_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
    std::map< std::string, std::map<boost::gregorian::date,double> > pxData; // price data
    int size; // number of stocks
public:
    StockPool(std::vector<std::string> tickerList): size{tickerList.size()} {
        
        for (auto itr=tickerList.begin();itr!=tickerList.end();++itr) {
            std::string path=dataFolder;
            
            std::map<boost::gregorian::date,double> thisPxData;
            
            parse_csv(path.append(*itr).append(".csv"), thisPxData);
            pxData.insert(std::pair<std::string,std::map<boost::gregorian::date,double> >(*itr,thisPxData))
            
        }
        
    }
    
    int getSize() const { return size;}
    
    // get stocks name list in the StockPool as a vector<string>
    std::vector<std::string> getStockList() const {
        std::vector<std::string> stockList;
        for (auto cit=pxData.cbegin();cit!=pxData.cend();++cit) stockList.push_back(cit->first);
        return stockList;
    }
    
    // get price data of a given ticker as a map<boost::gregorian::date,double>
    const std::map<boost::gregorian::date,double>& getPrice(std::string ticker) const {
        return this->pxData.at(ticker)
    }
    
    // get price of given ticker and date
    double getPrice(std::string ticker, std::string date_) const {
        return this->pxData.at(ticker).at(boost::gregorian::from_simple_string(date_));
    }
    
    
};




#endif /* StockPool_hpp */
