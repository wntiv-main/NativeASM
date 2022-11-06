#pragma once

class InstructionPrefix {
private:
	static const InstructionPrefix nullPrefix;
	unsigned char opcode = 0x00;
	InstructionPrefix(unsigned char opcode);
public:
	InstructionPrefix() = default;
	static InstructionPrefix getPrefix(unsigned char opcode, bool is64Bit);

	bool isValid();
	unsigned char getPrefix();
};
