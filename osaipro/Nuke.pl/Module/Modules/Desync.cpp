#include "Desync.h"

Desync::Desync() : IModule(0, Category::MISC, "Automatically disables the function only when eating") {
}
Desync::~Desync() {
}
const char* Desync::getModuleName() {
	return ("Desync");
}