#pragma once

#include <unordered_map>

#include "Instruction.h"

class Instructions {
private:
	Instructions() = delete;
public:
	static Instruction* readInstruction(unsigned char*& ip);
};
