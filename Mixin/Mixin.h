#pragma once

#include "Target.h"

#include <vector>

class Mixin {
public:
	static void inject(void(*injection)(), std::vector<Target> at);
};
