#pragma once
#include <string>

class FilePath {
private:
	char pad_0x0[0x18];
public:
	std::string filePathStr;
};