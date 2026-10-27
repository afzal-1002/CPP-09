#include "RPN.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(const RPN &other) : stack(other.stack)
{
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		stack = other.stack;
	return *this;
}

RPN::~RPN()
{
}

// Take the top number out of the stack
long RPN::popNumber()
{
	if (stack.empty())
		throw std::runtime_error("not enough numbers");
	long number = stack.back();
	stack.pop_back();
	return number;
}

// Example: "3 4 +"  ->  push 3, push 4, see '+', pop 4 and 3, push 7
long RPN::calculate(const std::string &expression)
{
	std::istringstream stream(expression);
	std::string token;

	stack.clear();
	while (stream >> token)
	{
		if (token.size() != 1)
			throw std::runtime_error("bad token");

		char c = token[0];
		if (std::isdigit(c))
		{
			stack.push_back(c - '0');
			continue;
		}

		long right = popNumber();
		long left = popNumber();

		if (c == '+')
			stack.push_back(left + right);
		else if (c == '-')
			stack.push_back(left - right);
		else if (c == '*')
			stack.push_back(left * right);
		else if (c == '/')
		{
			if (right == 0)
				throw std::runtime_error("division by zero");
			stack.push_back(left / right);
		}
		else
			throw std::runtime_error("bad token");
	}

	// At the end exactly one number must be left: the result
	if (stack.size() != 1)
		throw std::runtime_error("bad expression");
	return stack.back();
}
