#include "RegRegInstruction.h"

#include <sstream>

RegRegInstruction::RegRegInstruction(unsigned char opcode, std::string name, unsigned char*& ip)
	: Instruction(opcode, name) {
	dest = (*ip >> 3) & 0b111;
	source = Expression(ip);
}

std::string RegRegInstruction::toString() {
	std::ostringstream str;
	str << Instruction::toString() << " " << dest.toString() << ", " << source.toString();
	return str.str();
}

std::string RegRegInstruction::toString(std::string opSep, std::string argSep) {
	std::ostringstream str;
	str << Instruction::toString() << opSep << dest.toString() << argSep << source.toString();
	return str.str();
}
