#include <iostream>

#include "Target.h"
#include "Mixin.h"
#include "FunctionReader.h"


void test() {
	std::cout << "Returning from branchExplorer!" << std::endl;
}

void entering() {
	std::cout << "Entering test2!" << std::endl;
}

void returning() {
	std::cout << "Returning from test2!\n\n" << std::endl;
}

int test2(bool x) {
	if(x) {
		std::cout << "Hello!" << std::endl;
		return 3;
	}
	if(!x) {
		std::cout << "Goodbye?" << std::endl;
		return 4;
	}
	std::cout << "OOF" << std::endl;
	return 0;
}

int main() {
	//std::vector<Target> targets = Target::Return(Target::branchExplorer).build();
	//Mixin::inject(test, targets);

	//test2(true);
	//test2(false);

	Mixin::inject(entering, std::vector<Target>({ Target::Head(test2).build() }));
	Mixin::inject(returning, Target::Return(test2).build());

	test2(true);
	test2(false);
}
