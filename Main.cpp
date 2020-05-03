#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "Account.hpp"
#include "Asset.h"
#include "OptimizeSystem.h"
#include "StockPool.hpp"
// #include<boost/date_time/gregorian/gregorian.hpp>
// typedef boost::gregorian::date Date;

int main(int argc, char const *argv[])
{
    /*Code*/
	// input ticker list and backtesting period here
    std::vector<std::string> tickerlist{"AAPL","AMZN","C","F","GS","JPM","TD","WFC","WMT","CVX"};
    Date d_start = boost::gregorian::from_us_string("1/2/2015");
    Date d_end = boost::gregorian::from_us_string("5/1/2020");


    return 0;
}