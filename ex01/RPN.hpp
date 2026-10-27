#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>

class RPN
{
public:
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();

	long calculate(const std::string &expression);

private:
	std::list<long> stack; // used like a stack: push_back / back / pop_back

	long popNumber();
};

#endif
