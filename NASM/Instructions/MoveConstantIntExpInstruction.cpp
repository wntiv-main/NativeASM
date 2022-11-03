#include "MoveConstantIntExpInstruction.h"

MoveConstantIntExpInstruction::MoveConstantIntExpInstruction(unsigned char*& ip)
	: Instruction(opcode, "mov") {
	target = ScaledIndexByte(ip);
	value = *(int*)ip;
	ip += 4;
}

std::string MoveConstantIntExpInstruction::toString() {
	return Instruction::toString() + " [" + target.toString() + "] <- 0x" + hexString(value);
}

Instruction* MoveConstantIntExpInstruction::create(unsigned char*& ip) {
	return new MoveConstantIntExpInstruction(ip);
}

const unsigned char MoveConstantIntExpInstruction::opcode = 0xc7;
