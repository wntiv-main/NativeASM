#pragma once

#include "Register.h"
#include <string>

class Expression {
private:
	bool valid;
	bool ext = false;
	Register base;
	Register index;
	unsigned char scale = 1;
	int addrOffset = 0;
	int valueOffset = 0;
public:
	Expression();
	Expression(unsigned char*& ip);
	std::string toString();
	bool isValid();
	bool isMemory();
	bool isConstant();
	int getValueOffset();
};

