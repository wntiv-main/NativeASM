#include "JumpIfBelowInstruction.h"

JumpIfBelowInstruction::JumpIfBelowInstruction(unsigned char*& ip)
	: Instruction(opcode, "jb") {
	offset = (int)*(char*)ip;
	ip++;
	target = ip + offset;
}

std::string JumpIfBelowInstruction::toString() {
	return Instruction::toString() + " -> 0x" + hexString((size_t)target);
}

Instruction* JumpIfBelowInstruction::create(unsigned char*& ip) {
	return new JumpIfBelowInstruction(ip);
}

const unsigned char JumpIfBelowInstruction::opcode = 0x72;
