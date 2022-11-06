#pragma once

#include "Register.h"

class Operand {
private:
	size_t length = 0;
	int immediate = 0;
	Register base = {};
	Register index = {};
	unsigned char scale = 1;
	Operand(size_t length, int immediate, Register base, Register index, unsigned char scale);
public:
	Operand();
	Operand(int immediate, size_t length);
	Operand(Register reg, size_t length);
	bool isValid();
	int getImmediate();

	class Builder {
	private:
		static unsigned char* nullPtr;
		size_t length = 0;
		int immediate = 0;
		Register base;
		Register index;
		unsigned char scale = 1;
		unsigned char*& ip;
		void readSIB(int immediateBytes, unsigned char mod);
	public:
		Builder(unsigned char*& ip);
		Builder(unsigned char*& ip, size_t length);
		Builder& readImmediate(int bytes);
		Builder& readModRM(bool extended);
		Builder& readSIB();
		Builder& baseUseSegmentRegisters();
		Operand build();

		static void doubleReg(unsigned char*& ip, int sz1, int sz2, Operand* op1, Operand* op2);
	};
};
