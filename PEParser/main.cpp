#include <iostream>
#include <fstream>

#include "PEParser.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		std::cerr << "Program expects 1 argument, got " << argc;
		throw std::runtime_error("Not enough args");
	}
	std::fstream file;
	file.open(argv[1], std::ios::in | std::ios::out | std::ios::binary);
	try {
		PEParser parser(file);
	} catch(std::exception e) {
		file.close();
		throw e;
	}
	file.close();
	std::cout << "Hello World!\n";
}