#pragma once

#include "Instruction.h"
#include "Operands/Expression.h"

class ExtendedOpInstruction : public Instruction {
protected:
	const unsigned char type;
	Expression target;

	ExtendedOpInstruction(unsigned char opcode, const std::string names[8], unsigned char*& ip);
public:
	virtual std::string toString() override;
};
