#pragma once

#include "RegRegInstruction.h"

class MoveZeroExtendInstruction : public RegRegInstruction {
protected:
	MoveZeroExtendInstruction(unsigned char*& ip);
public:
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
