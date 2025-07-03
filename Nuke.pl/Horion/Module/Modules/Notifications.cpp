#include "Notifications.h"

#include "../../Utils/ImGuiUtils.h"
#include "../ModuleManager.h"

Notifications::Notifications() : IModule(0x0, Category::CLIENT, "Show notification") {
	setEnabled(true);
}

Notifications::~Notifications() {
	notifList.clear();
}

bool Notifications::isVisible() {
	return false;
}

const char* Notifications::getModuleName() {
	return "Notifications";
}

void Notifications::addNotifBox(std::string message, float duration) {
	std::shared_ptr<NotificationBox> notif = std::make_shared<NotificationBox>(message, duration);
	notifList.push_back(notif);
	// notifList.emplace_back(new NotificationBox(message, duration));
}

void Notifications::Render(ImDrawList* drawlist) {
	if (!moduleMgr->isInitialized()) return;
	static Notifications* notifBox = moduleMgr->getModule<Notifications>();
	if (!notifBox->isEnabled()) {
		if (notifList.size() != 0) notifList.clear();
		return;
	}
	static const float textSize = 1.f;
	const float textHeight = ImGuiUtils::getTextHeight(textSize);
	static const float textPadding = 2.f * textSize;

	Vec2 windowSize = Game.getGuiData()->windowSizeReal;
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

		Vec4 rectPos;
		Vec2 textPos;
			rectPos = Vec4(notifBox->PosX - textPadding * 2.f, notifBox->PosY - textPadding * 2.f, notifBox->PosX + textWidth + textPadding * 2.f, notifBox->PosY + textHeight + textPadding);
			textPos = Vec2(notifBox->PosX, notifBox->PosY - 1.f * textSize);
		ImGuiUtils::drawRectFilled(rectPos, UIColor(0, 0, 0, (int)(110 * idk)), 0.f);
		ImGuiUtils::drawRectFilled(Vec4(rectPos.x, rectPos.w - 1.f * textSize, rectPos.z, rectPos.w), UIColor(0, 0, 0, (int)(75 * idk)), 0.f);
		if (notifBox->duration != 0.f) {
			Vec4 underline = Vec4(rectPos.x, rectPos.w - 1.f * textSize, rectPos.x + (rectPos.z - rectPos.x) * (notifBox->duration / notifBox->maxDuration), rectPos.w);
			ImGuiUtils::drawRectFilled(underline, UIColor(255, 255, 255, (int)(255 * idk)), 0.f);
		}
		ImGuiUtils::drawText(textPos, notifBox->message.c_str(), UIColor(255, 255, 255, (int)(255 * idk)), textSize, false);
		if (notifBox->duration > 0.f) {
			notifBox->PosX = smoothLerp(windowSize.x - textWidth - 10.f, notifBox->PosX, animSpeed);
			notifBox->PosY = smoothLerp(yStart, notifBox->PosY, animSpeed);
			notifBox->duration -= ImGui::GetIO().DeltaTime;
			if (notifBox->duration < 0.f) notifBox->duration = 0.f;
		} else {
			notifBox->VelocicyX += 3.f;
			notifBox->PosX += notifBox->VelocicyX;
			notifBox->PosY = smoothLerp(yStart, notifBox->PosY, animSpeed);

			if (notifBox->PosX > windowSize.x) {
				notifList.erase(notifList.begin() + i);
				i--;
			}
		}

		yStart -= (textHeight + textPadding + 15.f * textSize) * idk;
	}
}