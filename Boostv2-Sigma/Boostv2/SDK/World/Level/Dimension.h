#include "../Actor.h"

class Weather {
public:
	CLASS_MEMBER(float, fogLevel, 0x4C); // 1.21.2
	CLASS_MEMBER(float, lightingLevel, 0x44); // 1.21.2
	CLASS_MEMBER(float, rainLevel, 0x38); // 1.21.2
};
class Dimension { // is named mcDimension since it conflicts with the Dimension class in Engine.hpp
public:
	CLASS_MEMBER(Weather*, weather, 0x1B0); // 1.21.2
};
