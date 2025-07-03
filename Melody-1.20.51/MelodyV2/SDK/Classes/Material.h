// Created by Tony on 2024-10-10 10:01:54

#pragma once

class Material {
public:
	bool isSolidBlocking() {
		if (*(bool*)((__int64)this + 4)) return false;
		return *(bool*)((__int64)this + 12);
	}
};