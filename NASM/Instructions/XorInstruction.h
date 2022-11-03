#pragma once

#include "RegRegInstruction.h"

class XorInstruction : public RegRegInstruction {
protected:
	XorInstruction(unsigned char*& ip);
public:
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};

