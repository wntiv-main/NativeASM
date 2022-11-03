#pragma once
#include "Instruction.h"
class StoreStringInstruction : public Instruction {
protected:
	StoreStringInstruction(unsigned char*& ip);
public:
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
