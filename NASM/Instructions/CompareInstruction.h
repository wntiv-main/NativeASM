#pragma once
#include "RegRegInstruction.h"
class CompareInstruction : public RegRegInstruction {
protected:
	CompareInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
