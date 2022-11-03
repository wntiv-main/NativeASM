#include "JumpInstruction.h"

JumpInstruction::JumpInstruction(unsigned char*& ip)
	: Instruction(opcode, "jmp") {
	offset = *(int*)ip;
	ip += 4;
	ip += offset;
	target = ip;
}

Instruction* JumpInstruction::create(unsigned char*& ip) {
	return new JumpInstruction(ip);
}

std::string JumpInstruction::toString() {
	return Instruction::toString() + " -> 0x" + hexString((size_t)target);
}

const unsigned char JumpInstruction::opcode = 0xe9;