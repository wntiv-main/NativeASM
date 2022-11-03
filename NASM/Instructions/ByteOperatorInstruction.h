#pragma once
#include "ExtendedOpInstruction.h"

class ByteOperatorInstruction : public ExtendedOpInstruction {
protected:
	static const std::string names[8];
	int operand2;

	ByteOperatorInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
	static const unsigned char aliasOpcode;
};
