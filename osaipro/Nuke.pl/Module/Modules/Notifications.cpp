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
		if (!notifList.empty()) notifList.clear();
		return;
	}

	static const float textSize = 1.f;
	const float textHeight = ImGuiUtils::getTextHeight(textSize);
	static const float textPadding = 6.f * textSize;
	static const float boxPadding = 10.f;
	static const float rounding = 8.f; // Rounded corners

	Vec2 windowSize = Game.getGuiData()->windowSizeReal;
	float yStart = windowSize.y - textHeight - 40.f * textSize;

	const float animSpeed = ImGui::GetIO().DeltaTime * 15.f;

	for (int i = 0; i < notifList.size(); i++) {
		std::shared_ptr<NotificationBox>& notifBox = notifList[i];

		if (notifBox->duration == notifBox->maxDuration) {
			notifBox->PosX = windowSize.x + 5.f;
			notifBox->PosY = yStart;
		}

		float textWidth = ImGuiUtils::getTextWidth(notifBox->message.c_str(), textSize);
		float boxWidth = textWidth + 2 * boxPadding;
		float boxHeight = textHeight + 2 * textPadding;

		float KC1 = textWidth + 20.f;
		float KC2 = windowSize.x - notifBox->PosX;
		float alphaFactor = KC2 / KC1;
		if (alphaFactor > 1.f) alphaFactor = 1.f;
		Vec2 rectMin = Vec2(notifBox->PosX - boxPadding, notifBox->PosY - boxPadding);
		Vec2 rectMax = Vec2(notifBox->PosX + boxWidth + boxPadding, notifBox->PosY + boxHeight + boxPadding);
		Vec2 textPos = Vec2(notifBox->PosX + textPadding, notifBox->PosY);
		UIColor a = moduleMgr->getModule<Colors>()->getColor();
		drawlist->AddRectFilled(
			ImVec2(rectMin.x, rectMin.y),
			ImVec2(rectMax.x, rectMax.y),
			IM_COL32(30, 30, 30, (int)(200 * alphaFactor)),
			rounding
		);
		drawlist->AddRect(
			ImVec2(rectMin.x, rectMin.y),
			ImVec2(rectMax.x, rectMax.y),
			IM_COL32(255, 255, 255, (int)(100 * alphaFactor)),
			rounding
		);

		ImGuiUtils::drawText(Vec2(textPos.x + 1, textPos.y + 1), notifBox->message.c_str(), UIColor(0, 0, 0, (int)(150 * alphaFactor)), textSize, false);
		ImGuiUtils::drawText(textPos, notifBox->message.c_str(), UIColor(a.r, a.g, a.b, (int)(255 * alphaFactor)), textSize, false);
		if (notifBox->duration > 0.f) {
			notifBox->PosX = smoothLerp(windowSize.x - boxWidth - 15.f, notifBox->PosX, animSpeed);
			notifBox->PosY = smoothLerp(yStart, notifBox->PosY, animSpeed);
			notifBox->duration -= ImGui::GetIO().DeltaTime;
			if (notifBox->duration < 0.f) notifBox->duration = 0.f;
		}
		else {
			notifBox->VelocicyX += 3.f;
			notifBox->PosX += notifBox->VelocicyX;
			notifBox->PosY = smoothLerp(yStart, notifBox->PosY, animSpeed);

			if (notifBox->PosX > windowSize.x) {
				notifList.erase(notifList.begin() + i);
				i--;
			}
		}

		yStart -= (boxHeight + 15.f * textSize) * alphaFactor;
	}
}
