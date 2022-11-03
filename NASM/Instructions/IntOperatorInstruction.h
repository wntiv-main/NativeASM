#pragma once

#include "ExtendedOpInstruction.h"

#include <string>

class IntOperatorInstruction : public ExtendedOpInstruction {
protected:
	static const std::string names[8];
	int operand2;

	IntOperatorInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
