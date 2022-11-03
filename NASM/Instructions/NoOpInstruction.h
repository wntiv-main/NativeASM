#pragma once
#include "Instruction.h"
class NoOpInstruction : public Instruction {
protected:
	NoOpInstruction(unsigned char*& ip);
public:
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};