// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include "Actor.h"

enum class RotationType {
	REAL,
	RENDER
};

class ActorRotateEvent {
	Vec2<float> rotation;
	Vec2<float> oldRotation;
	RotationType rotationType;
public:
	ActorRotateEvent(Actor* actor, const Vec2<float>& rotation, const Vec2<float>& oldRotation, RotationType rotationType){
		this->rotation = rotation;
		this->oldRotation = oldRotation;
		this->rotationType = rotationType;
	};

	RotationType getRotationType() {
		return this->rotationType;
	}

	const Vec2<float>& getRotation() {
		return this->rotation;
	}
	void setRotation(const Vec2<float>& rot) {
		this->rotation = rot;
	}

	const Vec2<float>& getOldRotation() {
		return this->oldRotation;
	}
	void setOldRotation(const Vec2<float>& rot) {
		this->oldRotation = rot;
	}

	[[nodiscard]] float getYaw() const {
		return this->rotation.y;
	}
	void setYaw(float yaw) {
		this->rotation.y = yaw;
	}
	[[nodiscard]] float getOldYaw() const {
		return this->oldRotation.y;
	}
	void setOldYaw(float yaw) {
		this->oldRotation.y = yaw;
	}

	[[nodiscard]] float getPitch() const {
		return this->rotation.x;
	}
	void setPitch(float pitch) {
		this->rotation.x = pitch;
	}
	[[nodiscard]] float getOldPitch() const {
		return this->oldRotation.x;
	}
	void setOldPitch(float pitch) {
		this->oldRotation.x = pitch;
	}
};
