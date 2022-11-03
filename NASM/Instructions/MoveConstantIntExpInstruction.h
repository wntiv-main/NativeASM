#pragma once

#include "Instruction.h"

#include "Operands/ScaledIndexByte.h"

class MoveConstantIntExpInstruction : public Instruction {
protected:
	ScaledIndexByte target;
	int value;

	MoveConstantIntExpInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
