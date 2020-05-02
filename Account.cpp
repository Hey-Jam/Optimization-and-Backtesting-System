# include "Account.hpp"

// constructor
Account::Account(double capital, std::vector<std::string>& v): cash(capital), balance(capital)
{
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		pos[*it] = 0;
	}
}

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

	// calculate new position
	std::unordered_map<std::string, int> pos_new;
	// need access to portfolio weights

	// clacuate trasactions need to be done
	if (pos == pos_new) {
		balance[d] = balance_new;
	}
	else {
		std::unordered_map<std::string, int> operation = pos_new - pos;
		pos = pos_new;	// update local position book
		variVec log;	// save different data type into a vector
		for (auto itr = operation.begin(); itr != operation.end(); ++itr)
		{
			if(operation[itr]!=0) {
				// need access to data
				operation[itr]
			}
		}

	}

	// cash remaining

}