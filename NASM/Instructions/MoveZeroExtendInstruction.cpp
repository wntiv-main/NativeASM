#include "MoveZeroExtendInstruction.h"

MoveZeroExtendInstruction::MoveZeroExtendInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "movzx", ip) {}

Instruction* MoveZeroExtendInstruction::create(unsigned char*& ip) {
	return new MoveZeroExtendInstruction(ip);
}

const unsigned char MoveZeroExtendInstruction::opcode = 0xb6;
