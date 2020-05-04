//
//  CSV_Parser.cpp
//  testProj
//
//  Created by Jam on 5/3/20.
//  Copyright © 2020 Jam. All rights reserved.
//

#include "CSV_Parser.hpp"
#include <fstream>

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
