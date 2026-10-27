#include "PmergeMe.hpp"

#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other) : _vector(other._vector), _deque(other._deque)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vector = other._vector;
		_deque = other._deque;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}

// Only digits are accepted, so "-1", "abc" or "" are errors
int PmergeMe::toNumber(const std::string &text)
{
	if (text.empty() || text.size() > 10)
		throw std::runtime_error("bad number");
	for (size_t i = 0; i < text.size(); i++)
	{
		if (!std::isdigit(text[i]))
			throw std::runtime_error("bad number");
	}
	long number = std::atol(text.c_str());
	if (number > INT_MAX)
		throw std::runtime_error("number too big");
	return number;
}

// Order in which the small numbers are inserted (Jacobsthal numbers).
// smalls[0] is already in the main chain, so for count = 6 we get:
// 2 1, 4 3, 5   (groups end at Jacobsthal numbers 3, 5, 11, 21, ...)
std::vector<size_t> PmergeMe::insertionOrder(size_t count)
{
	std::vector<size_t> order;
	size_t previous = 1;
	size_t current = 3;
	size_t done = 1;

	while (done < count)
	{
		size_t end = std::min(current, count);
		for (size_t i = end; i > done; i--)
			order.push_back(i - 1);
		done = end;

		size_t next = current + 2 * previous;
		previous = current;
		current = next;
	}
	return order;
}

/*
** Ford-Johnson (merge-insertion) sort:
** 1. Make pairs and put the bigger number first.
** 2. Sort the big numbers with the same algorithm (recursion).
** 3. Put the small numbers in the same order as their big partners.
** 4. Main chain = first small + all bigs (first small is smaller than all of them).
** 5. Insert the other smalls with binary search, in Jacobsthal order.
**    Each small only needs to be searched before its big partner.
*/
void PmergeMe::sortVector(std::vector<int> &numbers)
{
	if (numbers.size() < 2)
		return;

	// 1. Make pairs (big, small)
	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i + 1 < numbers.size(); i += 2)
	{
		int a = numbers[i];
		int b = numbers[i + 1];
		if (a < b)
			std::swap(a, b);
		pairs.push_back(std::make_pair(a, b));
	}

	// 2. Sort the bigs
	std::vector<int> bigs;
	for (size_t i = 0; i < pairs.size(); i++)
		bigs.push_back(pairs[i].first);
	sortVector(bigs);

	// 3. Smalls in the same order as the sorted bigs
	std::vector<int> smalls;
	for (size_t i = 0; i < bigs.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (pairs[j].first == bigs[i])
			{
				smalls.push_back(pairs[j].second);
				pairs.erase(pairs.begin() + j);
				break;
			}
		}
	}
	if (numbers.size() % 2 == 1)
		smalls.push_back(numbers.back()); // odd number has no partner

	// 4. Main chain
	std::vector<int> mainChain;
	mainChain.push_back(smalls[0]);
	mainChain.insert(mainChain.end(), bigs.begin(), bigs.end());

	// 5. Insert the rest
	std::vector<size_t> order = insertionOrder(smalls.size());
	for (size_t i = 0; i < order.size(); i++)
	{
		size_t index = order[i];
		std::vector<int>::iterator limit = mainChain.end();
		if (index < bigs.size())
			limit = std::find(mainChain.begin(), mainChain.end(), bigs[index]);

		std::vector<int>::iterator position = std::lower_bound(mainChain.begin(), limit, smalls[index]);
		mainChain.insert(position, smalls[index]);
	}

	numbers = mainChain;
}

// Same algorithm as sortVector, but with std::deque
void PmergeMe::sortDeque(std::deque<int> &numbers)
{
	if (numbers.size() < 2)
		return;

	// 1. Make pairs (big, small)
	std::deque<std::pair<int, int> > pairs;
	for (size_t i = 0; i + 1 < numbers.size(); i += 2)
	{
		int a = numbers[i];
		int b = numbers[i + 1];
		if (a < b)
			std::swap(a, b);
		pairs.push_back(std::make_pair(a, b));
	}

	// 2. Sort the bigs
	std::deque<int> bigs;
	for (size_t i = 0; i < pairs.size(); i++)
		bigs.push_back(pairs[i].first);
	sortDeque(bigs);

	// 3. Smalls in the same order as the sorted bigs
	std::deque<int> smalls;
	for (size_t i = 0; i < bigs.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (pairs[j].first == bigs[i])
			{
				smalls.push_back(pairs[j].second);
				pairs.erase(pairs.begin() + j);
				break;
			}
		}
	}
	if (numbers.size() % 2 == 1)
		smalls.push_back(numbers.back()); // odd number has no partner

	// 4. Main chain
	std::deque<int> mainChain;
	mainChain.push_back(smalls[0]);
	mainChain.insert(mainChain.end(), bigs.begin(), bigs.end());

	// 5. Insert the rest
	std::vector<size_t> order = insertionOrder(smalls.size());
	for (size_t i = 0; i < order.size(); i++)
	{
		size_t index = order[i];
		std::deque<int>::iterator limit = mainChain.end();
		if (index < bigs.size())
			limit = std::find(mainChain.begin(), mainChain.end(), bigs[index]);

		std::deque<int>::iterator position = std::lower_bound(mainChain.begin(), limit, smalls[index]);
		mainChain.insert(position, smalls[index]);
	}

	numbers = mainChain;
}

void PmergeMe::run(int argc, char **argv)
{
	if (argc < 2)
		throw std::runtime_error("no numbers");

	// Vector: read numbers + sort (both are timed)
	clock_t start = clock();
	for (int i = 1; i < argc; i++)
		_vector.push_back(toNumber(argv[i]));
	sortVector(_vector);
	double vectorTime = (double)(clock() - start) / CLOCKS_PER_SEC * 1000000;

	// Deque: read numbers + sort (both are timed)
	start = clock();
	for (int i = 1; i < argc; i++)
		_deque.push_back(toNumber(argv[i]));
	sortDeque(_deque);
	double dequeTime = (double)(clock() - start) / CLOCKS_PER_SEC * 1000000;

	std::cout << "Before:";
	for (int i = 1; i < argc; i++)
		std::cout << " " << argv[i];
	std::cout << std::endl;

	std::cout << "After:";
	for (size_t i = 0; i < _vector.size(); i++)
		std::cout << " " << _vector[i];
	std::cout << std::endl;

	std::cout << "Time to process a range of " << _vector.size()
		<< " elements with std::vector : " << vectorTime << " us" << std::endl;
	std::cout << "Time to process a range of " << _deque.size()
		<< " elements with std::deque  : " << dequeTime << " us" << std::endl;
}
