#include "TwoByteInstructions.h"

#include <iostream>

#include "MoveZeroExtendInstruction.h"

Instruction* TwoByteInstructions::readInstruction(unsigned char*& ip) {
	if (*ip == MoveZeroExtendInstruction::opcode) return MoveZeroExtendInstruction::create(++ip);


	std::cerr << std::hex << "Two-byte opcode 0F" << (int)*ip << " not implemented at 0x" << (size_t)ip;
	throw std::runtime_error("Not implemented");
}

const unsigned char TwoByteInstructions::opcode = 0x0f;