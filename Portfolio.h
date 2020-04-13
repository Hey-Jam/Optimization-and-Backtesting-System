#ifndef PORTFOLIO
#define PORTFOLIO

#include<iostream>
#include"boost/date_time/gregorian/gregorian.hpp"
#include<string>
#include<unordered_map>
#include<gsl/gsl_vector.h>

class Portfolio{
 private:
  boost::gregorian::date t;
  std::unordered_map<asset*, double> p;
  gsl_vector* weights;
  asset* AssetList;
  
 public:
  Portfolio(boost::gregorian::date, std::unordered_map<asset*, double>);
  Portfolio(boost::gregorian::date, asset*, gsl_vector);
  Portfolio(Portfolio);
  Portfolio(Portfolio&&);
  ~Portfolio();

  // getter
  boost::gregorian::date getDate() const { return t;}
  std::unordered_map<asset*, double> getPortfolio() const { return p;}
  gsl_vector* getWeights() const{ return weights;}
  asset* getAssetList() const{ return AssetList;}
  
  // setter
  void setDate(boost::gregorian::date _t) { t=_t;}

  // operation overload
  double operation[](asset* X){ return p[X];}
};

#endif
