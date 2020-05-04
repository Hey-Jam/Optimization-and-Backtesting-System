/*
    Asset.h is a top-down asset class.

    For now, the only derived class is Stock, as we just take stock into consideration at this stage.
    In the future, fixed income product, commodity product and currency product could be included.
*/


#ifndef Asset_H
#define Asset_H

#include<iostream>
#include<string>
#include<map>
#include"boost/date_time/gregorian/gregorian.hpp"



using namespace std;

// Asset Base Class
class Asset
{
private:
    string ticker;                          // ticker name (eg. aapl)
    string company_name;            // the full company name     (eg. Apple)
    string exchange;                        // the exchange where the asset is traded (eg.NASDAQ)
    string trading_state;               // the tradeing state of the asset (eg. Suspended)


public:
    Asset() {};
    virtual ~Asset() {};

    string get_ticker() const { return ticker; };
    string get_company_name() const { return company_name; };
    string get_exchage() const { return exchange; };
    string get_trading_state() const { return trading_state; };

    void set_ticker(const string& ticker_) { ticker = ticker_; };
    void set_company_name(const string& company_name_) { company_name = company_name_; };
    void set_exchange(const string& exchange_) { exchange = exchange_; };
    void set_trading_state(const string& trading_state_) { trading_state = trading_state_; };

};

class Stock : public Asset
{
private:
    // all the trading fee included
    double platform_commission;     // the platform commission
    double platform_fee;                    // the platform fee
    double threshold_amount;            // the lowest amount of money in the account required
    double clearance_fee;                   // the clearance fee
    double SEC_fee;                             // the fee given to SEC
    double FINRA_fee;                       // the fee given to FINRA
    map<boost::gregorian::date, double> price;  // the series of date and price

public:
    Stock() {};
    virtual ~Stock() {};

    double get_platform_commission() const { return platform_commission; };
    double get_platform_fee() const { return platform_fee; };
    double get_threshold_amount() const { return threshold_amount; };
    double get_clearance_fee() const { return clearance_fee; };
    double get_SEC_fee() const { return SEC_fee; };
    double get_FINRA_fee() const { return FINRA_fee; };
    std::map<boost::gregorian::date, double> get_price() const { return this->price; };
    double get_price(boost::gregorian::date date_) const { return this->price.at(date_); };

    void set_platform_commission(const double& platform_commission_) { platform_commission = platform_commission_; };
    void set_platform_fee(const double& platform_fee_) { platform_fee = platform_fee_; };
    void set_threshold_amount(const double& threshold_amount_) { threshold_amount = threshold_amount_; };
    void set_clearance_fee(const double& clearance_fee_) { clearance_fee = clearance_fee_; };
    void set_SEC_fee(const double& SEC_fee_) { SEC_fee = SEC_fee_; };
    void set_FINRA_fee(const double& FINRA_fee_) { FINRA_fee = FINRA_fee_; };
    void set_price(const map<boost::gregorian::date, double>& price_) { price = price_; };
};

#endif // Asset_H

