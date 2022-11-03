#include <iostream>
#include "FunctionReader.h"

class Example {
public:
	static float function1(std::string input, int random) {
		std::cout << input << random << std::endl;
		return (float)random / 2;
	}

	static void function2() {
		std::cout << "Hello, World!" << std::endl;
	}

	static void function3() {
		std::cout << 0x4e110 << std::endl;
		//{ volatile unsigned int _ = 0xBAD0BEEF; }
	}

	virtual void func4() {
		std::cout << 0 << std::endl;
	}
};

void assert(bool condition, std::string message) {
	if (!condition) throw new std::runtime_error(message);
}

void mixin(char* target, void(&mixin)()) {
	char* mixinFunc = (char*)mixin;
	while(*(short*)target != 0xc3cc) {
		target++;
	}
	while(*(short*)target != 0xc3cc) {
		*target = *mixinFunc;
		mixinFunc++;
		target++;
	}
	*target = 0xc3;
}

void other() {
	std::cout << "Hello, World!" << std::endl;
}

void print4e110() {
	std::cout << std::hex << 0x4e110 << std::endl;
}

int main() {
	FunctionReader reader;

	// Call initial time
	print4e110();
	// Find and modify constant to 0x800db7e
	char* addr = (char*)print4e110;
	addr++;
	char* actualPtr = addr + 4 + *(int*)addr;
	actualPtr += (0x0029E421 - 0x0029E3F0) + 1;
	*(int*)actualPtr = 0x800db7e;
	// Call second time
	print4e110();
	// mixin
	mixin((char*)&print4e110, other);
	reader.read(print4e110);
	// Call final time
	print4e110();
}


	//auto p = getCodeSectionStart(Example::function1);

	//std::cout << std::hex << "\n -------- CODE START --------- at " << (unsigned long)p << std::endl;

	//Example example;
	////reader.read(Example::function3, "Example::function3");


	//reader.read3(Example::function1, "Example::function1");
	//reader.read(Example::function2, "Example::function2");
	//reader.read(Example::function3, "Example::function3");
	//reader.read(atest2, "atest2");
	////reader.read<int, int>(std::abs, "std::abs");
	//reader.read<float, float>(std::abs, "std::abs");
	//reader.read<double, double>(std::abs, "std::abs");
	//reader.read<void, void(&)(), std::string>(FunctionReader::read);
	////reader.read<void, void(&)(), std::string, std::ostream, std::string>(FunctionReader::read3, "read3");
	//reader.read2(main, "main");
//	std::cout << "--------------" << std::endl;
//	//offset(function4);
//}
