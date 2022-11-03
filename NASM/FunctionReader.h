#pragma once

#include "FunctionDescriptor.h"
#include "Instructions/Instructions.h"
#include "Instructions/ReturnInstruction.h"
#include <iostream>
#include <sstream>

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
	static void read3(Return(&func)(Args...), std::string name, std::ostream& out = std::cout, std::string indent = "") {
		unsigned char* codeStart = getCodeSectionStart(func);
		// Print function description
		FunctionDescriptor desc(func);
		out << indent << desc.getReturnTypeName() << " " << name << " (" << desc.getArgumentTypeNames() << "):" << std::endl;
		// Start reading "bytecode"
		unsigned char* ip = (unsigned char*)func;
		Instruction* instr;
		do {
			instr = Instructions::readInstruction(ip);
			out << instr->toString(indent) << std::endl;
		} while (instr->getOpcode() != ReturnInstruction::opcode && ((delete instr), true));
		delete instr;
	}

	template <class Return, class... Args>
	static void read2(Return(&func)(Args...), std::string name) {
		unsigned char* codeStart = getCodeSectionStart(func);
		// Print function description
		FunctionDescriptor desc(func);
		std::cout << desc.getReturnTypeName() << " " << name << " (" << desc.getArgumentTypeNames() << "):" << std::endl;
		// Start reading "bytecode"
		unsigned char* ip = (unsigned char*)func;
		while (!(((unsigned int*)ip)[0] == 0xcccccccc && ((unsigned int*)ip)[1] == 0xcccccccc)) {
			unsigned char opcode = *ip;
			std::cout << "0x" << (size_t)ip << "\t";
			ip++;
			int args = 0;
			switch (opcode) {
			case 0x00:
				args = 1;
				std::cout << "add " << (int)*ip << " + al" << std::endl;
				break;
			case 0x0f:
				args = 3;
				std::cout << "movzx " << (int)*ip << " " << (int)*(ip + 1) << std::endl;
				break;
			case 0x24:
				args = 1;
				std::cout << "and al & " << (int)*ip << std::endl;
				break;
			case 0x33:
				args = 1;
				std::cout << "xor eax x " << (int)*ip << std::endl;
				break;
			case 0x45:
				std::cout << "inc ebp" << std::endl;
				break;
			case 0x50:
				std::cout << "push eax" << std::endl;
				break;
			case 0x51:
				std::cout << "push ecx" << std::endl;
				break;
			case 0x52:
				std::cout << "push edx" << std::endl;
				break;
			case 0x53:
				std::cout << "push ebx" << std::endl;
				break;
			case 0x55:
				std::cout << "push ebp" << std::endl;
				break;
			case 0x56:
				std::cout << "push esi" << std::endl;
				break;
			case 0x57:
				std::cout << "push edi" << std::endl;
				break;
			case 0x64:
				args = 5;
				std::cout << "mov eax <- " << (int)*ip << " " << *(unsigned int*)(ip + 1) << std::endl;
				break;
			case 0x68: {// push
				args = 4;
				std::cout << "push " << *(unsigned int*)ip << " -> ";
				std::vector<unsigned char> bytes = tryRead((unsigned char*)*(unsigned int*)ip);
				bool readable = true;
				std::string str = "\"";
				for (unsigned char byte : bytes) {
					str += byte;
					if (!isprint(byte)) readable = false;
				}
				if (readable) {
					std::cout << str << "\"";
				}
				else {
					std::cout << "[ ";
					for (unsigned char byte : bytes) {
						std::cout << std::hex << (byte & 0xf0 ? "" : "0") << (int)byte << " ";
					}
					std::cout << "]";
				}
				std::cout << std::endl;
			}
					 break;
			case 0x6a:
				args = 1;
				std::cout << "push " << (int)*ip << std::endl;
				break;
			case 0x74:
				args = 1;
				std::cout << "je " << (int)*ip << std::endl;
				break;
			case 0x75:
				args = 1;
				std::cout << "jne " << (int)*ip << std::endl;
				break;
			case 0x81:
				args = 5;
				std::cout << "sub " << (int)*ip << " " << *(unsigned int*)(ip + 1) << std::endl;
				break;
			case 0x83:
				args = 2;
				std::cout << "add " << (int)*ip << " + " << (int)*(ip + 1) << std::endl;
				break;
			case 0x85:
				args = 1;
				std::cout << "test " << (int)*ip << std::endl;
				break;
			case 0x89:
				args = mov(true, ip);
				break;
			case 0x8b:
				args = mov(false, ip);
				break;
			case 0x8d:
				switch (*ip) {
				case 0x45:
					args = 2;
					std::cout << "lea edi " << (int)*ip << " " << (int)*(char*)(ip + 1) << std::endl;
					break;
				case 0x4d:
					args = 2;
					std::cout << "lea ecx " << (int)*ip << " " << (int)*(ip + 1) << std::endl;
					break;
				case 0xBD:
					args = 5;
					std::cout << "lea edi ebp" << (int)*ip << " " << *(unsigned int*)(ip + 1) << std::endl;
					break;
				default:
					throw std::runtime_error("not impl");
				}
				break;
			case 0xa1:
				args = 4;
				std::cout << "mov eax <- dword ptr " << *(unsigned int*)ip << std::endl;
				break;
			case 0xb6:
				args = 1;
				std::cout << "mov dh <- " << (int)*ip << std::endl;
				break;
			case 0xb8:
				args = 4;
				std::cout << "mov eax <- " << *(unsigned int*)ip << std::endl;
				break;
			case 0xb9:
				args = 4;
				std::cout << "mov ecx <- " << *(unsigned int*)ip << std::endl;
				break;
			case 0xc7:
				args = 6;
				std::cout << "mov dword ptr " << (int)*ip << " " << (int)*(char*)(ip + 1) << " <- " << *(unsigned int*)(ip + 2) << std::endl;
				break;
			case 0xe8: {
				args = 4;
				unsigned char* funcPtr = (ip + *(unsigned int*)ip + 4);
				std::cout << "call -> 0x" << (size_t)funcPtr << std::endl;
				std::ostringstream ptrStr;
				ptrStr << "0x" << std::hex << (size_t)funcPtr;
				read(*(FunctionDescriptor::Unknown(*)(FunctionDescriptor::Unknown))funcPtr, ptrStr.str());
				std::cout << "CODE EXECUTION RETURNED" << std::endl;
			}
					 break;
			case 0xe9: // jmp
				args = 4;
				if ((*(unsigned int*)ip & 0xff000000) != 0) {
					std::cerr << std::hex << *(unsigned int*)ip;
					throw std::runtime_error("not impl");
				}
				ip += *(unsigned int*)ip;
				std::cout << "jmp -> 0x" << (size_t)(ip + 4) << std::endl;
				break;
			case 0xf3:
				switch (*ip) {
				case 0xab:
					args = 1;
					std::cout << "rep stos  dword ptr es:[edi]" << std::endl;
					break;
				default:
					throw std::runtime_error("not impl");
				}
				break;
			case 0xfc:
				std::cout << "cld" << std::endl;
				break;
			case 0xff: {
				args = 5;
				unsigned char* funcAddr = (ip + *(unsigned int*)(ip + 1) + 5);
				std::cout << "call -> " << (int)*ip << " 0x" << (size_t)funcAddr << std::endl;
				std::ostringstream ptrStr;
				ptrStr << "0x" << std::hex << (size_t)funcAddr;
				read(*(FunctionDescriptor::Unknown(*)(FunctionDescriptor::Unknown))funcAddr, ptrStr.str());
				std::cout << "CODE EXECUTION RETURNED" << std::endl;
			}
					 break;
			default:
				std::cerr << std::hex << "Opcode " << (int)opcode << " not implemented at 0x" << (size_t)(ip - 1) << std::endl;
				throw std::runtime_error("File can't be read, unimplemented opcode.");
				break;
			}
			ip += args;
		}
		std::cout << std::endl;
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