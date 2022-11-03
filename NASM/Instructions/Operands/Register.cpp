#include "Register.h"

const std::string Register::registerNames[8][5] = {
	{"al", "ax", "eax", "mm0", "xmm0"},
	{"cl", "cx", "ecx", "mm1", "xmm1"},
	{"dl", "dx", "edx", "mm2", "xmm2"},
	{"bl", "bx", "ebx", "mm3", "xmm3"},
	{"al", "sp", "esp", "mm4", "xmm4"},
	{"cl", "bp", "ebp", "mm5", "xmm5"},
	{"dl", "si", "esi", "mm6", "xmm6"},
	{"bl", "di", "edi", "mm7", "xmm7"}
};

Register::Register() : value(0xff) {}

Register::Register(unsigned char reg) : value(reg & 0b111) {}

bool Register::isValid() {
	return value <= 0b111;
}

std::string Register::toString() {
	if (!isValid()) return "INVALID";
	return registerNames[value][2];
}

unsigned char Register::getByte() {
	return value;
}
