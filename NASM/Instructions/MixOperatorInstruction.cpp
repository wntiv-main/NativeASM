#include "MixOperatorInstruction.h"

const std::string MixOperatorInstruction::names[8] = { "add", "or", "adc", "sbb", "and", "sub", "xor", "cmp" };

MixOperatorInstruction::MixOperatorInstruction(unsigned char*& ip)
	: ExtendedOpInstruction(opcode, names, ip) {
	operand2 = *ip & 0xff;
	ip++;
}

Instruction* MixOperatorInstruction::create(unsigned char*& ip) {
	return new MixOperatorInstruction(ip);
}

std::string MixOperatorInstruction::toString() {
	return ExtendedOpInstruction::toString() + ", " + hexString(operand2);
}

const unsigned char MixOperatorInstruction::opcode = 0x83;
