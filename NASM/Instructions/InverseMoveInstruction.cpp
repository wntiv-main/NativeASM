#include "InverseMoveInstruction.h"

InverseMoveInstruction::InverseMoveInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "mov", ip) {}

std::string InverseMoveInstruction::toString() {
	return RegRegInstruction::toString(" ", " -> ");
}

Instruction* InverseMoveInstruction::create(unsigned char*& ip) {
	return new InverseMoveInstruction(ip);
}

const unsigned char InverseMoveInstruction::opcode = 0x89;
