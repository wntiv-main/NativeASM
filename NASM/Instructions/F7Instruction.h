#pragma once

#include "ExtendedOpInstruction.h"

class F7Instruction : public ExtendedOpInstruction {
protected:
	static const std::string names[8];
	F7Instruction(unsigned char*& ip);

	int immediate;
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
