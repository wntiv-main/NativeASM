#include "ReturnInstruction.h"

ReturnInstruction::ReturnInstruction(unsigned char*& ip)
	: Instruction(opcode, "retn") {}

Instruction* ReturnInstruction::create(unsigned char*& ip) {
	return new ReturnInstruction(ip);
}

const unsigned char ReturnInstruction::opcode = 0xc3;
