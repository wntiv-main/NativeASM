#include "PrefixOpcode.h"

#include "../Instruction.h"
#include "../Instructions.h"

PrefixOpcode::PrefixOpcode() : valid(false) {}

PrefixOpcode::PrefixOpcode(unsigned char*& ip) : valid(true) {
	opcode = *(ip - 1);
}

bool PrefixOpcode::isValid() {
	return valid;
}

Instruction* PrefixOpcode::create(unsigned char*& ip) {
	PrefixOpcode prefix = PrefixOpcode(ip);
	Instruction* instr = Instructions::readInstruction(ip);
	instr->prefix = prefix;
	return instr;
}

unsigned char PrefixOpcode::getCode() {
	return opcode;
}
