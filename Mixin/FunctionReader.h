#pragma once

#include "FunctionDescriptor.h"
#include <iostream>
#include <sstream>
#include "Instruction.h"

template <class T, class... R>
unsigned char* getCodeSectionStart(T(&f)(R...)) {
	auto p = (unsigned char*)f;

	// Looking for four CC bytes preceded by zero-initialised memory
	while (*(unsigned int*)p != 0xcccccccc || *(unsigned int*)(p - 8) != 0) {
		p--;
	}
	return p;
}

static unsigned char* lastReadLocation;

class FunctionReader {
private:
	static void readError(int signal);

	static unsigned char tryReadByte(unsigned char* ptr);

	static unsigned char readByteOrDefault(unsigned char* ptr, unsigned char def);

	static std::vector<unsigned char> tryRead(unsigned char* ptr);

	static unsigned char getReg(unsigned char modRMByte) {
		return (modRMByte >> 3) & 0b0111; // Pg 2672 // Pg 510
	}

	static int mov(bool reverse, unsigned char* ip) {
		int args = 0;
		std::string dir = reverse ? " -> " : " <- ";
		switch (*ip) {
		case 0x15:
			args = 5;
			std::cout << "mov edx" << dir << "dword ptr " << *(unsigned int*)(ip + 1) << std::endl;
			break;
		case 0x45:
			args = 2;
			std::cout << "mov eax" << dir << "dword ptr " << (int)*(ip + 1) << std::endl;
			break;
		case 0x4d:
			args = 2;
			std::cout << "mov ecx" << dir << "dword ptr " << (int)*(ip + 1) << std::endl;
			break;
		case 0xc8:
			args = 1;
			std::cout << "mov ecx" << dir << "eax " << (int)*ip << std::endl;
			break;
		case 0xec:
			args = 1;
			std::cout << "mov ebp" << dir << "esp " << (int)*ip << std::endl;
			break;
		case 0xf4:
			args = 1;
			std::cout << "mov esi" << dir << "esp " << (int)*ip << std::endl;
			break;
		case 0xfc:
			args = 1;
			std::cout << "mov edi" << dir << "esp " << (int)*ip << std::endl;
			break;
		default:
			throw std::runtime_error("not impl");
		}
		return args;
	}
public:
	template <class Return, class... Args>
	static void read2(Return(&func)(Args...), std::string name) {
		FunctionDescriptor desc = { func };
		std::cout << desc.getReturnTypeName() << " " << name << " (" << desc.getArgumentTypeNames() << "):" << std::endl;
		unsigned char* ip = (unsigned char*)func;
		Instruction instr;
		do {
			instr = Instruction(ip);
			if(instr.getOpcode() == 0xe9) {
				ip += instr.getOperands()[0].getImmediate();
			}
		} while(instr.getOpcode() != 0xc3 && instr.getOpcode() != 0xcc);
	}

	template <class Return, class... Args>
	static void read(Return(&func)(Args...), std::string name) {
		unsigned char* codeStart = getCodeSectionStart(func);
		FunctionDescriptor desc = { func };
		std::cout << desc.getReturnTypeName() << " " << name << " (" << desc.getArgumentTypeNames() << "):" << std::endl;
		unsigned char* ip = (unsigned char*)func;
		unsigned char* funcStart = ip;
		while (!((ip[-1] == 0xc3 || ip[-3] == 0xc2) && ip[0] == 0xcc && ip[1] == 0xcc) || !(((unsigned int*)ip)[0] == 0xcccccccc && ((unsigned int*)ip)[1] == 0xcccccccc)) {
			std::cout << std::hex << (*ip & 0xf0 ? "" : "0") << (int)*ip << " ";
			if (ip - funcStart >= 4 && ip[-4] == 0xe9) {
				ip += *(int*)(ip - 3);
				std::cout << "-> ( 0x" << (size_t)(ip + 1) << " ):" << std::endl;
			}
			if (ip - funcStart >= 4 && ip[-4] == 0x68) {
				std::cout << "-> ( " << *(unsigned int*)(ip - 3) << " -> ";
				std::vector<unsigned char> bytes = tryRead((unsigned char*)*(unsigned int*)(ip - 3));
				bool readable = true;
				std::string str = "\"";
				for (unsigned char byte : bytes) {
					str += byte;
					if (!isprint(byte)) readable = false;
				}
				if (readable) {
					std::cout << str << "\" ";
				}
				else {
					for (unsigned char byte : bytes) {
						std::cout << std::hex << (byte & 0xf0 ? "" : "0") << (int)byte << " ";
					}
				}
				std::cout << ") ";
			}
			ip++;
		}
		std::cout << std::endl;
	}

	template <class Return, class... Args>
	static void read(Return(&func)(Args...)) {
		read(func, "anonymous");
	}
};