#ifndef PORTFOLIO
#define PORTFOLIO

#include<iostream>
#include"boost/date_time/gregorian/gregorian.hpp"
#include<string>
#include<unordered_map>
#include<gsl/gsl_vector.h>
#include"Asset.h"

class Portfolio{
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
      gsl_vector_mmcpy(_weights, weights);

      for(int i=0; i<num; i++){
	p[_tickers[i]] = gsl_vector_get(weights, i);
      }
    }
  
  ~Portfolio(){
    gsl_vector_free(weights);
  }

  
  // Getter
  boost::gregorian::date getDate() const { return t;}
  std::unordered_map<string, double>& getPortfolio() const { return p;}
  gsl_vector* getWeights() const{ return weights;}

  
  // Setter
  void setDate(boost::gregorian::date _t) { t=_t;}

  // Other Operation Overload
  double operation[](Asset* X){ return p[X];}
};

#endif
