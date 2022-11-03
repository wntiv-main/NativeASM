#pragma once

#include "Instruction.h"

class JumpIfBelowInstruction : public Instruction {
protected:
	int offset;
	unsigned char* target;

	JumpIfBelowInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
