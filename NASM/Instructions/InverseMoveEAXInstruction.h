#pragma once

#include "Instruction.h"

class InverseMoveEAXInstruction : public Instruction {
protected:
	int constant;

	InverseMoveEAXInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
