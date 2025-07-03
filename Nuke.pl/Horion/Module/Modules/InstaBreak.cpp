#include "InstaBreak.h"

InstaBreak::InstaBreak() : IModule(0, Category::WORLD, "Break any block instantly.") {
}

InstaBreak::~InstaBreak() {
}

const char* InstaBreak::getModuleName() {
	return ("InstaBreak");
}
