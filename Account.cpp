# include "Account.hpp"
# include <math>

// constructor
Account::Account(double capital, std::vector<std::string>& v): cash(capital), balance(capital) {}

// copy constructor
Account::Account(const Account& a)
{
	cash = a.cash;
	balance = a.balance;
	pos = a.pos;
	transaction_log = a.transaction_log;
	balance_log = a.balance_log;
}

Account::update(const Portfolio& p)
{
	auto d = p.getDate();

	// calculate current balance
	double balance_new = 0;

	// need access to data
	balance = balance_new;

	// update balance log
	balance_log[d] = balance_new;	// update balance log

	// clacuate trasactions need to be done
	if (w != w_new) {
		// weights changed
		// calculate new position
		std::unordered_map<std::string, int> pos_new;
		// need access to portfolio weights

		std::unordered_map<std::string, int> operation = pos_new - pos;
		pos = pos_new;	// update local position book

		for (auto itr = operation.begin(); itr != operation.end(); ++itr)
		{
			if (operation[itr] != 0) {
				// need access to data
				double transaction = itr->second * Stock[itr->frist];
				// upload transaction
				transaction_log[d] = variVec {itr->first, abs(itr->second), price, transaction};	// save different data type into a vector
			}
		}
	}

	// cash remaining

}