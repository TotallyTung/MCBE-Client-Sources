#include "FullBright.h"

FullBright::FullBright() : Module(0x0, Category::RENDER, "FullBright") {
}

const char* FullBright::getModuleName() {
	return "FullBright";
}