#include "Mixin.h"

void Mixin::inject(void(*injection)(), std::vector<Target> at) {
	for(Target pos : at) {
		unsigned char bytes[5] = {0xe8};
		*(int*)(bytes + 1) = (unsigned char*)injection - pos.address - 5;
		pos.insert(5, bytes);
	}
}
