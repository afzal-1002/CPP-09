#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

/*
Goal: read a price database (data.csv) and an input file of date | amount lines. 
For each line, print the amount multiplied by the bitcoin price on that date. 
If that exact date isn’t in the database, use the closest earlier date.

*/


class BitcoinExchange
{

private:
	std::map<std::string, double> prices;
	bool isValidDate(const std::string &date);
	bool isValidNumber(const std::string &text);

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void loadDatabase(const std::string &fileName);
	void processInput(const std::string &fileName);


};

#endif
