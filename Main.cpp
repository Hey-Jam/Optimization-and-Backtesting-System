#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "Account.hpp"
#include "asset.h"
// #include "OptimizeSystem.h"
#include "StockPool.hpp"
// #include<boost/date_time/gregorian/gregorian.hpp>
// typedef boost::gregorian::date Date;

int main(int argc, char const *argv[])
{
	/*Code*/
	// input ticker list and backtesting period here
	std::vector<std::string> tickerList{"AAPL", "AMZN", "C", "F", "GS", "JPM", "TD", "WFC", "WMT", "CVX"};
	// Date d_start = boost::gregorian::from_us_string("1/2/2015");
	// Date d_end = boost::gregorian::from_us_string("5/1/2020");

	// set datapath and stockpool
	setDataFolder("./testdata/");
	StockPool sp = StockPool(tickerList);

	std::cout << "size: " << sp.getSize() << "\n";

	double px = sp.getStock("AAPL")->get_price(boost::gregorian::from_simple_string("2020/01/22"));

	std::cout << "AAPL 2020/01/22 price: " << px << "\n";

	boost::gregorian::date start{2020, 1, 29};
	boost::gregorian::date end{2020, 2, 3};

	std::vector<boost::gregorian::date> tradingDates = sp.getTradingDates(start, end);
	std::cout << "trading Dates between " << start << ", " << end << ":\n";
	for (int i = 0; i < tradingDates.size(); ++i) {
		std::cout << tradingDates.at(i) << "\n";
	}

	std::vector<std::string> tk{"AAPL", "AMZN", "C", "F", "GS", "JPM", "TD", "WFC", "WMT", "CVX"};
	gsl_matrix* closeprice = sp.getPrice(tk, start, end);
	std::size_t nrow = closeprice->size1;
	std::size_t ncol = closeprice->size2;
	std::cout << "row num: " << closeprice->size1 << "\n";
	std::cout << "col num: " << closeprice->size2 << "\n";

	for (int i = 0; i < nrow; ++i) {
		int count = 0;
		for (int j = 0; j < ncol; ++j) {
			std::cout << gsl_matrix_get(closeprice, i, j) << '\t';
			count += 1;
			if (count == 3)
				std::cout << "\n";
		}
	}

	return 0;
}