#pragma once

#include "RegRegInstruction.h"

class InverseMoveInstruction : public RegRegInstruction {
protected:
	InverseMoveInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
