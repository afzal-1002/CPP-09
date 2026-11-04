#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>
#include <cctype>
#include <sstream>
#include <stdexcept>



/*
The program evaluates math written in Reverse Polish Notation, 
where the operator comes after its two numbers: 3 4 + means 3 + 4. 
It works with a stack: numbers go on top, and an operator takes the 
top two off, combines them, and puts the result back.
 */


class RPN
{

private:
	std::list<long> stack; // used like a stack: push_back / back / pop_back
	long popNumber();

public:
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();

	long calculate(const std::string &expression);
};

#endif
