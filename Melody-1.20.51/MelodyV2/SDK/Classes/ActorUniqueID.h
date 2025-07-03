// Created by Tony on 2024-10-10 10:01:54

#pragma once

class ActorUniqueID {
	int64_t leId;
public:
	ActorUniqueID(int64_t id) {
		this->leId = id;
	}
	int64_t GetID() const {
		return this->leId;
	}
	int64_t operator=(const ActorUniqueID& rhs) const {
		return rhs.GetID();
	}
};