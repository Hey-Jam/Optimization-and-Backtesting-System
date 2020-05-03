#ifndef PORTFOLIO
#define PORTFOLIO

#include<iostream>
#include"boost/date_time/gregorian/gregorian.hpp"
#include<string>
#include<unordered_map>
#include<gsl/gsl_vector.h>
#include"Asset.h"

class Portfolio {
private:
  boost::gregorian::date t;
  std::unordered_map<string, double> p;
  gsl_vector* weights;

public:
  //
  Portfolio(boost::gregorian::date _t, vector<string>& _tickers, gsl_vector* _weights):
    t{_t} {
    size_t num = _weights->size;
    weights = gsl_vector_alloc(num);
    // gsl_vector_mmcpy(_weights, weights); 2020-05-03 16:10 BUG1
    gsl_vector_memcpy(_weights, weights);
    for (int i = 0; i < num; i++) {
      p[_tickers[i]] = gsl_vector_get(weights, i);
    }
  }

  ~Portfolio() {
    gsl_vector_free(weights);
  }


  // Getter
  boost::gregorian::date getDate() const { return t;}
  // std::unordered_map<string, double>& getPortfolio() const { return p;}  // BUG2 2020-05-03 16:24
  const std::unordered_map<string, double>& getPortfolio() const { return p;}
  // gsl_vector* getWeights() const { return weights;}  // BUG3 2020-05-03 16:25
  const gsl_vector* getWeights() const { return weights;}

  // Setter
  void setDate(boost::gregorian::date _t) { t = _t;}

  // Other Operation Overload
  // double operator [] (Asset* X) { return p[X];} BUG4 error: no match for ‘operator[]’ (operand types are ‘std::unordered_map<std::__cxx11::basic_string<char>, double>’ and ‘Asset*’)
};

#endif
