#include "Silent.h"

Silent::Silent() : IModule(0, Category::MISC, "All Module is Auto Silent:)") {
}

Silent::~Silent() {
}
const char* Silent::getModuleName() {
	return ("Silent");
}