#include "Notifications.h"
#include "../../../Client.h"

Notifications::Notifications() : Module("Notifications", "Show notification", Category::CLIENT) {
	clickme("Save config", "save the current config");
}

Notifications::~Notifications() {
	notifList.clear();
}

bool Notifications::isVisible() {
	return false;
}

void Notifications::addNotifBox(std::string message, float duration) {
	std::shared_ptr<NotificationBox> notif = std::make_shared<NotificationBox>(message, duration);
	notifList.push_back(notif);
}
void Notifications::Render(ImDrawList* drawlist) {
	if (!client->isInitialized()) return;
	static Notifications* notifBox = (Notifications*)client->moduleMgr->getModule("Notifications");
	if (!notifBox->isEnabled()) {
		if (notifList.size() != 0) notifList.clear();
		return;
	}
	static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
	UIColor mainColor(colorsMod->getColor());
	static const float textSize = 1.1f;
	const float textHeight = ImGuiUtils::getTextHeight(textSize);
	static const float textPadding = 2.f * textSize;
	Vec2 windowSize = mc.getClientInstance()->guiData->windowSizeReal;
	float yStart = windowSize.y - textHeight - 30.f * textSize;
	const float animSpeed = ImGui::GetIO().DeltaTime * 15.f;
	for (int i = 0; i < notifList.size(); i++) {
		std::shared_ptr<NotificationBox>& notifBox = notifList[i];
		if (notifBox->duration == notifBox->maxDuration) {
			notifBox->PosX = windowSize.x + 5.f;
			notifBox->PosY = yStart;
		}
		float textWidth = ImGuiUtils::getTextWidth(notifBox->message.c_str(), textSize);
		float KC1 = textWidth + 10.f;
		float KC2 = windowSize.x - notifBox->PosX;
		float idk = KC2 / KC1;
		if (idk > 1.f) idk = 1.f;
		Vec4 rectPos = Vec4(notifBox->PosX - textPadding * 2.f, notifBox->PosY - textPadding * 2.f, notifBox->PosX + textWidth + textPadding * 2.f, notifBox->PosY + textHeight + textPadding);
		Vec2 textPos = Vec2(notifBox->PosX, notifBox->PosY - 1.f * textSize);
		ImGuiUtils::fillRectangle(rectPos, UIColor(0, 0, 0, (int)(110 * idk)), 6.f);
		const float sidelineThickness = 8.0f; 
		Vec4 sidelineRect = Vec4(rectPos.x - 7.f, rectPos.y, rectPos.x, rectPos.w);
		ImGuiUtils::fillRectangle(sidelineRect, UIColor(mainColor.r, mainColor.g, mainColor.b, (int)(255 * idk)), 6.f); 
		if (notifBox->duration != 0.f) {
		}
		ImGuiUtils::drawText(textPos, notifBox->message.c_str(), UIColor(255, 255, 255, (int)(255 * idk)), textSize, false);
		if (notifBox->duration > 0.f) {
			notifBox->PosX = Math::lerp(windowSize.x - textWidth - 10.f, notifBox->PosX, animSpeed);
			notifBox->PosY = Math::lerp(yStart, notifBox->PosY, animSpeed);
			notifBox->duration -= ImGui::GetIO().DeltaTime;
			if (notifBox->duration < 0.f) notifBox->duration = 0.f;
		}
		else {
			notifBox->VelocicyX += 3.f;
			notifBox->PosX += notifBox->VelocicyX;
			notifBox->PosY = Math::lerp(yStart, notifBox->PosY, animSpeed);

			if (notifBox->PosX > windowSize.x) {
				notifList.erase(notifList.begin() + i);
				i--;
			}
		}

		yStart -= (textHeight + textPadding + 15.f * textSize) * idk;
	}
}