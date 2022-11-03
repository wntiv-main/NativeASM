#include "PopRegisterInstruction.h"

PopRegisterInstruction::PopRegisterInstruction(unsigned char opcode)
	: Instruction(opcode, "pop") {
	target = opcode & 0b111;
}

Instruction* PopRegisterInstruction::create(unsigned char*& ip) {
	return new PopRegisterInstruction(*(ip - 1));
}

std::string PopRegisterInstruction::toString() {
	return Instruction::toString() + " -> " + target.toString();
}

const unsigned char PopRegisterInstruction::opcode = 0x58;