#pragma once

#include "RegRegInstruction.h"

class InverseCompareInstruction : public RegRegInstruction {
protected:
	InverseCompareInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
