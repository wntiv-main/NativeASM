#include "NoOpInstruction.h"

NoOpInstruction::NoOpInstruction(unsigned char*& ip)
	: Instruction(opcode, "nop") {}

Instruction* NoOpInstruction::create(unsigned char*& ip) {
	return new NoOpInstruction(ip);
}

const unsigned char NoOpInstruction::opcode = 0x90;
