#pragma once
#include "../Module.h"

struct NotificationBox {
	float duration;
	float maxDuration;
	std::string message;
	float PosX;
	float PosY;
	float VelocicyX;

	NotificationBox(std::string Message, float Duration) {
		this->message = Message;
		this->duration = Duration;
		this->maxDuration = Duration;
		this->VelocicyX = 0.f;
	}
};

class Notifications : public Module {
private:
	static inline std::vector<std::shared_ptr<NotificationBox>> notifList;
public:
	Notifications();
	~Notifications();
	static void addNotifBox(std::string message, float duration);
	static void Render(ImDrawList* drawlist);

	virtual bool isVisible() override;
};
