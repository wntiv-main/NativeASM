#include "Instruction.h"

std::string hexString(unsigned long value) {
	std::ostringstream strm;
	strm << std::hex << value;
	return strm.str();
}

Instruction::Instruction(unsigned char opcode, std::string name) : opcode(opcode), name(name) {}

std::string Instruction::toString() {
	return (prefix.isValid() ? hexString(prefix.getCode() & 0xff) + " " : "") + name;
}

std::string Instruction::toString(std::string indent) {
	return indent + toString();
}

unsigned char Instruction::getOpcode() {
	return opcode;
}
