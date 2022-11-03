#pragma once

#include "Instruction.h"

class TwoByteInstructions {
	TwoByteInstructions() = delete;
public:
	static Instruction* readInstruction(unsigned char*& ip);
	static const unsigned char opcode;
};

