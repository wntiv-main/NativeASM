#pragma once

#include "Instruction.h"
#include "Operands/Register.h"

class PopRegisterInstruction : public Instruction {
protected:
	Register target;

	PopRegisterInstruction(unsigned char opcode);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};

