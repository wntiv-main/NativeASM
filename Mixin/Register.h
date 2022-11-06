#pragma once

#include <string>

class Register {
private:
	unsigned char value = 0xff;
public:
	static const unsigned char ES = 0b1000;
	static const unsigned char CS = 0b1001;
	static const unsigned char SS = 0b1010;
	static const unsigned char DS = 0b1011;
	static const unsigned char FS = 0b1100;
	static const unsigned char GS = 0b1101;
	static const std::string registerNames[8][5];

	Register() = default;
	Register(unsigned char reg);

	bool isValid();
	std::string toString(int type = 2);
	unsigned char getByte();
};

