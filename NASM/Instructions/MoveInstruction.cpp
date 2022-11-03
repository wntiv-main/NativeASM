#include "MoveInstruction.h"



MoveInstruction::MoveInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "mov", ip) {}

std::string MoveInstruction::toString() {
	return RegRegInstruction::toString(" ", " <- ");
}

Instruction* MoveInstruction::create(unsigned char*& ip) {
	return new MoveInstruction(ip);
}

const unsigned char MoveInstruction::opcode = 0x8b;
