#pragma once
#include "Instruction.h"
class JumpIfEqualInstruction : public Instruction {
protected:
	int offset;
	unsigned char* target;

	JumpIfEqualInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
