#include "PushRegisterInstruction.h"

PushRegisterInstruction::PushRegisterInstruction(unsigned char opcode)
	: Instruction(opcode, "push") {
	target = opcode & 0b111;
}

Instruction* PushRegisterInstruction::create(unsigned char*& ip) {
	return new PushRegisterInstruction(*(ip - 1));
}

std::string PushRegisterInstruction::toString() {
	return Instruction::toString() + " <- " + target.toString();
}

const unsigned char PushRegisterInstruction::opcode = 0x50;