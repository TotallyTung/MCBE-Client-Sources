#pragma once

class Weather {
public:
	BUILD_ACCESS(float, fogLevel, 0x50); // 1.21.73 ?
	BUILD_ACCESS(float, lightingLevel, 0x48); // 1.21.73
	BUILD_ACCESS(float, rainLevel, 0x3C); // 1.21.73
};