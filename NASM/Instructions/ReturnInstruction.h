#pragma once

#include "Instruction.h"

class ReturnInstruction : public Instruction {
protected:
	ReturnInstruction(unsigned char*& ip);
public:
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};