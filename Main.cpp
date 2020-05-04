#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "Account.hpp"
#include "asset.h"
#include "OptimizeSystem.h"
#include "StockPool.hpp"
// #include<boost/date_time/gregorian/gregorian.hpp>
// typedef boost::gregorian::date Date;

int main(int argc, char const *argv[])
{
	/*Code*/
	// input ticker list and backtesting period here
	std::vector<std::string> tickerList{"AAPL", "AMZN", "C", "F", "GS", "JPM", "TD", "WFC", "WMT", "CVX"};
	Date start_d = boost::gregorian::from_us_string("1/2/2015");
	Date end_d = boost::gregorian::from_us_string("5/1/2020");
	Date start_sart = boost::gregorian::from_us_string("1/2/2014");

	// initialize stockpool
	StockPool* sp = new StockPool(tickerList, "./testdata/");

	// initialize Account
	double initial_capital = 1000000.0;
	Account account(initial_capital, tickerList, sp);

	// // initialize Portfolio
	// Portfolio P(start_d, tickerList,);

	// get trading calendar
	std::vector<Date> Calendar(sp->getTradingDates(start_d,end_d));

	// initialize OptimizeSystem
	ClassicMarkowitz optimizer(sp);
	Portfolio* P = optimizer.optimize(tickerList,start_sart,start_d);

	// for (auto iter = Calendar.begin(); iter != Calendar.end(); ++iter)
	// {

	// 	/* code */
	// }


	return 0;
}

