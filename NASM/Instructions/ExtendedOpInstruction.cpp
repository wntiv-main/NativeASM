#include "ExtendedOpInstruction.h"

#include <sstream>

ExtendedOpInstruction::ExtendedOpInstruction(unsigned char opcode, const std::string names[8], unsigned char*& ip)
	: type((*ip >> 3) & 0b111),
	  Instruction(opcode, names[(*ip >> 3) & 0b111]) {
	target = Expression(ip);
}

std::string ExtendedOpInstruction::toString() {
	std::ostringstream str;
	str << Instruction::toString() << "/" << (int)type << " " << target.toString();
	return str.str();
}
