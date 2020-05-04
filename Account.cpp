# include "Account.hpp"
# include <cmath>
# include <fstream>

// map opearator overload
std::unordered_map<std::string, int> operator - ( const std::unordered_map<std::string, int> &p_new, const std::unordered_map<std::string, int> &p) {
	std::unordered_map<std::string, int> res;
	std::unordered_map<std::string, int> p_new_(p_new);
	std::unordered_map<std::string, int> p_(p);

	for (auto iter = p_new_.begin(); iter != p_new_.end(); ++iter) {
		res[iter->first] = iter->second - p_[iter->first];
	}

	// sell out tickers in pos_old but not in pos_new
	for (auto iter = p_.begin(); iter != p_.end(); ++iter) {
		if (p_new_[iter->first] == 0) {
			res[iter->first] = -iter->second;
		}
	}
	return res;
}

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

	// update transaction log and local position
	std::unordered_map<std::string, double> w_new = p->getPortfolio();
	if (w != w_new) {
		// weights changed
		// calculate new position
		std::unordered_map<std::string, int> pos_new;
		for (auto iter = w_new.begin(); iter != w_new.end(); ++iter) {
			pos_new[iter->first] = (int)(sp->getStock(iter->first)->get_price(d) * iter->second);
		}

		// calcualte operation book
		std::unordered_map<std::string, int> operation = pos_new - pos;
		// for (auto iter = operation.begin(); iter != operation.end(); ++iter)
		// {
		// 	std::cout << iter->first << " " << iter->second << std::endl;
		// }
		pos = pos_new;	// update local position book

		// upload transaction log
		for (auto itr = operation.begin(); itr != operation.end(); ++itr)
		{
			if (itr->second != 0) {
				double transaction = itr->second * sp->getStock(itr->first)->get_price(d);
				transaction_log.push_back(variVec {d,itr->first, abs(itr->second), sp->getStock(itr->first)->get_price(d), transaction});	// save different data type into a vector
			}
		}

		// update weights
		w = w_new;
	}

	// calculate current balance
	double balance_new = 0;
	for (auto iter = pos.begin(); iter != pos.end(); ++iter) {
		balance_new += sp->getStock(iter->first)->get_price(d) * iter->second;
	}
	balance = balance_new;

	// update balance log
	balance_log[d] = balance_new;	// update balance log

	// cash remaining
}

// write the result to Transactions.out
void Account::showTransaction() {
	fstream myfile;
	myfile.open("Transactions.out", fstream::out);
	if (!myfile.is_open()) {
		cerr << "Failed to open testmulti to read and wirte\n";
		throw - 1;
	}
	myfile << "Transaction Log" << endl;
	myfile << "DATE   TICKER   SHARES   PRICE   CASHFLOW" << endl;
	for (auto iter = transaction_log.begin(); iter != transaction_log.end(); ++iter)
	{
		// myfile << iter->first << " ";
		for (auto iter2 = (*iter).begin(); iter2 != (*iter).end(); ++iter2)
		{
			myfile << *iter2 << " ";
		}
		myfile << "\n";
	}
	myfile.close();
}


