#include "CallInstruction.h"

#include <sstream>
#include "../FunctionReader.h"

CallInstruction::CallInstruction(unsigned char*& ip)
	: Instruction(opcode, "call") {
	offset = *(int*)ip;
	ip += 4;
	target = ip + offset;
}

Instruction* CallInstruction::create(unsigned char*& ip) {
	return new CallInstruction(ip);
}

std::string CallInstruction::toString() {
	return toString("");
}

std::string CallInstruction::toString(std::string indent) {
	std::ostringstream str;
	str << indent << Instruction::toString() << " -> 0x" << hexString((size_t)target);
	FunctionReader::read3(*(FunctionDescriptor::UnknownFunctionPtr)target, "0x" + hexString((size_t)target), std::cout, indent + "    ");
	return str.str();
}

const unsigned char CallInstruction::opcode = 0xe8;