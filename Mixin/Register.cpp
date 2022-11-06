#include "Register.h"

const std::string Register::registerNames[8][5] = {
	{"al", "ax", "eax", "mm0", "xmm0"},
	{"cl", "cx", "ecx", "mm1", "xmm1"},
	{"dl", "dx", "edx", "mm2", "xmm2"},
	{"bl", "bx", "ebx", "mm3", "xmm3"},
	{"ah", "sp", "esp", "mm4", "xmm4"},
	{"ch", "bp", "ebp", "mm5", "xmm5"},
	{"dh", "si", "esi", "mm6", "xmm6"},
	{"bh", "di", "edi", "mm7", "xmm7"}
};

Register::Register(unsigned char reg) : value(reg) {}

bool Register::isValid() {
	//   Standard Registers |  DS, ES, FS, GS, SS
	return value <= 0b111 || ((value & 0b1000) && ((value ^ 0b1000) <= 0b101));
}

std::string Register::toString(int type) {
	if (!isValid()) return "INVALID";
	return registerNames[value][type];
}

unsigned char Register::getByte() {
	return value;
}
