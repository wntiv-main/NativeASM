#include "InverseMoveEAXInstruction.h"

InverseMoveEAXInstruction::InverseMoveEAXInstruction(unsigned char*& ip)
	: Instruction(opcode, "mov") {
	constant = *(int*)ip;
	ip += 4;
}

std::string InverseMoveEAXInstruction::toString() {
	return Instruction::toString() + " <- " + hexString(constant);
}

Instruction* InverseMoveEAXInstruction::create(unsigned char*& ip) {
	return new InverseMoveEAXInstruction(ip);
}

const unsigned char InverseMoveEAXInstruction::opcode = 0xa3;
