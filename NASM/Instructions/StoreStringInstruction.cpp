#include "StoreStringInstruction.h"

StoreStringInstruction::StoreStringInstruction(unsigned char*& ip)
	: Instruction(opcode, "stos") {}

Instruction* StoreStringInstruction::create(unsigned char*& ip) {
	return new StoreStringInstruction(ip);
}

const unsigned char StoreStringInstruction::opcode = 0xab;