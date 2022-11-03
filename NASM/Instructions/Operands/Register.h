#pragma once

#include <string>

class Register {
private:
	unsigned char value;
public:
	static const std::string registerNames[8][5];

	Register();
	Register(unsigned char reg);

	bool isValid();
	std::string toString();
	unsigned char getByte();
};

