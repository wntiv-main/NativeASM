#pragma once

#include <functional>
#include <vector>

#include "Instruction.h"

class Target {
private:
	using byte = unsigned char;
	using CodeSection = std::pair<void*, void*>;
	using CodeSections = std::vector<CodeSection>;
public:
	class BranchWiseData {
	private:
		CodeSections visited = {};
		void* currentBlockStart;
	public:
		const int size;
		int branchId = 0;
		void* data;
		void* address;

		void addSection(CodeSection section);
		bool hasVisited(void* addr);

		BranchWiseData(int size = 0);
		BranchWiseData(const BranchWiseData& other);
		BranchWiseData(BranchWiseData&& other) noexcept;
		friend class Target;
	};
	enum class Shift {
		BEFORE,
		AFTER
	};
private:
	using Visitor = std::function<bool(Instruction, BranchWiseData&)>;
	using Predicate = std::function<bool(Instruction)>;
	byte* address = nullptr;
	Target(byte* target);

	unsigned char peek();
	int peekInt();
	unsigned int peekUint();
	byte read();
	int readInt();
	unsigned int readUint();
	void remove(int num);
	void write(int num, byte* bytes);
	void write(int num, long bytes);
	void insert(int num, byte* bytes);
	void insertSpace(int num);
	void seek(int num);
	void seekTo(int search);
	void seekToOpcode(unsigned char search);
	void jump();
	static bool branchExplorer(Visitor visitor, byte* addr, BranchWiseData data, int& branchCount);
public:
	byte* getAddress();

	class Head {
	protected:
		using byte = Target::byte;
		byte* address;
	public:
		Head(void* method);
		Target build();
	};

	class Operation {
	protected:
		using byte = Target::byte;
		using Predicate = Target::Predicate;
		byte* address;
		unsigned char opcode = 0;
		Predicate isSameOpcode = [this](Instruction instr) -> bool { return instr.getOpcode() == opcode; };
		Predicate predicate_ = [this](Instruction instr) -> bool { return true; };
	public:
		Operation(void* method, unsigned char opcode);
		Operation& predicate(Predicate predicate);
		std::vector<Target> build();
	};

	class Return : public Target::Operation {
	public:
		Return(void* method);
	};

	friend class Mixin;
	friend int main();
};
