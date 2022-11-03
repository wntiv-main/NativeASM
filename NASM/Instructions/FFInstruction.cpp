#include "FFInstruction.h"

#include <sstream>

#include "../FunctionReader.h"

const std::string FFInstruction::names[8] = {"inc", "dec", "call", "callf", "jmp", "jmpf", "push", "UNDEFINED"};

FFInstruction::FFInstruction(unsigned char*& ip)
	: ExtendedOpInstruction(opcode, names, ip) {
	if((type == 3 || type == 5) && !target.isMemory())
		throw std::runtime_error("Invalid Operand");
	if(type > 1 && type < 6 && target.isConstant()) {
		targetAddr = ip + target.getValueOffset();
		if(type > 3) ip = targetAddr;
	}
}

std::string FFInstruction::toString() {
	return toString("");
}

std::string FFInstruction::toString(std::string indent) {
	if(type < 2)
		return indent + ExtendedOpInstruction::toString();
	if(type < 4)
		return indent + Instruction::toString() + " -> " + target.toString();
	if(type < 6) {
		if(target.isConstant()) {
			std::ostringstream str;
			str << indent << Instruction::toString() + " -> " + target.toString();
			//FunctionReader::read3(*(FunctionDescriptor::UnknownFunctionPtr)(targetAddr), "0x" + hexString((size_t)targetAddr), str, indent + "    ");
			return str.str();
		} else return indent + Instruction::toString() + " -> " + target.toString();
	}
	return indent + Instruction::toString() + " <- " + target.toString();
}

Instruction* FFInstruction::create(unsigned char*& ip) {
	return new FFInstruction(ip);
}

const unsigned char FFInstruction::opcode = 0xff;
