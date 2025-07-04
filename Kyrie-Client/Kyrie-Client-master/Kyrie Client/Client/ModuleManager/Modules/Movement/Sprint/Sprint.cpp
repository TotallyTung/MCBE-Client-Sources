#include "Sprint.h"

Sprint::Sprint() : Module(0x0, Category::MOVEMENT, "Sprint") {
}

const char* Sprint::getModuleName() {
	return "Sprint";
}

const char* Sprint::getModName() {
	return "Legit";
}