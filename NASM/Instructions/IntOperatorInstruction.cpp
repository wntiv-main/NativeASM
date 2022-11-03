#include "IntOperatorInstruction.h"

const std::string IntOperatorInstruction::names[8] = { "add", "or", "adc", "sbb", "and", "sub", "xor", "cmp" };

IntOperatorInstruction::IntOperatorInstruction(unsigned char*& ip)
	: ExtendedOpInstruction(opcode, names, ip) {
	operand2 = *(int*)ip;
	ip += 4;
}

Instruction* IntOperatorInstruction::create(unsigned char*& ip) {
	return new IntOperatorInstruction(ip);
}

std::string IntOperatorInstruction::toString() {
	return ExtendedOpInstruction::toString() + ", " + hexString(operand2);
}

const unsigned char IntOperatorInstruction::opcode = 0x81;
