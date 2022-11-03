#include "JumpIfEqualInstruction.h"

JumpIfEqualInstruction::JumpIfEqualInstruction(unsigned char*& ip)
	: Instruction(opcode, "je") {
	offset = (int)*(char*)ip;
	ip++;
	target = ip + offset;
}

std::string JumpIfEqualInstruction::toString() {
	return Instruction::toString() + " -> 0x" + hexString((size_t)target);
}

Instruction* JumpIfEqualInstruction::create(unsigned char*& ip) {
	return new JumpIfEqualInstruction(ip);
}

const unsigned char JumpIfEqualInstruction::opcode = 0x74;
