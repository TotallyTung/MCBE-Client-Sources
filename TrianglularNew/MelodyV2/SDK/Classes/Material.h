#pragma once

class Material {
public:
	bool isSolidBlocking() {
		if (*(bool*)((__int64)this + 4)) return false;
		return *(bool*)((__int64)this + 12);
	}
};