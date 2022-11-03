#pragma once
#include "RegRegInstruction.h"
class MoveInstruction : public RegRegInstruction {
protected:
	MoveInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
