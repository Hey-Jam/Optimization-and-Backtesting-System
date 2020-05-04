//
//  CSV_Parser.hpp
//  testProj
//
//  Created by Jam on 5/3/20.
//  Copyright © 2020 Jam. All rights reserved.
//

#ifndef CSV_Parser_hpp
#define CSV_Parser_hpp

#include <iostream>
#include <map>
#include <boost/date_time/gregorian/gregorian.hpp>

// Extract dates and Adj Close from csv file
void parse_csv(std::string csvPath, std::map<boost::gregorian::date, double>& series);

#endif /* CSV_Parser_hpp */
