#pragma once

#include "Instruction.h"

class JumpInstruction : public Instruction {
protected:
	int offset;
	unsigned char* target;

	JumpInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
