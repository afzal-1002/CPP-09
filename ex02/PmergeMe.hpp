#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <string>
#include <vector>

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	void run(int argc, char **argv);

private:
	std::vector<int> _vector;
	std::deque<int> _deque;

	int toNumber(const std::string &text);
	std::vector<size_t> insertionOrder(size_t count);
	void sortVector(std::vector<int> &numbers);
	void sortDeque(std::deque<int> &numbers);
};

#endif
