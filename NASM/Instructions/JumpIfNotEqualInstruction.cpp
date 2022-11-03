#include "JumpIfNotEqualInstruction.h"

JumpIfNotEqualInstruction::JumpIfNotEqualInstruction(unsigned char*& ip)
	: Instruction(opcode, "jne") {
	offset = (int)*(char*)ip;
	ip++;
	target = ip + offset;
}

std::string JumpIfNotEqualInstruction::toString() {
	return Instruction::toString() + " -> 0x" + hexString((size_t)target);
}

Instruction* JumpIfNotEqualInstruction::create(unsigned char*& ip) {
	return new JumpIfNotEqualInstruction(ip);
}

const unsigned char JumpIfNotEqualInstruction::opcode = 0x75;
