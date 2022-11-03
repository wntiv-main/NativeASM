#include "ScaledIndexByte.h"

#include "../Instruction.h"
#include <stdexcept>
#include <sstream>

ScaledIndexByte::ScaledIndexByte()
	: valid(false) {}

ScaledIndexByte::ScaledIndexByte(unsigned char*& ip)
	: valid(true),
	base(*ip & 0b111),
	index((*ip >> 3) & 0b111),
	scale(1 << ((*ip >> 6) & 0b11)) {
	ip++;
	if (index.getByte() == 0b100) throw std::runtime_error("Invalid SIB byte");
	if (base.getByte() == 0b101) {
		if (scale == 1) {
			base = Register();
		}
		if (scale == 1 || scale == 4) {
			addrOffset = *(int*)ip;
			ip += 4;
		} else if (scale == 2) {
			addrOffset = (int)*(char*)ip;
			ip++;
		} else throw std::runtime_error("Invalid SIB byte");
	}
}

std::string ScaledIndexByte::toString() {
	std::ostringstream str;
	bool first = true;
	if (index.isValid()) {
		if (!first) str << " + ";
		first = false;
		str << index.toString();
		if (scale > 1) {
			str << "*" << (int)scale;
		}
	}
	if (base.isValid()) {
		if (!first) str << " + ";
		first = false;
		str << base.toString();
	}
	if (addrOffset != 0) {
		if (!first) str << " + ";
		first = false;
		str << "0x" << hexString(addrOffset);
	}
	return str.str();
}
