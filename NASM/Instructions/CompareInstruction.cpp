#include "CompareInstruction.h"

CompareInstruction::CompareInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "cmp", ip) {}

std::string CompareInstruction::toString() {
	return RegRegInstruction::toString(" ", " & ");
}

Instruction* CompareInstruction::create(unsigned char*& ip) {
	return new CompareInstruction(ip);
}

const unsigned char CompareInstruction::opcode = 0x39;
