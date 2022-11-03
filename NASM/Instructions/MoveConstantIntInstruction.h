#pragma once

#include "Instruction.h"
#include "Operands/Register.h"

class MoveConstantIntInstruction : public Instruction {
protected:
	Register target;
	int value;

	MoveConstantIntInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};

