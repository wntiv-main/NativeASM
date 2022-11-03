#pragma once

class Instruction;

class PrefixOpcode {
private:
	bool valid;
	unsigned char opcode = 0xcc;
public:
	PrefixOpcode();
	PrefixOpcode(unsigned char*& ip);
	
	bool isValid();
	unsigned char getCode();

	static Instruction* create(unsigned char*& ip);
};

