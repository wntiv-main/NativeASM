#include "Expression.h"

#include "../Instruction.h"

Expression::Expression() : valid(false) {}

Expression::Expression(unsigned char*& ip) : valid(true) {
	unsigned char modRM = *ip++;
	base = modRM & 0b111;
	unsigned char mod = (modRM >> 6) & 0b11;
	if (mod == 0b11) {
		ext = true;
		return;
	}
	if (base.getByte() == 0b101 && mod == 0b00) {
		base = Register();
	}
	if (base.getByte() == 0b100) {
		unsigned char sib = *ip++;
		base = sib & 0b111;
		index = (sib >> 3) & 0b111;
		if (index.getByte() == 0b100) throw "Unexpected SIB byte";
		unsigned char sibMod = ((sib >> 6) & 0b11);
		scale = 1 << sibMod;
		if (base.getByte() == 0b101) {
			if (sibMod == 0b00) {
				base = Register();
			}
			if (sibMod == 0b11) {
				throw "Unexpected SIB byte";
			} else if (sibMod == 0b01) {
				addrOffset = (int)*(char*)(ip++);
			} else {
				addrOffset = *(int*)ip;
				ip += 4;
			}
		}
	}
	if (mod == 0b01) {
		valueOffset = (int)*(char*)(ip++);
	} else if (mod == 0b10 || (mod == 0b00 && !base.isValid())) {
		valueOffset = *(int*)ip;
		ip += 4;
	}
}

std::string Expression::toString() {
	if (!isValid()) return "INVALID";
	if (ext) {
		return "*" + base.toString();
	}
	bool first = true;
	std::string result = "[";
	if (base.isValid()) {
		result += first ? "" : " + ";
		result += base.toString();
		first = false;
	}
	if (index.isValid()) {
		result += first ? "" : " + ";
		result += index.toString();
		first = false;
		if (scale != 1) {
			result += "*" + (int)scale;
		}
	}
	if (addrOffset != 0) {
		result += first ? "" : " + ";
		result += "0x" + hexString(addrOffset);
		first = false;
	}
	result += "]";
	if (valueOffset != 0) {
		if (first) {
			result = "";
		} else {
			result += " + ";
		}
		result += "0x" + hexString(valueOffset);
	}
	return result;
}

bool Expression::isValid() {
	return valid;
}

bool Expression::isMemory() {
	return !ext;
}

bool Expression::isConstant() {
	return !ext && !base.isValid() && !index.isValid() && scale == 1 && addrOffset == 0;
}

int Expression::getValueOffset() {
	return valueOffset;
}
