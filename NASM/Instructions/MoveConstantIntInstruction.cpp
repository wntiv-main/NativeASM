#include "MoveConstantIntInstruction.h"

MoveConstantIntInstruction::MoveConstantIntInstruction(unsigned char*& ip)
	: Instruction(*(ip - 1), "mov") {
	target = *(ip - 1) & 0b111;
	value = *(int*)ip;
	ip += 4;
}

Instruction* MoveConstantIntInstruction::create(unsigned char*& ip) {
	return new MoveConstantIntInstruction(ip);
}

std::string MoveConstantIntInstruction::toString() {
	return Instruction::toString() + " " + target.toString() + " <- " + hexString(value);
}

const unsigned char MoveConstantIntInstruction::opcode = 0xb8;