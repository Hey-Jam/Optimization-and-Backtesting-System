//
//  main.cpp
//  testProj
//
//  Created by Jam on 5/3/20.
//  Copyright © 2020 Jam. All rights reserved.
//

#include <iostream>
#include "StockPool.hpp"
// #include <boost/date_time/gregorian/gregorian.hpp>
// #include <gsl/gsl_matrix.h>


int main() {
    std::cout << "start" << std::endl;
    setDataFolder("/home/clarence/Dropbox/FordhamClass/advanced cpp/Optimization-and-Backtesting-System/testdata/");

    std::vector<std::string> tickerList{"AAPL", "F", "C", "CVX", "AMZN"};

    StockPool sp = StockPool(tickerList);

    std::cout << "size: " << sp.getSize() << "\n";

    double px = sp.getStock("AAPL")->get_price(boost::gregorian::from_simple_string("2020/01/22"));

    std::cout << "AAPL 2020/01/22 price: " << px << "\n";

    boost::gregorian::date start{2020, 1, 20};
    boost::gregorian::date end{2020, 2, 3};

    std::vector<boost::gregorian::date> tradingDates = sp.getTradingDates(start, end);
    std::cout << "trading Dates between " << start << ", " << end << ":\n";
    for (int i = 0; i < tradingDates.size(); ++i) {
        std::cout << tradingDates.at(i) << "\n";
    }

    std::vector<std::string> tk{"AAPL", "F", "AMZN"};
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
