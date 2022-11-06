#include <iostream>

#include "Target.h"
#include "FunctionReader.h"

void test() {
	std::cout << std::hex << 0x4e110 << std::endl;
}

int main() {
	FunctionReader::read2(test, "test");
	//test();
	//Target atTest = Target::Builder().at((void*)test).build();
	//atTest.jump();
	//atTest.seekTo(0x4e110);
	//atTest.write(2, 0x0800);
	//atTest.write(2, 0xdb7e);
	//atTest.write(1, 0x98);
	//atTest.write(4, 0x0800db7e);
	//atTest.write(1, 0xc3);
	//atTest.write(4, 0xcccccccc);
	//test();
	//test(Target::Builder().at(Target::Type::HEAD).ordinal(0).build());
}