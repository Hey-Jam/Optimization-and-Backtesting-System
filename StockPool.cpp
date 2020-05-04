//
//  StockPool.cpp
//  testProj
//
//  Created by Jam on 5/3/20.
//  Copyright © 2020 Jam. All rights reserved.
//

#include <iostream>
#include "CSV_Parser.hpp"
#include "StockPool.hpp"
#include "asset.h"
#include <utility>
#include <map>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <gsl/gsl_matrix.h>

/*
// Extract dates and Adj Close from csv file
void parse_csv(std::string csvPath, std::map<boost::gregorian::date, double>& series) {
    std::ifstream csv(csvPath);
    if (csv.fail()) std::cerr << "cannot open: " << csvPath << std::endl;

    std::stringstream ss_readline;
    std::string s, date, readline;
    double px;

    std::getline(csv, readline); //header
    while (std::getline(csv, readline)) {
        ss_readline = std::stringstream(readline);
        std::getline(ss_readline, date, ',');
        for (int i = 0; i < 5; ++i) {
            std::getline(ss_readline, s, ',');
        }
        std::stringstream(s) >> px;

        series.insert(std::pair<boost::gregorian::date, double>(boost::gregorian::from_simple_string(date), px));
    }
    csv.close();
}
*/

// Struct StockPool object given a ticker list
StockPool::StockPool(std::vector<std::string> tickerList, const std::string& dataFolder): size{tickerList.size()} {

    for (auto itr = tickerList.begin(); itr != tickerList.end(); ++itr) {
        Stock* p = new Stock;

        std::string path(dataFolder);

        std::map<boost::gregorian::date, double> thisPxData;

        parse_csv(path.append(*itr).append(".csv"), thisPxData);

        p->set_price(thisPxData);
        stocks.insert(std::pair<std::string, Stock*>(*itr, p));

    }

}

StockPool::~StockPool() {
    for (auto itr = stocks.begin(); itr != stocks.end(); ++itr) {
        Stock* p = itr->second;
        delete p;
    }
}

std::size_t StockPool::getSize() const { return size;}

// get stocks name list in the StockPool as a vector<string>
std::vector<std::string> StockPool::getStockList() const {
    std::vector<std::string> stockList;
    for (auto cit = stocks.cbegin(); cit != stocks.cend(); ++cit) {
        stockList.push_back(cit->first);
    }
    return stockList;
}

// get specific Stock object of a given ticker as an pointer
const Stock* StockPool::getStock(std::string ticker) const {
    return this->stocks.at(ticker);
}


std::vector<boost::gregorian::date> StockPool::getTradingDates(boost::gregorian::date start, boost::gregorian::date end) const {

    std::map<boost::gregorian::date, double> pxData = (stocks.begin()->second)->get_price();
    auto itr_start = pxData.lower_bound(start);
    auto itr_end = pxData.upper_bound(end);
    std::vector<boost::gregorian::date> tradingDates;
    std::map<boost::gregorian::date, double>::iterator itr;
    for (itr = itr_start; itr != itr_end; ++itr) {
        tradingDates.push_back(itr->first);
    }

    return tradingDates;

}

gsl_matrix* StockPool::getPrice(std::vector<std::string> tickers, boost::gregorian::date start, boost::gregorian::date end) const {

    size_t ncol = tickers.size();

    std::vector<boost::gregorian::date> tradingDates = this->getTradingDates(start, end);
    size_t nrow = tradingDates.size();

    gsl_matrix* priceMatrix = gsl_matrix_alloc(nrow, ncol);
    for (int j = 0; j < ncol; ++j) {
        for (int i = 0; i < nrow; ++i) {
            gsl_matrix_set(priceMatrix, i, j, stocks.at(tickers.at(j))->get_price(tradingDates.at(i)));
        }
    }

    return priceMatrix;

}
