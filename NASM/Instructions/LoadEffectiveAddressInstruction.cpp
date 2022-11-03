#include "LoadEffectiveAddressInstruction.h"

LoadEffectiveAddressInstruction::LoadEffectiveAddressInstruction(unsigned char*& ip)
	: RegRegInstruction(opcode, "lea", ip) {}

std::string LoadEffectiveAddressInstruction::toString() {
	return RegRegInstruction::toString(" ", " <- ");
}

Instruction* LoadEffectiveAddressInstruction::create(unsigned char*& ip) {
	return new LoadEffectiveAddressInstruction(ip);
}

const unsigned char LoadEffectiveAddressInstruction::opcode = 0x8d;