#pragma once
class Target {
public:
	enum class Type {
		UNDEFINED,
		HEAD,
		TAIL,
		RETURN,
		OPCODE
	};
private:
	const Type at;
	const unsigned char opcode;
	// Ordinal specifies which n^th operation should be
	// targeted, default is -1, meaning all matching targets.
	const int ordinal = -1;
	void* address = nullptr;
	Target(void* address);
	Target(Type at, unsigned char opcode, int ordinal);

	unsigned char peek();
	int peekInt();
	unsigned int peekUint();
	unsigned char read();
	int readInt();
	unsigned int readUint();
	void remove(int num);
	void write(int num, void* bytes);
	void write(int num, long bytes);
	void insert(int num, void* bytes);
	void insertSpace(int num);
	void seek(int num);
	void seekTo(int search);
	void jump();
public:
	class Builder {
	private:
		Target::Type _at = Target::Type::UNDEFINED;
		unsigned char _opcode = 0xcc;
		int _ordinal = -1;
		void* address = nullptr;
	public:
		Builder& at(Target::Type x);
		Builder& at(void* x);
		Builder& opcode(unsigned char x);
		Builder& ordinal(int x);

		Target build();
	};
	friend class Mixin;
	friend int main();
};
