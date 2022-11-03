#include "F7Instruction.h"

const std::string F7Instruction::names[8] = {"test", "test", "not", "neg", "mul", "imul", "div", "idiv"};

F7Instruction::F7Instruction(unsigned char*& ip)
	: ExtendedOpInstruction(opcode, names, ip) {
	if(type < 2) {
		immediate = *(int*)ip;
		ip += 4;
	}
}

std::string F7Instruction::toString() {
	if(type < 2)
		return ExtendedOpInstruction::toString() + " & 0x" + hexString(immediate);
	if(type < 4)
		return ExtendedOpInstruction::toString();
	if(type < 6)
		return Instruction::toString() + " edx:eax <- eax * (" + target.toString() + ")";
	return Instruction::toString() + " eax(quot), edx(rem) <- edx:eax / (" + target.toString() + ")";
}

Instruction* F7Instruction::create(unsigned char*& ip) {
	return new F7Instruction(ip);
}

const unsigned char F7Instruction::opcode = 0xf7;
