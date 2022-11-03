#include "TestInstruction.h"

TestInstruction::TestInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "test", ip) {}

std::string TestInstruction::toString() {
	return RegRegInstruction::toString(" ", " & ");
}

Instruction* TestInstruction::create(unsigned char*& ip) {
	return new TestInstruction(ip);
}

const unsigned char TestInstruction::opcode = 0x85;
