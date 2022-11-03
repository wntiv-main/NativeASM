#include "ByteOperatorInstruction.h"

const std::string ByteOperatorInstruction::names[8] = { "add", "or", "adc", "sbb", "and", "sub", "xor", "cmp" };

ByteOperatorInstruction::ByteOperatorInstruction(unsigned char*& ip)
	: ExtendedOpInstruction(opcode, names, ip) {
	operand2 = *ip & 0xff;
	ip++;
}

Instruction* ByteOperatorInstruction::create(unsigned char*& ip) {
	return new ByteOperatorInstruction(ip);
}

std::string ByteOperatorInstruction::toString() {
	return ExtendedOpInstruction::toString() + ", " + hexString(operand2);
}

const unsigned char ByteOperatorInstruction::opcode = 0x80;
const unsigned char ByteOperatorInstruction::aliasOpcode = 0x82;
