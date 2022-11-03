#pragma once

#include "Instruction.h"
#include "Operands/Expression.h"
#include "Operands/Register.h"

class RegRegInstruction : public Instruction {
protected:
	Register dest;
	Expression source;

	RegRegInstruction(unsigned char opcode, std::string name, unsigned char*& ip);
public:
	virtual std::string toString() override;
	std::string toString(std::string opSep, std::string argSep);
};
