#pragma once
#include "Instruction.h"
class MoveEAXInstruction : public Instruction {
protected:
	int constant;

	MoveEAXInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
