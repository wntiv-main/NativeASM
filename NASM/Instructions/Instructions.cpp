#include "Instructions.h"

#include <iostream>
#include "JumpInstruction.h"
#include "PushRegisterInstruction.h"
#include "MoveInstruction.h"
#include "InverseMoveInstruction.h"
#include "PushByteInstruction.h"
#include "PushIntInstruction.h"
#include "MoveEAXInstruction.h"
#include "InverseMoveEAXInstruction.h"
#include "ByteOperatorInstruction.h"
#include "IntOperatorInstruction.h"
#include "LoadEffectiveAddressInstruction.h"
#include "MoveConstantIntInstruction.h"
#include "StoreStringInstruction.h"
#include "XorInstruction.h"
#include "MoveConstantIntExpInstruction.h"
#include "CallInstruction.h"
#include "MixOperatorInstruction.h"
#include "TwoByteInstructions.h"
#include "TestInstruction.h"
#include "JumpIfEqualInstruction.h"
#include "FFInstruction.h"
#include "F7Instruction.h"
#include "CompareInstruction.h"
#include "JumpIfNotEqualInstruction.h"
#include "NoOpInstruction.h"
#include "PopRegisterInstruction.h"
#include "ReturnInstruction.h"
#include "InverseCompareInstruction.h"
#include "JumpIfBelowInstruction.h"
#include "JumpByteInstruction.h"

Instruction* Instructions::readInstruction(unsigned char*& ip) {
	if(*ip == JumpInstruction::opcode) return JumpInstruction::create(++ip);
	if(*ip >= PushRegisterInstruction::opcode
		&& *ip < PushRegisterInstruction::opcode + 8) return PushRegisterInstruction::create(++ip);
	if(*ip == MoveInstruction::opcode) return MoveInstruction::create(++ip);
	if(*ip == InverseMoveInstruction::opcode) return InverseMoveInstruction::create(++ip);
	if(*ip == PushByteInstruction::opcode) return PushByteInstruction::create(++ip);
	if(*ip == PushIntInstruction::opcode) return PushIntInstruction::create(++ip);
	if(*ip == 0x64 || *ip == 0xf2 || *ip == 0xf3) return PrefixOpcode::create(++ip);
	if(*ip == MoveEAXInstruction::opcode) return MoveEAXInstruction::create(++ip);
	if(*ip == InverseMoveEAXInstruction::opcode) return InverseMoveEAXInstruction::create(++ip);
	if(*ip == ByteOperatorInstruction::opcode
		|| *ip == ByteOperatorInstruction::aliasOpcode) return ByteOperatorInstruction::create(++ip);
	if(*ip == IntOperatorInstruction::opcode) return IntOperatorInstruction::create(++ip);
	if(*ip == LoadEffectiveAddressInstruction::opcode) return LoadEffectiveAddressInstruction::create(++ip);
	if(*ip >= MoveConstantIntInstruction::opcode
		&& *ip < MoveConstantIntInstruction::opcode + 8) return MoveConstantIntInstruction::create(++ip);
	if(*ip == StoreStringInstruction::opcode) return StoreStringInstruction::create(++ip);
	if(*ip == XorInstruction::opcode) return XorInstruction::create(++ip);
	if(*ip == MoveConstantIntExpInstruction::opcode) return MoveConstantIntExpInstruction::create(++ip);
	if(*ip == CallInstruction::opcode) return CallInstruction::create(++ip);
	if(*ip == MixOperatorInstruction::opcode) return MixOperatorInstruction::create(++ip);
	if(*ip == TwoByteInstructions::opcode) return TwoByteInstructions::readInstruction(++ip);
	if(*ip == TestInstruction::opcode) return TestInstruction::create(++ip);
	if(*ip == JumpIfEqualInstruction::opcode) return JumpIfEqualInstruction::create(++ip);
	if(*ip == FFInstruction::opcode) return FFInstruction::create(++ip);
	if(*ip == F7Instruction::opcode) return F7Instruction::create(++ip);
	if(*ip == CompareInstruction::opcode) return CompareInstruction::create(++ip);
	if(*ip == JumpIfNotEqualInstruction::opcode) return JumpIfNotEqualInstruction::create(++ip);
	if(*ip == NoOpInstruction::opcode) return NoOpInstruction::create(++ip);
	if(*ip >= PopRegisterInstruction::opcode
		&& *ip < PopRegisterInstruction::opcode + 8) return PopRegisterInstruction::create(++ip);
	if(*ip == ReturnInstruction::opcode) return ReturnInstruction::create(++ip);
	if(*ip == InverseCompareInstruction::opcode) return InverseCompareInstruction::create(++ip);
	if(*ip == JumpIfBelowInstruction::opcode) return JumpIfBelowInstruction::create(++ip);
	if(*ip == JumpByteInstruction::opcode) return JumpByteInstruction::create(++ip);


	std::cerr << std::hex << "Opcode " << (int)*ip << " not implemented at 0x" << (size_t)ip;
	throw std::runtime_error("Not implemented");
}