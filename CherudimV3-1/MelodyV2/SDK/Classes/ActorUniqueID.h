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