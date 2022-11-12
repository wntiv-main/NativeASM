#include "Instruction.h"

#include <iostream>

bool Instruction::parser64BitMode = false;
const std::string Instruction::arithmaticNames[8] = { "add", "or", "adc", "sbb", "and", "sub", "xor", "cmp" };
const std::string Instruction::extMathNames[8] = { "test", "test", "not", "neg", "mul", "imul", "div", "idiv" };
const std::string Instruction::shiftNames[8] = { "rol", "ror", "rcl", "rcr", "shl", "shr", "sal", "sar" };
const std::string Instruction::conditionalNames[16] = { "jo", "jno", "jb", "jae", "je", "jne", "jbe", "ja", "js", "jns", "jpe", "jpo", "jl", "jge", "jle", "jg" };
const std::string Instruction::utilNames[8] = { "inc", "dec", "call", "callf", "jmp", "jmpf", "push", "UNDEFINED" };

bool range(int lower, int value, int range) {
	return value >= lower && value < lower + range;
}

bool Instruction::hasPrefix(unsigned char prefix) {
	for(InstructionPrefix p : prefixes) {
		if(p.getPrefix() == prefix) return true;
	}
	return false;
}

Instruction::Instruction()
	: opcode(0xcc) {}

Instruction::Instruction(unsigned char*& ip) {
	sourceAddr = ip;
	parseInstruction(ip);
	byteLength = ip - sourceAddr;
}

