#pragma once

#include "Operand.h"

#include <vector>

#include "InstructionPrefix.h"

class Instruction {
private:
	static const std::string arithmaticNames[8];
	static const std::string extMathNames[8];
	static const std::string shiftNames[8];
	static const std::string conditionalNames[16];
	static const std::string utilNames[8];
	Operand operands[4] = { {}, {}, {}, {} };
	std::string name = "";
	unsigned char opcode = 0;
	unsigned char extOpcode = 0;
	std::vector<InstructionPrefix> prefixes;
	bool hasPrefix(unsigned char prefix);
public:
	static bool parser64BitMode;
	Instruction();
	Instruction(unsigned char*& ip);
	void handleArithmatic(unsigned char*& ip);
	unsigned char getOpcode();
	Operand* getOperands();
};
