#pragma once
#include "RegRegInstruction.h"
class LoadEffectiveAddressInstruction : public RegRegInstruction {
protected:
	LoadEffectiveAddressInstruction(unsigned char*& ip);
public:
	virtual std::string toString() override;
	static Instruction* create(unsigned char*& ip);
	static const unsigned char opcode;
};
