#pragma once

#include "Instruction.h"

class PushByteInstruction : public Instruction {
protected:
	char byte;

	PushByteInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
