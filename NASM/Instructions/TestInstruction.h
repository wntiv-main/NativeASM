#pragma once

#include "RegRegInstruction.h"

class TestInstruction : public RegRegInstruction {
protected:
	TestInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
