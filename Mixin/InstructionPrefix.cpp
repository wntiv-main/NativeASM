#include "InstructionPrefix.h"

const InstructionPrefix InstructionPrefix::nullPrefix = InstructionPrefix();

InstructionPrefix::InstructionPrefix(unsigned char opcode)
	: opcode(opcode) {}

InstructionPrefix InstructionPrefix::getPrefix(unsigned char opcode, bool is64Bit) {
	if(opcode == 0x26
		|| opcode == 0x2e
		|| opcode == 0x36
		|| opcode == 0x3e
		|| opcode == 0x64
		|| opcode == 0x65
		|| opcode == 0x66
		|| opcode == 0x67
		|| opcode == 0x9b
		|| opcode == 0xf0
		|| opcode == 0xf2
		|| opcode == 0xf3
		|| (is64Bit && opcode >= 0x40 && opcode < 0x50)) {
		return InstructionPrefix(opcode);
	}
	return nullPrefix;
}

bool InstructionPrefix::isValid() {
	return opcode != 0x00;
}

unsigned char InstructionPrefix::getPrefix() {
	return opcode;
}
