#include "PushIntInstruction.h"

PushIntInstruction::PushIntInstruction(unsigned char*& ip)
	: Instruction(opcode, "push int") {
	value = *(int*)ip;
	ip += 4;
}

std::string PushIntInstruction::toString() {
	return Instruction::toString() + " 0x" + hexString(value);
}

Instruction* PushIntInstruction::create(unsigned char*& ip) {
	return new PushIntInstruction(ip);
}

const unsigned char PushIntInstruction::opcode = 0x68;
