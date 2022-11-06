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

void mixin(unsigned char* target, void(&mixin)()) {
	unsigned char* mixinFunc = (unsigned char*)mixin;
	//unsigned char* actualMixin = mixinFunc + 1 + 4 + *(int*)(mixinFunc + 1);
	unsigned char* actualTarget = target + 1 + 4 + *(int*)(target + 1);
	while(*actualTarget != 0xc3 || *(unsigned int*)(actualTarget + 1) != 0xcccccccc) {
		//std::cout << (size_t)actualTarget << " " << (int)*(unsigned char*)actualTarget << std::endl;
		actualTarget++;
	}
	*actualTarget = 0xe8;
	actualTarget++;
	int offset = mixinFunc - actualTarget - 4;
	*(int*)actualTarget = offset;
	actualTarget += 4;
	*actualTarget = 0xc3;
}

void other() {
	std::cout << "Hello, World!" << std::endl;
}

void print4e110() {
	std::cout << std::dec << "Your final grade is: " << 0 << "%" << std::endl;
}

int main() {
	// Call initial time
	print4e110();
	// Find and modify constant to 0x800db7e
	char* addr = (char*)print4e110;
	addr++;
	char* actualPtr = addr + 4 + *(int*)addr;
	while(*(unsigned short*)actualPtr != 0x006a){ actualPtr++; }
	actualPtr++;
	*actualPtr = 100;
	// Call second time
	print4e110();
	//mixin()
	FunctionReader::read<int, int>(std::abs, "std::abs");
	std::abs(-3);
	//// mixin
	//mixin((unsigned char*)&print4e110, other);
	//// Call final time
	//print4e110();
	//mixin((unsigned char*)&other, Example::function3);
	//print4e110();
	//mixin((unsigned char*)&print4e110, Example::function2);
	//print4e110();
}


	//auto p = getCodeSectionStart(Example::function1);
	//FunctionReader reader;

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
