#pragma once

#include "Instruction.h"

class CallInstruction : public Instruction {
protected:
	int offset;
	unsigned char* target;

	CallInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	virtual std::string toString(std::string indent) override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};

