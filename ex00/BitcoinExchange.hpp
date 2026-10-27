#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void loadDatabase(const std::string &fileName);
	void processInput(const std::string &fileName);

private:
	std::map<std::string, double> prices; // date -> bitcoin price

	bool isValidDate(const std::string &date);
	bool isValidNumber(const std::string &text);
};

#endif
