#include "BitcoinExchange.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : prices(other.prices)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		prices = other.prices;
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

// Date must look like YYYY-MM-DD and be a real calendar date
bool BitcoinExchange::isValidDate(const std::string &date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	for (size_t i = 0; i < date.size(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(date[i]))
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12 || day < 1)
		return false;

	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
	if (isLeapYear)
		daysInMonth[1] = 29;

	return day <= daysInMonth[month - 1];
}

// Number can have a sign, digits and at most one dot (ex: 3, -1, 1.2)
bool BitcoinExchange::isValidNumber(const std::string &text)
{
	int digits = 0;
	int dots = 0;

	for (size_t i = 0; i < text.size(); i++)
	{
		if (i == 0 && (text[i] == '-' || text[i] == '+'))
			continue;
		if (text[i] == '.')
			dots++;
		else if (std::isdigit(text[i]))
			digits++;
		else
			return false;
	}
	return digits > 0 && dots <= 1;
}

// Read data.csv and store every "date,price" line in the map
void BitcoinExchange::loadDatabase(const std::string &fileName)
{
	std::ifstream file(fileName.c_str());
	if (!file.is_open())
		throw std::runtime_error("could not open database.");

	std::string line;
	std::getline(file, line); // skip header "date,exchange_rate"

	while (std::getline(file, line))
	{
		size_t comma = line.find(',');
		if (comma == std::string::npos)
			continue;

		std::string date = line.substr(0, comma);
		std::string price = line.substr(comma + 1);

		if (isValidDate(date) && isValidNumber(price))
			prices[date] = std::atof(price.c_str());
	}

	if (prices.empty())
		throw std::runtime_error("database is empty.");
}

// Read the input file and print "date => value = value * price"
void BitcoinExchange::processInput(const std::string &fileName)
{
	std::ifstream file(fileName.c_str());
	if (!file.is_open())
		throw std::runtime_error("could not open file.");

	std::cout.precision(10); // show big results like 47115930 without "e+07"

	std::string line;
	std::getline(file, line); // skip header "date | value"

	while (std::getline(file, line))
	{
		size_t separator = line.find(" | ");
		if (separator == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = line.substr(0, separator);
		std::string valueText = line.substr(separator + 3);

		if (!isValidDate(date) || !isValidNumber(valueText))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		double value = std::atof(valueText.c_str());
		if (value < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}

		// upper_bound gives the first date AFTER our date,
		// so one step back is our date or the closest lower date
		std::map<std::string, double>::iterator it = prices.upper_bound(date);
		if (it == prices.begin())
		{
			std::cout << "Error: no price before => " << date << std::endl;
			continue;
		}
		--it;

		std::cout << date << " => " << value << " = " << value * it->second << std::endl;
	}
}
