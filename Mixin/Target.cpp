#include "Target.h"

#include <stdexcept>
#include <iostream>
//#include "Instructions/Instructions.h"
//#include "Instructions/ReturnInstruction.h"


// Target

Target::Target(Type at, unsigned char opcode, int ordinal)
	: at(at), opcode(opcode), ordinal(ordinal) {}

Target::Target(void* address)
	:address(address), at(Type::UNDEFINED), opcode(0) {}

unsigned char Target::peek() {
	return *(unsigned char*)address;
}

int Target::peekInt() {
	return *(int*)address;
}

unsigned int Target::peekUint() {
	return (unsigned int)peekInt();
}

unsigned char Target::read() {
	unsigned char result = peek();
	address = (unsigned char*)address + 1;
	return result;
}

int Target::readInt() {
	int result = peekInt();
	address = (int*)address + 1;
	return result;
}

unsigned int Target::readUint() {
	return (unsigned int)readInt();
}

void Target::remove(int num) {
	//unsigned char* addr = (unsigned char*)address;
	//unsigned char* oldAddr = addr;
	//addr += num;
	//while(true) {
	//	Instruction* instr = Instructions::readInstruction(addr);
	//	while(oldAddr + num < addr) {
	//		*oldAddr = *oldAddr + num;
	//		oldAddr++;
	//	}
	//	if(instr->getOpcode() == ReturnInstruction::opcode) {
	//		delete instr;
	//		break;
	//	}
	//	delete instr;
	//}
	//while(oldAddr < addr) {
	//	*oldAddr = 0xcc;
	//	oldAddr++;
	//}
}

void Target::write(int num, long bytes) {
	write(num, &bytes);
}

void Target::write(int num, void* bytes) {
	for(int i = 0; i < num; i++) {
		*((unsigned char*)address + i) = *((unsigned char*)bytes + i);
	}
	seek(num);
}

void Target::insert(int num, void* bytes) {
	insertSpace(num);
	write(num, bytes);
}

void Target::insertSpace(int num) {}

void Target::seek(int num) {
	address = (unsigned char*)address + num;
}

void Target::seekTo(int search) {
	while(*(int*)address != search) {
		address = (unsigned char*)address + 1;
	}
}

void Target::jump() {
	if(read() != 0xe9) throw std::runtime_error("Cannot jump, no jump instruction present");
	address = readInt() + (unsigned char*)address;
}

// Builder

Target::Builder& Target::Builder::at(Target::Type x) {
	_at = x;
	return *this;
}

Target::Builder& Target::Builder::at(void* x) {
	address = x;
	return *this;
}

Target::Builder& Target::Builder::opcode(unsigned char x) {
	_opcode = x;
	return *this;
}

Target::Builder& Target::Builder::ordinal(int x) {
	_ordinal = x;
	return *this;
}

Target Target::Builder::build() {
	if(address != nullptr) {
		return Target(address);
	}
	if(_at == Target::Type::UNDEFINED) throw std::runtime_error("Mixin injection point not defined.");
	return Target(_at, _opcode, _ordinal);
}
