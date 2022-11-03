#pragma once

#include <string>
#include <sstream>

class Instruction;

#include "Prefixes/PrefixOpcode.h"

std::string hexString(unsigned long value);

class Instruction {
protected:
	const unsigned char opcode;
	const std::string name;

	Instruction(unsigned char opcode, std::string name);
public:
	PrefixOpcode prefix;
	virtual std::string toString();
	virtual std::string toString(std::string indent);
	unsigned char getOpcode();
};
