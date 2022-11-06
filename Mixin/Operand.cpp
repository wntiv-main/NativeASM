#include "Operand.h"

#include <stdexcept>


Operand::Operand(size_t length, int immediate, Register base, Register index, unsigned char scale)
	: length(length), immediate(immediate), base(base), index(index), scale(scale) {}

Operand::Operand()
	: length(0) {}

Operand::Operand(int immediate, size_t length)
	: length(length), immediate(immediate) {}

Operand::Operand(Register reg, size_t length)
	: length(length), base(reg) {}

bool Operand::isValid() {
	return length != 0;
}

int Operand::getImmediate() {
	return immediate;
}

unsigned char* Operand::Builder::nullPtr = nullptr;

Operand::Builder::Builder(unsigned char*& ip)
	: ip(ip), length(0) {}

Operand::Builder::Builder(unsigned char*& ip, size_t length)
	: ip(ip), length(length) {}

Operand::Builder& Operand::Builder::readImmediate(int bytes) {
	if(bytes == 0) return *this;
	if(length == 0) length = bytes;
	if(bytes == 1) {
		immediate = (int)*(char*)ip;
		ip += 1;
	} else if(bytes == 2) {
		immediate = (int)*(short*)ip;
		ip += 2;
	} else if(bytes == 4) {
		immediate = (int)*(int*)ip;
		ip += 4;
	} else throw std::runtime_error("Unexpected number of immediate bytes to read");
	return *this;
}

Operand::Builder& Operand::Builder::readModRM(bool extended) {
	int immediateBytes = 0;
	unsigned char byte = *ip;
	ip++;
	if(!extended) {
		base = (byte >> 3) & 0b111;
		return *this;
	}
	base = byte & 0b111;
	unsigned char mod = (byte >> 6) & 0b11;
	if(mod == 0b00 && base.getByte() == 0b101) {
		immediateBytes = 4;
		base = Register();
	}
	if(mod == 0b01) immediateBytes = 1;
	if(mod == 0b10) immediateBytes = 4;
	if(mod != 0b11 && base.getByte() == 0b100) {
		readSIB();
	} else readImmediate(immediateBytes);
	return *this;
}

Operand::Builder& Operand::Builder::readSIB() {
	readSIB(0, 0xff);
	return *this;
}

Operand::Builder& Operand::Builder::baseUseSegmentRegisters() {
	base = base.getByte() | 0b1000;
	return *this;
}

void Operand::Builder::readSIB(int bytes, unsigned char mod) {
	unsigned char byte = *ip;
	ip++;
	base = byte & 0b111;
	index = (byte >> 3) & 0b111;
	scale = (byte >> 6) & 0b11;
	if(index.getByte() == 0b100) throw std::runtime_error("Invalid SIB byte");
	if(base.getByte() == 0b101) {
		int immediateBytes = bytes;
		unsigned char m = mod;
		if(m >> 2) m = scale;
		if(immediateBytes != 0) {
			if(m == 0b01) {
				immediateBytes = 1;
			} else {
				immediateBytes = 4;
			}
		}
		if(m != 0b01 && m != 0b10) {
			base = Register();
		}
		readImmediate(immediateBytes);
	}
}

Operand Operand::Builder::build() {
	return Operand(length, immediate, base, index, scale);
}

void Operand::Builder::doubleReg(unsigned char*& ip, int sz1, int sz2, Operand* op1, Operand* op2) {
	*op1 = Builder(ip, sz1).readModRM(false).build();
	ip--;
	*op2 = Builder(ip, sz2).readModRM(true).build();
}
