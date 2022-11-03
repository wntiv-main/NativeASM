#pragma once

#include "ExtendedOpInstruction.h"

class FFInstruction : public ExtendedOpInstruction {
protected:
	static const std::string names[8];
	FFInstruction(unsigned char*& ip);

	unsigned char* targetAddr;
public:
	virtual std::string toString() override;
	virtual std::string toString(std::string indent) override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
