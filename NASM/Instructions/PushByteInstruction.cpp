#include "PushByteInstruction.h"

PushByteInstruction::PushByteInstruction(unsigned char*& ip)
	: Instruction(opcode, "push byte") {
	byte = *(char*)(ip++);
}

std::string PushByteInstruction::toString() {
	return Instruction::toString() + " 0x" + hexString((int)byte);
}

Instruction* PushByteInstruction::create(unsigned char*& ip) {
	return new PushByteInstruction(ip);
}

const unsigned char PushByteInstruction::opcode = 0x6a;
