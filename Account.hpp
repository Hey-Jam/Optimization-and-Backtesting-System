#ifndef ACCOUNT_CPP
#define ACCOUNT_CPP

/*
Account class is designed to rocord all the trading details while backtesting or real trading.

	- today: current date in the backtesting period (updated per date)
	- cash: initialized with initial capital, and used to calculate wheather the excution can be done (updated per date)
	- balance: record account balance change in backtesting (updated per date)
	- p: current position state, it contains pointers to assets data and corresponding shres (only updated when there's a trading happend)
	- transaction_log: record each trasaction in time, ticker, shares(+/-) and excution price (only updated when there's a trading happend)
		+ transaction_log format: map<Date, variVec<string ticker,int shares(+/-), double price>>

*/

#include <iostream>
#include <map>
#include <unordered_map>
#include<boost/date_time/gregorian/gregorian.hpp>
#include <boost/variant.hpp>
#include <vector>
#include "Portfolio.h"

typedef boost::gregorian::date Date;
typedef std::vector<boost::variant<int, std::string, double>> variVec;

class Account
{
private:
	double cash;	// cash account, only update when there is a transaction
	double balance;	// update every tick
	std::unordered_map<std::string, int> pos;	// record each ticker and its shares in the portfolio;
	std::map<Date, variVec> transaction_log;
	std::map<Date, double> balance_log;

public:
	Account(Date,Date,double,std::vector<std::string>);
	Account(const Account&);

	// getter
	const double getCash() const {return cash;}
	const double getBalance() const {return balance;}
	const std::unordered_map<std::string, int> getPosition() const {return pos;}
	const std::map<Date, double>* getBalanceLog() const {return balance_log;}

	// setter
	void update(const Portfolio&);

	void showTrasaction();   // display trading details
	void perfomanceSummary();
	double maxDropdown();
	double sharpe();
	double volatility();
	double alpha();
	double beta();
};

#endif // ACCOUNT_CPP
