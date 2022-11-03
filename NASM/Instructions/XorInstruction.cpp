#include "XorInstruction.h"

XorInstruction::XorInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "xor", ip) {}

Instruction* XorInstruction::create(unsigned char*& ip) {
	return new XorInstruction(ip);
}

const unsigned char XorInstruction::opcode = 0x33;