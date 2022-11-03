#include "MoveEAXInstruction.h"

MoveEAXInstruction::MoveEAXInstruction(unsigned char*& ip)
	: Instruction(opcode, "mov") {
	constant = *(int*)ip;
	ip += 4;
}

std::string MoveEAXInstruction::toString() {
	return Instruction::toString() + " eax <- " + hexString(constant);
}

Instruction* MoveEAXInstruction::create(unsigned char*& ip) {
	return new MoveEAXInstruction(ip);
}

const unsigned char MoveEAXInstruction::opcode = 0xa1;
