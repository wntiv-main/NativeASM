#pragma once

#include "Instruction.h"

class JumpIfNotEqualInstruction : public Instruction {
protected:
	int offset;
	unsigned char* target;

	JumpIfNotEqualInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
