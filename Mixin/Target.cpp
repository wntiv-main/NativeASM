#include "Target.h"

#include <stdexcept>
#include <iostream>

// Target

Target::Target(byte* target)
	: address(target) {}

unsigned char Target::peek() {
	return *(unsigned char*)address;
}

int Target::peekInt() {
	return *(int*)address;
}

unsigned int Target::peekUint() {
	return (unsigned int)peekInt();
}

Target::byte Target::read() {
	byte result = peek();
	address += sizeof(byte);
	return result;
}

int Target::readInt() {
	int result = peekInt();
	address += sizeof(int);
	return result;
}

unsigned int Target::readUint() {
	return (unsigned int)readInt();
}

void Target::remove(int num) {
	byte* addr = address;
	byte* oldAddr = addr;
	for(int i = 0; i < num; i++) {
		Instruction instr(addr);
	}
	int bytesDeleting = addr - oldAddr;
	while(true) {
		Instruction instr(addr);
		while(oldAddr + bytesDeleting < addr) {
			*oldAddr = *(oldAddr + bytesDeleting);
			oldAddr++;
		}
		if(instr.returns() || instr.getOpcode() == 0xc3) {
			break;
		}
	}
	while(oldAddr < addr) {
		*oldAddr = 0xcc;
		oldAddr++;
	}
}

void Target::write(int num, long bytes) {
	write(num, (byte*)&bytes);
}

void Target::write(int num, byte* bytes) {
	for(int i = 0; i < num; i++) {
		*(address + i) = *(bytes + i);
	}
	seek(num);
}

void Target::insert(int num, byte* bytes) {
	insertSpace(num);
	seek(-num);
	write(num, bytes);
}

int remapOffset(void* startMov, int by, int bytesMoved, void* remapAddr) {
	if(remapAddr < startMov) {
		return -by;
	} else if(remapAddr <= (unsigned char*)startMov + bytesMoved) {
		return 0;
	} else {
		return -by;
	}
}

void Target::insertSpace(int num) {
	std::vector<byte> data;
	byte* addr = address;
	std::vector<Instruction> toRemap;
	while(true) {
		Instruction instr(addr);
		if(instr.jumps().jumps && instr.jumps().relative) {
			toRemap.push_back(instr);
		}
		for(int i = 0; i < instr.getSourceLength(); i++) {
			data.push_back(*(instr.getSourceLocation() + i));
		}
		if(instr.returns()) break;
	}
	for(int i = 0; i < num; i++) {
		*address = 0xcc;
		address++;
	}
	addr = address;
	for(byte b : data) {
		*addr = b;
		addr++;
	}
	for(Instruction remap : toRemap) {
		char* addr = (char*)remap.getJumpOpLocation() + num;
		if(remap.jumpOpSize() == 1) {
			*addr = *addr + remapOffset(address, num, data.size(), remap.getSourceLocation() + remap.getSourceLength() + *addr);
		} else if(remap.jumpOpSize() == 4) {
			*(int*)addr = *(int*)addr + remapOffset(address, num, data.size(), remap.getSourceLocation() + remap.getSourceLength() + *(int*)addr);
		}
	}
}

void Target::seek(int num) {
	address = address + num;
}

void Target::seekTo(int search) {
	while(*(int*)address != search) {
		address = address + 1;
	}
}

void Target::seekToOpcode(unsigned char search) {
	Instruction instr;
	while(instr.getOpcode() != search) {
		instr = Instruction(address);
	}
	address -= instr.getSourceLength();
}

void Target::jump() {
	if(peek() != 0xe9) throw std::runtime_error("Cannot jump, no jump instruction present");
	seek(1);
	address += readInt();
}

bool Target::branchExplorer(Visitor visitor, byte* addr, BranchWiseData data, int& branchCount) {
	data.branchId = branchCount++;
	data.currentBlockStart = addr;
	Instruction instr;
	while(true) {
		data.address = addr;
		instr = Instruction(addr);
		if(visitor(instr, data) || instr.returns()) break;
		if(instr.jumps().jumps && !instr.jumps().functional) {
			data.addSection({ data.currentBlockStart, addr });
			byte* jumpAddr = nullptr;
			if(instr.jumps().relative) {
				jumpAddr = addr + instr.jumpsTo();
			} else {
				jumpAddr = (byte*)instr.jumpsTo();
			}
			if(data.hasVisited(jumpAddr)) return false;
			if(instr.jumps().conditional) {
				branchExplorer(visitor, jumpAddr, data, branchCount);
			} else {
				addr = jumpAddr;
			}
		}
	}
	return true;
}

Target::byte* Target::getAddress() {
	return address;
}

void Target::BranchWiseData::addSection(CodeSection section) {
	visited.push_back(section);
}

bool Target::BranchWiseData::hasVisited(void* addr) {
	for(CodeSection section : visited) {
		if(addr >= section.first && addr < section.second) {
			return true;
		}
	}
	if(addr >= currentBlockStart && addr <= address) return true;
	return false;
}

Target::BranchWiseData::BranchWiseData(int size)
	: size(size) {
	data = new unsigned char[size] {0};
	branchId = 0;
	address = nullptr;
	currentBlockStart = nullptr;
}

Target::BranchWiseData::BranchWiseData(const BranchWiseData& other)
	: size(other.size) {
	data = new unsigned char[size] {0};
	memcpy(data, other.data, size);
	visited = other.visited;
	branchId = other.branchId;
	currentBlockStart = other.currentBlockStart;
}

Target::BranchWiseData::BranchWiseData(BranchWiseData&& other) noexcept
	: size(other.size) {
	data = other.data;
	other.data = nullptr;
	visited = other.visited;
	branchId = other.branchId;
	currentBlockStart = other.currentBlockStart;
}

Target::Head::Head(void* method)
	: address((byte*)method) {}

Target Target::Head::build() {
	byte* addr = (byte*)address;
	// If method begins with a jump instruction, follow it.
	Instruction instr(addr);
	if(instr.jumps() == Instruction::JumpType::UNCONDITIONAL_RELATIVE) {
		addr += instr.jumpsTo();
	} else if(instr.jumps() == Instruction::JumpType::UNCONDITIONAL_ABSOLUTE) {
		addr = (byte*)instr.jumpsTo();
	} else {
		addr = (byte*)address;
	}
	Target position = Target(addr);
	return position;
}

Target::Operation::Operation(void* method, unsigned char opcode)
	: address((byte*)method), opcode(opcode) {}

Target::Operation& Target::Operation::predicate(Predicate p) {
	predicate_ = p;
	return *this;
}

std::vector<Target> Target::Operation::build() {
	byte* addr = (byte*)address;
	int branches = 0;
	std::vector<Target> found;
	branchExplorer([&found, this](Instruction instr, BranchWiseData& data) -> bool {
		if(isSameOpcode(instr)) {
			for(Target t : found) {
				if(t.getAddress() == data.address) return false;
			}
			if(predicate_(instr)) {
				//std::cout << "INJECT" << std::endl;
				found.push_back(Target((byte*)data.address));
			}
		}
		//std::cout << data.branchId << " " << instr.getName() << std::endl;
		return false;
	}, addr, BranchWiseData(), branches);
	return found;
}

Target::Return::Return(void* method)
	: Operation(method, 0xc3) {
	isSameOpcode = [this](Instruction instr) -> bool { return instr.returns(); };
}
