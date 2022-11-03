#include "InverseCompareInstruction.h"

InverseCompareInstruction::InverseCompareInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "cmp", ip) {}

std::string InverseCompareInstruction::toString() {
	return RegRegInstruction::toString(" ", " <& ");
}

Instruction* InverseCompareInstruction::create(unsigned char*& ip) {
	return new InverseCompareInstruction(ip);
}

const unsigned char InverseCompareInstruction::opcode = 0x3b;
