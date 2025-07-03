#pragma once
#include <string>
#include <vector>
#include "../../Utils/Maths.h"

struct NotificationBase {
	std::string message;
	float duration;
	float maxDuration;

	Vec2<float> pos;
	Vec2<float> vel;

	NotificationBase(std::string Message, float Duration = 3.f) {
		this->message = Message;
		this->duration = Duration;
		this->maxDuration = Duration;
	}
};

class NotificationManager {
private:
	static inline std::vector<NotificationBase*> notifList;
public:
	static inline void addNotification(std::string message, float duration) {
		notifList.push_back(new NotificationBase(message, duration));
	}
	static void Render();
};