void Instruction::handleArithmatic(unsigned char*& ip) {
	unsigned char relativeOpcode = opcode & 0x07;
	if(relativeOpcode == 0x00) {
		Operand::Builder::doubleReg(ip, 1, 1, operands + 1, operands);
		return;
	}
	if(relativeOpcode == 0x01) {
		Operand::Builder::doubleReg(ip, 4, 4, operands + 1, operands);
		return;
	}
	if(relativeOpcode == 0x02) {
		Operand::Builder::doubleReg(ip, 1, 1, operands, operands + 1);
		return;
	}
	if(relativeOpcode == 0x03) {
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		return;
	}
	if(relativeOpcode == 0x04) {
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(relativeOpcode == 0x05) {
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
	}
}

void Instruction::parseInstruction(unsigned char*& ip) {
	InstructionPrefix prefix;
	do {
		prefix = InstructionPrefix::getPrefix(*ip, parser64BitMode);
		if(prefix.isValid()) {
			prefixes.push_back(prefix);
			ip++;
		}
	} while(prefix.isValid());
	opcode = *ip;
	ip++;
	if(range(0x00, opcode, 6)) {
		name = "add";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x06) {
		name = "push";
		operands[0] = Operand(Register(Register::ES), 4);
		return;
	}
	if(opcode == 0x07) {
		name = "pop";
		operands[0] = Operand(Register(Register::ES), 4);
		return;
	}
	if(range(0x08, opcode, 6)) {
		name = "or";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x0e) {
		name = "push";
		operands[0] = Operand(Register(Register::CS), 4);
		return;
	}
	if(opcode == 0x0f) {
		secondOpcode = *ip;
		ip++;
		if(secondOpcode == 0xb6) {
			name = "movzx";
			Operand::Builder::doubleReg(ip, 1, 1, operands, operands + 1);
			return;
		}
		if(range(0x80, secondOpcode, 0x10)) {
			name = conditionalNames[secondOpcode - 0x80];
			Operand::Builder(ip).readImmediate(4).build();
			return;
		}
		throw "TWO BYTES GO AWAY";
		return;
	}
	if(range(0x10, opcode, 6)) {
		name = "adc";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x16) {
		name = "push";
		operands[0] = Operand(Register(Register::SS), 4);
		return;
	}
	if(opcode == 0x17) {
		name = "pop";
		operands[0] = Operand(Register(Register::SS), 4);
		return;
	}
	if(range(0x18, opcode, 6)) {
		name = "sbb";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x1e) {
		name = "push";
		operands[0] = Operand(Register(Register::DS), 4);
		return;
	}
	if(opcode == 0x1f) {
		name = "pop";
		operands[0] = Operand(Register(Register::DS), 4);
		return;
	}
	if(range(0x20, opcode, 6)) {
		name = "and";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x27) {
		name = "daa";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(range(0x28, opcode, 6)) {
		name = "sub";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x2f) {
		name = "das";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(range(0x30, opcode, 6)) {
		name = "xor";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x37) {
		name = "aaa";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand(Register(0b100 /* AH */), 1);
		return;
	}
	if(range(0x38, opcode, 6)) {
		name = "cmp";
		handleArithmatic(ip);
		return;
	}
	if(opcode == 0x3f) {
		name = "aas";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand(Register(0b100 /* AH */), 1);
		return;
	}
	if(range(0x40, opcode, 8)) {
		name = "inc";
		operands[0] = Operand(Register(opcode & 0b111), 4);
		return;
	}
	if(range(0x48, opcode, 8)) {
		name = "dec";
		operands[0] = Operand(Register(opcode & 0b111), 4);
		return;
	}
	if(range(0x50, opcode, 8)) {
		name = "push";
		operands[0] = Operand(Register(opcode & 0b111), 4);
		return;
	}
	if(range(0x58, opcode, 8)) {
		name = "pop";
		operands[0] = Operand(Register(opcode & 0b111), 4);
		return;
	}
	if(opcode == 0x60) {
		name = "pusha";
		return;
	}
	if(opcode == 0x61) {
		name = "popa";
		return;
	}
	if(opcode == 0x62) {
		name = "bound";
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		return;
	}
	if(opcode == 0x63) {
		if(parser64BitMode) {
			name = "movsxd";
			Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		} else {
			name = "arpl";
			Operand::Builder::doubleReg(ip, 2, 2, operands + 1, operands);
		}
		return;
	}
	if(opcode == 0x68) {
		name = "push";
		operands[0] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0x69) {
		name = "imul";
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		operands[2] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0x6a) {
		name = "push";
		operands[0] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0x6b) {
		name = "imul";
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		operands[2] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0x6c) {
		name = "ins";
		operands[1] = Operand(Register(0b010 /* DX */), 1);
		return;
	}
	if(opcode == 0x6d) {
		name = "ins";
		operands[1] = Operand(Register(0b010 /* EDX */), 4);
		return;
	}
	if(opcode == 0x6e) {
		name = "out";
		operands[0] = Operand(Register(0b010 /* DX */), 1);
		return;
	}
	if(opcode == 0x6f) {
		name = "out";
		operands[0] = Operand(Register(0b010 /* EDX */), 4);
		return;
	}
	if(range(0x70, opcode, 0x10)) {
		name = conditionalNames[opcode & 0x0f];
		operands[0] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0x80 || opcode == 0x82) {
		extOpcode = (*ip >> 3) & 0b111;
		name = arithmaticNames[extOpcode];
		operands[0] = Operand::Builder(ip, 1).readModRM(true).build();
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0x81) {
		extOpcode = (*ip >> 3) & 0b111;
		name = arithmaticNames[extOpcode];
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0x83) {
		extOpcode = (*ip >> 3) & 0b111;
		name = arithmaticNames[extOpcode];
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0x84) {
		name = "test";
		Operand::Builder::doubleReg(ip, 1, 1, operands + 1, operands);
		return;
	}
	if(opcode == 0x85) {
		name = "test";
		Operand::Builder::doubleReg(ip, 4, 4, operands + 1, operands);
		return;
	}
	if(opcode == 0x86) {
		name = "xchg";
		Operand::Builder::doubleReg(ip, 1, 1, operands, operands + 1);
		return;
	}
	if(opcode == 0x87) {
		name = "xchg";
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		return;
	}
	if(opcode == 0x88) {
		name = "mov";
		Operand::Builder::doubleReg(ip, 1, 1, operands + 1, operands);
		return;
	}
	if(opcode == 0x89) {
		name = "mov";
		Operand::Builder::doubleReg(ip, 4, 4, operands + 1, operands);
		return;
	}
	if(opcode == 0x8a) {
		name = "mov";
		Operand::Builder::doubleReg(ip, 1, 1, operands, operands + 1);
		return;
	}
	if(opcode == 0x8b) {
		name = "mov";
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		return;
	}
	if(opcode == 0x8c) {
		name = "mov";
		operands[1] = Operand::Builder(ip, 2).readModRM(false).baseUseSegmentRegisters().build();
		ip--;
		operands[0] = Operand::Builder(ip, 2).readModRM(true).build();
		return;
	}
	if(opcode == 0x8d) {
		name = "lea";
		Operand::Builder::doubleReg(ip, 4, 4, operands, operands + 1);
		return;
	}
	if(opcode == 0x8e) {
		name = "mov";
		operands[0] = Operand::Builder(ip, 2).readModRM(false).baseUseSegmentRegisters().build();
		ip--;
		operands[1] = Operand::Builder(ip, 2).readModRM(true).build();
		return;
	}
	if(opcode == 0x8f) {
		name = "pop";
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		return;
	}
	if(opcode == 0x90) {
		if(hasPrefix(0xf3)) {
			name = "pause";
		} else {
			name = "nop";
		}
		return;
	}
	if(range(0x90, opcode, 8)) {
		name = "xchg";
		operands[0] = Operand(Register(opcode & 0b111), 4);
		operands[1] = Operand(Register(0b000 /* RAX */), 4);
		return;
	}
	if(opcode == 0x98) {
		name = "cvt";
		operands[0] = Operand(Register(0b000 /* EAX */), 4);
		operands[1] = Operand(Register(0b000 /* AX */), 2);
		return;
	}
	if(opcode == 0x99) {
		name = "cvt";
		operands[0] = Operand(Register(0b010 /* EDX */), 4);
		operands[1] = Operand(Register(0b000 /* AX */), 2);
		return;
	}
	if(opcode == 0x9a) {
		name = "callf";
		operands[0] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0x9b) {
		name = "wait";
		return;
	}
	if(opcode == 0x9c) {
		name = "pushf";
		return;
	}
	if(opcode == 0x9d) {
		name = "popf";
		return;
	}
	if(opcode == 0x9e) {
		name = "sahf";
		operands[0] = Operand(Register(0b100 /* AH */), 1);
		return;
	}
	if(opcode == 0x9f) {
		name = "lahf";
		operands[0] = Operand(Register(0b100 /* AH */), 1);
		return;
	}
	if(opcode == 0xa0) {
		name = "mov";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand::Builder(ip, 1).readImmediate(4).build();
		return;
	}
	if(opcode == 0xa1) {
		name = "mov";
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand::Builder(ip, 4).readImmediate(4).build();
		return;
	}
	if(opcode == 0xa2) {
		name = "mov";
		operands[0] = Operand::Builder(ip, 1).readImmediate(4).build();
		operands[1] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(opcode == 0xa3) {
		name = "mov";
		operands[0] = Operand::Builder(ip, 4).readImmediate(4).build();
		operands[1] = Operand(Register(0b000 /* RAX */), 4);
		return;
	}
	if(opcode == 0xa4) {
		name = "movs";
		operands[0] = Operand(Register(0b111 /* RDI */), 1);
		operands[1] = Operand(Register(0b110 /* RSI */), 1);
		return;
	}
	if(opcode == 0xa5) {
		name = "movs";
		operands[0] = Operand(Register(0b111 /* RDI */), 4);
		operands[1] = Operand(Register(0b110 /* RSI */), 4);
		return;
	}
	if(opcode == 0xa6) {
		name = "cmps";
		operands[0] = Operand(Register(0b111 /* RDI */), 1);
		operands[1] = Operand(Register(0b110 /* RSI */), 1);
		return;
	}
	if(opcode == 0xa6) {
		name = "cmps";
		operands[0] = Operand(Register(0b111 /* RDI */), 4);
		operands[1] = Operand(Register(0b110 /* RSI */), 4);
		return;
	}
	if(opcode == 0xa8) {
		name = "test";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0xa9) {
		name = "test";
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xaa) {
		name = "stos";
		operands[0] = Operand(Register(0b111 /* RDI */), 1);
		operands[1] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(opcode == 0xab) {
		name = "stos";
		operands[0] = Operand(Register(0b111 /* RDI */), 4);
		operands[1] = Operand(Register(0b000 /* RAX */), 4);
		return;
	}
	if(opcode == 0xac) {
		name = "lods";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand(Register(0b111 /* RDI */), 1);
		return;
	}
	if(opcode == 0xad) {
		name = "lods";
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand(Register(0b111 /* RDI */), 4);
		return;
	}
	if(opcode == 0xae) {
		name = "scas";
		operands[0] = Operand(Register(0b111 /* RDI */), 1);
		operands[1] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(opcode == 0xaf) {
		name = "scas";
		operands[0] = Operand(Register(0b111 /* RDI */), 4);
		operands[1] = Operand(Register(0b000 /* RAX */), 4);
		return;
	}
	if(range(0xb0, opcode, 8)) {
		name = "mov";
		operands[0] = Operand(Register(opcode & 0b111), 1);
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(range(0xb8, opcode, 8)) {
		name = "mov";
		operands[0] = Operand(Register(opcode & 0b111), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xc0) {
		extOpcode = (*ip >> 3) & 0b111;
		name = shiftNames[extOpcode];
		operands[0] = Operand::Builder(ip, 1).readModRM(true).build();
		return;
	}
	if(opcode == 0xc1) {
		extOpcode = (*ip >> 3) & 0b111;
		name = shiftNames[extOpcode];
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		return;
	}
	if(opcode == 0xc2) {
		name = "retn";
		operands[0] = Operand::Builder(ip).readImmediate(2).build();
		return;
	}
	if(opcode == 0xc3) {
		name = "retn";
		return;
	}
	if(opcode == 0xc4) {
		name = "les";
		operands[0] = Operand(Register(Register::ES), 4);
		Operand::Builder::doubleReg(ip, 4, 4, operands + 1, operands + 2);
		return;
	}
	if(opcode == 0xc5) {
		name = "lds";
		operands[0] = Operand(Register(Register::DS), 4);
		Operand::Builder::doubleReg(ip, 4, 4, operands + 1, operands + 2);
		return;
	}
	if(opcode == 0xc6) {
		name = "mov";
		operands[0] = Operand::Builder(ip, 1).readModRM(true).build();
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0xc7) {
		name = "mov";
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xc8) {
		name = "enter";
		operands[0] = Operand(Register(0b101 /* RBP */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(2).build();
		operands[2] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0xc9) {
		name = "leave";
		operands[0] = Operand(Register(0b101 /* RBP */), 4);
		return;
	}
	if(opcode == 0xca) {
		name = "retf";
		operands[0] = Operand::Builder(ip).readImmediate(2).build();
		return;
	}
	if(opcode == 0xcb) {
		name = "retf";
		return;
	}
	if(opcode == 0xcc) {
		name = "int";
		operands[0] = Operand(0x03, 4);
		return;
	}
	if(opcode == 0xcd) {
		name = "int";
		operands[0] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xce) {
		name = "into";
		return;
	}
	if(opcode == 0xcf) {
		name = "iret";
		return;
	}
	if(opcode == 0xd0) {
		name = shiftNames[(*ip >> 3) & 0b111];
		operands[0] = Operand::Builder(ip, 1).readModRM(true).build();
		operands[1] = Operand(0x01, 1);
		return;
	}
	if(opcode == 0xd1) {
		name = shiftNames[(*ip >> 3) & 0b111];
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		operands[1] = Operand(0x01, 4);
		return;
	}
	if(opcode == 0xd2) {
		name = shiftNames[(*ip >> 3) & 0b111];
		operands[0] = Operand::Builder(ip, 1).readModRM(true).build();
		operands[1] = Operand(Register(0b001 /* CL */), 1);
		return;
	}
	if(opcode == 0xd3) {
		name = shiftNames[(*ip >> 3) & 0b111];
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		operands[1] = Operand(Register(0b001 /* CL */), 1);
		return;
	}
	if(opcode == 0xd4) {
		if(*ip == 0x0a) {
			ip++;
			name = "aam";
			operands[0] = Operand(Register(0b000 /* AL */), 1);
			operands[1] = Operand(Register(0b100 /* AH */), 1);
		} else {
			name = "amx";
			operands[0] = Operand(Register(0b000 /* AL */), 1);
			operands[1] = Operand(Register(0b100 /* AH */), 1);
			operands[2] = Operand::Builder(ip).readImmediate(1).build();
		}
		return;
	}
	if(opcode == 0xd5) {
		if(*ip == 0x0a) {
			ip++;
			name = "aad";
			operands[0] = Operand(Register(0b000 /* AL */), 1);
			operands[1] = Operand(Register(0b100 /* AH */), 1);
		} else {
			name = "adx";
			operands[0] = Operand(Register(0b000 /* AL */), 1);
			operands[1] = Operand(Register(0b100 /* AH */), 1);
			operands[2] = Operand::Builder(ip).readImmediate(1).build();
		}
		return;
	}
	if(opcode == 0xd6) {
		name = "salc";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(opcode == 0xd7) {
		name = "xlat";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	//////////////////////////////   TODO   /////////////////////////////////
	if(opcode == 0xe0) {
		name = "loopne";
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xe1) {
		name = "loope";
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xe2) {
		name = "loop";
		operands[0] = Operand(Register(0b000 /* RAX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xe3) {
		name = "jcxz";
		operands[0] = Operand(Register(0b001 /* RCX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xe4) {
		name = "in";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0xe5) {
		name = "in";
		operands[0] = Operand(Register(0b000 /* EAX */), 4);
		operands[1] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0xe6) {
		name = "out";
		operands[0] = Operand::Builder(ip).readImmediate(1).build();
		operands[1] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(opcode == 0xe7) {
		name = "out";
		operands[0] = Operand::Builder(ip).readImmediate(1).build();
		operands[1] = Operand(Register(0b000 /* EAX */), 4);
		return;
	}
	if(opcode == 0xe8) {
		name = "call";
		operands[0] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xe9) {
		name = "jmp";
		operands[0] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xea) {
		name = "jmpf";
		operands[0] = Operand::Builder(ip).readImmediate(4).build();
		return;
	}
	if(opcode == 0xeb) {
		name = "jmp";
		operands[0] = Operand::Builder(ip).readImmediate(1).build();
		return;
	}
	if(opcode == 0xec) {
		name = "in";
		operands[0] = Operand(Register(0b000 /* AL */), 1);
		operands[1] = Operand(Register(0b010 /* DX */), 2);
		return;
	}
	if(opcode == 0xed) {
		name = "in";
		operands[0] = Operand(Register(0b000 /* EAX */), 4);
		operands[1] = Operand(Register(0b010 /* DX */), 2);
		return;
	}
	if(opcode == 0xee) {
		name = "out";
		operands[0] = Operand(Register(0b010 /* DX */), 2);
		operands[1] = Operand(Register(0b000 /* AL */), 1);
		return;
	}
	if(opcode == 0xef) {
		name = "out";
		operands[0] = Operand(Register(0b010 /* DX */), 2);
		operands[1] = Operand(Register(0b000 /* EAX */), 4);
		return;
	}
	if(opcode == 0xf1) {
		name = "int1";
		return;
	}
	if(opcode == 0xf4) {
		name = "halt";
		return;
	}
	if(opcode == 0xf5) {
		name = "cmc";
		return;
	}
	if(opcode == 0xf6) {
		extOpcode = (*ip >> 3) & 0b111;
		name = extMathNames[extOpcode];
		Operand expr = Operand::Builder(ip, 1).readModRM(true).build();
		switch(extOpcode) {
		case 0:
		case 1:
			operands[0] = expr;
			operands[1] = Operand::Builder(ip).readImmediate(1).build();
			break;
		case 2:
		case 3:
			operands[0] = expr;
			break;
		case 4:
		case 5:
			operands[0] = Operand(Register(0b000 /* AX */), 2);
			operands[1] = Operand(Register(0b000 /* AL */), 1);
			operands[2] = expr;
			break;
		case 6:
		case 7:
			operands[0] = Operand(Register(0b000 /* AL */), 1);
			operands[1] = Operand(Register(0b100 /* AH */), 1);
			operands[2] = Operand(Register(0b000 /* AX */), 2);
			operands[3] = expr;
			break;
		}
		return;
	}
	if(opcode == 0xf7) {
		extOpcode = (*ip >> 3) & 0b111;
		name = extMathNames[extOpcode];
		Operand expr = Operand::Builder(ip, 4).readModRM(true).build();
		switch(extOpcode) {
		case 0:
		case 1:
			operands[0] = expr;
			operands[1] = Operand::Builder(ip).readImmediate(4).build();
			break;
		case 2:
		case 3:
			operands[0] = expr;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			operands[0] = Operand(Register(0b010 /* RDX */), 4);
			operands[1] = Operand(Register(0b000 /* RAX */), 4);
			operands[2] = expr;
			break;
		}
		return;
	}
	if(opcode == 0xf8) {
		name = "clc";
		return;
	}
	if(opcode == 0xf9) {
		name = "stc";
		return;
	}
	if(opcode == 0xfa) {
		name = "cli";
		return;
	}
	if(opcode == 0xfb) {
		name = "sti";
		return;
	}
	if(opcode == 0xfc) {
		name = "cld";
		return;
	}
	if(opcode == 0xfd) {
		name = "std";
		return;
	}
	if(opcode == 0xfe) {
		extOpcode = (*ip >> 3) & 0b111;
		name = extOpcode ? "dec" : "inc";
		operands[0] = Operand::Builder(ip, 1).readModRM(true).build();
		return;
	}
	if(opcode == 0xff) {
		extOpcode = (*ip >> 3) & 0b111;
		name = utilNames[extOpcode];
		operands[0] = Operand::Builder(ip, 4).readModRM(true).build();
		return;
	}
	std::cerr << "Unimplemented opcode 0x" << std::hex << (int)opcode << " at 0x" << (int)(ip - 1);
	throw std::runtime_error("Unimplemented Opcode");
}

unsigned char Instruction::getOpcode() {
	return opcode;
}

Operand* Instruction::getOperands() {
	return operands;
}

bool Instruction::returns() {
	return opcode == 0xc2
		|| opcode == 0xc3
		|| opcode == 0xca
		|| opcode == 0xcb
		|| opcode == 0xcf;
}

Instruction::JumpType::JumpType()
	: jumps(false) {}

Instruction::JumpType::JumpType(bool relative, bool conditional, bool functional)
	: jumps(true), relative(relative), conditional(conditional), functional(functional) {}

bool Instruction::JumpType::operator==(const JumpType other) {
	if(!(jumps || other.jumps)) return true;
	return jumps == other.jumps
		&& relative == other.relative
		&& conditional == other.conditional;
}

bool Instruction::JumpType::operator!=(const JumpType other) {
	return !this->operator==(other);
}

const Instruction::JumpType Instruction::JumpType::RELATIVE = { true, true, false };
const Instruction::JumpType Instruction::JumpType::ABSOLUTE = { false, true, false };
const Instruction::JumpType Instruction::JumpType::UNCONDITIONAL_RELATIVE = { true, false, false };
const Instruction::JumpType Instruction::JumpType::UNCONDITIONAL_ABSOLUTE = { false, false, false };
const Instruction::JumpType Instruction::JumpType::FUNCTIONAL_RELATIVE = { true, false, true };
const Instruction::JumpType Instruction::JumpType::FUNCTIONAL_ABSOLUTE = { false, false, true };
const Instruction::JumpType Instruction::JumpType::NONE = {};

Instruction::JumpType Instruction::jumps() {
	if(range(0x70, opcode, 0x10)
		|| (opcode == 0x0f && range(0x80, secondOpcode, 0x10))
		|| opcode == 0xe1
		|| opcode == 0xe3)
		return JumpType::RELATIVE;
	if(opcode == 0xe9
		|| opcode == 0xeb)
		//|| (opcode == 0xff && extOpcode == 4))
		return JumpType::UNCONDITIONAL_RELATIVE;
	if(opcode == 0xea)
		//|| (opcode == 0xff && extOpcode == 5))
		return JumpType::UNCONDITIONAL_ABSOLUTE;
	if(opcode == 0xe8)
		//|| (opcode == 0xff && extOpcode == 2))
		return JumpType::FUNCTIONAL_RELATIVE;
	if(opcode == 0x9a)
		//|| (opcode == 0xff && extOpcode == 3))
		return JumpType::FUNCTIONAL_ABSOLUTE;
	return JumpType::NONE;
}

Operand Instruction::getJumpSource() {
	if(jumps() == JumpType::NONE) throw std::runtime_error("Instruction does not jump");
	if(opcode == 0xe1) return operands[1];
	return operands[0];
}

int Instruction::jumpsTo() {
	if(jumps() == JumpType::NONE) throw std::runtime_error("Instruction does not jump");
	getJumpSource().getImmediate();
}

int Instruction::jumpOpSize() {
	if(jumps() == JumpType::NONE) throw std::runtime_error("Instruction does not jump");
	if(range(0x70, opcode, 0x10)
		|| opcode == 0xe1
		|| opcode == 0xe3
		|| opcode == 0xe9
		|| opcode == 0xeb)
		return 1;
	if((opcode == 0x0f && range(0x80, secondOpcode, 0x10))
		|| opcode == 0x9a
		|| opcode == 0xe8
		|| opcode == 0xea
		|| opcode == 0xff)
		return 4;
}

unsigned char* Instruction::getJumpOpLocation() {
	return getJumpSource().getSourceAddr();
}

std::string Instruction::getName() {
	return name;
}

unsigned char* Instruction::getSourceLocation() {
	return sourceAddr;
}

int Instruction::getSourceLength() {
	return byteLength;
}
