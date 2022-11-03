#pragma once
#include "ExtendedOpInstruction.h"

class MixOperatorInstruction : public ExtendedOpInstruction {
protected:
	static const std::string names[8];
	int operand2;

	MixOperatorInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
