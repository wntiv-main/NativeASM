#pragma once

#include "Operand.h"

#include <vector>

#include "InstructionPrefix.h"

class Instruction {
private:
	static const std::string arithmaticNames[8];
	static const std::string extMathNames[8];
	static const std::string shiftNames[8];
	static const std::string conditionalNames[16];
	static const std::string utilNames[8];
	Operand operands[4] = { {}, {}, {}, {} };
	unsigned char* sourceAddr = nullptr;
	int byteLength = 0;
	std::string name = "";
	unsigned char opcode = 0;
	unsigned char secondOpcode = 0;
	unsigned char extOpcode = 0;
	std::vector<InstructionPrefix> prefixes;
	bool hasPrefix(unsigned char prefix);
	void parseInstruction(unsigned char*& ip);
	Operand getJumpSource();
public:
	class JumpType {
	public:
		const bool jumps = false;
		const bool relative = true;
		const bool conditional = false;
		const bool functional = false;
		JumpType();
		JumpType(bool relative, bool conditional, bool functional);
		bool operator==(const JumpType other);
		bool operator!=(const JumpType other);
		static const JumpType RELATIVE;
		static const JumpType ABSOLUTE;
		static const JumpType UNCONDITIONAL_RELATIVE;
		static const JumpType UNCONDITIONAL_ABSOLUTE;
		static const JumpType FUNCTIONAL_RELATIVE;
		static const JumpType FUNCTIONAL_ABSOLUTE;
		static const JumpType NONE;
	};

	static bool parser64BitMode;
	Instruction();
	Instruction(unsigned char*& ip);

	void handleArithmatic(unsigned char*& ip);
	unsigned char getOpcode();
	Operand* getOperands();
	bool returns();

	JumpType jumps();
	int jumpsTo();
	int jumpOpSize();
	unsigned char* getJumpOpLocation();
	
	std::string getName();
	unsigned char* getSourceLocation();
	int getSourceLength();
};
