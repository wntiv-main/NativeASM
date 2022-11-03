#pragma once

#include "Instruction.h"

class PushIntInstruction : public Instruction {
protected:
	int value;

	PushIntInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
