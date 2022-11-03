#pragma once

#include "Register.h"

class ScaledIndexByte {
private:
	unsigned char scale = 1;
	Register base;
	Register index;
	int addrOffset = 0;
	bool valid;
public:
	ScaledIndexByte();
	ScaledIndexByte(unsigned char*& ip);
	//unsigned char getScale;
	//Register getBase;
	//Register getIndex;
	//int getAddrOffset;
	std::string toString();
};
