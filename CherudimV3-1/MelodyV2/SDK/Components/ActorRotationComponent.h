#pragma once

#include "../../Utils/Math.h"

struct ActorRotationComponent
{
public:
	Vec2<float> rotation;
	Vec2<float> oldRotation;

	ActorRotationComponent(const Vec2<float>& rotation) : rotation(rotation), oldRotation(rotation) {}

	[[nodiscard]] const Vec2<float>& Get() const
	{
		return this->rotation;
	}

	void Set(Vec2<float> rots)
	{
		this->rotation = rots;
	}
};