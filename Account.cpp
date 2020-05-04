# include "Account.hpp"
# include <cmath>
# include <fstream>

// constructor
Account::Account(double capital, std::vector<std::string>& v, StockPool* s): cash(capital), balance(capital), sp(s) {}

// copy constructor
Account::Account(const Account& a)
{
	cash = a.cash;
	balance = a.balance;
	pos = a.pos;
	transaction_log = a.transaction_log;
	balance_log = a.balance_log;
	sp = a.sp;
}

void Account::update(const Portfolio* p)
{
	Date d = p->getDate();

	// calculate current balance
	double balance_new = 0;
	for (auto iter = pos.begin(); iter != pos.end(); ++iter) {
		balance_new += sp->getStock(iter->first)->get_price(d) * iter->second
	}
	balance = balance_new;

	// update balance log
	balance_log[d] = balance_new;	// update balance log

	// update transaction log
	std::unordered_map<std::string, double> w_new = p->getPortfolio();
	if (w != w_new) {
		// weights changed

		// calculate new position
		std::unordered_map<std::string, int> pos_new;
		for (auto iter = pos.begin(); iter != pos.end(); ++iter) {
			pos_new[iter->first] = int(sp->getStock(iter->first)->get_price(d) * w_new[iter->first]);
		}

		// need access to portfolio weights
		std::unordered_map<std::string, int> operation = pos_new - pos;
		pos = pos_new;	// update local position book

		for (auto itr = operation.begin(); itr != operation.end(); ++itr)
		{
			if (operation[itr] != 0) {
				double transaction = itr->second * sp->getStock(itr->first)->get_price(d);
				// upload transaction log
				transaction_log[d] = variVec {itr->first, abs(itr->second), sp->getStock(itr->first)->get_price(d), transaction};	// save different data type into a vector
			}
		}
	}

	// cash remaining
}

// write the result to Transactions.out
void Account::showTransactions() {
	fstream myfile;
	myfile.open("Transactions.out", fstream::in);
	if (!myfile.is_open()) {
		cerr << "Failed to open testmulti to read and wirte\n";
		return -1;
	}
	myfile << "Transaction Log" << endl;
	myfile << "DATE   TICKER   SHARES   PRICE   CASHFLOW" << endl;
	for (auto iter = transaction_log.begin(); iter != transaction_log.end(); ++iter)
	{
		myfile << iter->first << " ";
		for (auto iter2 = (iter->second).begin(); iter2 != (iter->second).end(); ++iter2)
		{
			myfile << iter->first << " ";
		}
		myfile << "\n";
	}
	myfile.colse();
}

// map opearator overload
friend std::unordered_map<std::string, int> operator - ( const std::unordered_map<std::string, int> &p_new, const std::unordered_map<std::string, int> &p){
	std::unordered_map<std::string, int> res;

	for (auto iter = p_new.begin(); iter != p_new.end(); ++iter) {
		res[iter->first] = iter->second - p[iter->frist]
	}

	// sell out tickers in pos_old but not in pos_new
	for (auto iter = p.begin(); iter != p.end(); ++iter) {
		if(p_new[iter->first]==0) {
			res[iter->first] = -iter->second
		}
	}
}

