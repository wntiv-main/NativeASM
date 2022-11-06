#include "FunctionReader.h"

#include <windows.h>
#include <eh.h>
#include <exception>
#include <signal.h>

#ifndef __try1
#define __try1 __try
#define __except1 __except
#endif

void FunctionReader::readError(int signal) {
	std::cerr << "Read Access Violation while reading 0x" << std::hex << (int)lastReadLocation;
	throw std::runtime_error("read access exception");
}

// UNSAFE pointer dereference
unsigned char FunctionReader::tryReadByte(unsigned char* ptr) {
	lastReadLocation = ptr;
	void(*previousHandler)(int) = signal(SIGSEGV, FunctionReader::readError);
	unsigned char data;
#ifdef _WIN32
	__try1
	{
		data = *ptr;
	}
	__except1(EXCEPTION_EXECUTE_HANDLER)
	{
		readError(SIGSEGV);
	}
#else
	data = *ptr;
#endif // _WIN32
	signal(SIGSEGV, previousHandler);
	return data;
}

unsigned char FunctionReader::readByteOrDefault(unsigned char* ptr, unsigned char def)
{
	try {
		return tryReadByte(ptr);
	}
	catch (std::runtime_error e) {
		return def;
	}
}

std::vector<unsigned char> FunctionReader::tryRead(unsigned char* ptr) {
	std::vector<unsigned char> buffer;
	for (int i = 0; i < 64; i++) {
		unsigned char byte = readByteOrDefault(ptr + i, 0x00);
		if (byte == 0x00) break;
		buffer.push_back(byte);
	}
	return buffer;
}