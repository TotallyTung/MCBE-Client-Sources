#include "FullBright.h"

FullBright::FullBright() : IModule(0, Category::VISUAL, "Puts your gamma to max.") {
}

FullBright::~FullBright() {
}

const char* FullBright::getModuleName() {
	return "Fullbright";
}
