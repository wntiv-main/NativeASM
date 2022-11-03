#include "JumpByteInstruction.h"

JumpByteInstruction::JumpByteInstruction(unsigned char*& ip)
	: Instruction(opcode, "jmp") {
	offset = (int)*(char*)ip;
	ip++;
	ip += offset;
	target = ip;
}

Instruction* JumpByteInstruction::create(unsigned char*& ip) {
	return new JumpByteInstruction(ip);
}

std::string JumpByteInstruction::toString() {
	return Instruction::toString() + " -> 0x" + hexString((size_t)target);
}

const unsigned char JumpByteInstruction::opcode = 0xeb;