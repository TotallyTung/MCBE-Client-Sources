// Created by Tony on 2024-10-10 10:01:54

#include "Notifications.h"
#include "../../../Client.h"
#include <unordered_set>

bool playerjoin = false;
std::unordered_set<std::string> previousPlayers;
Notifications::Notifications() : Module("Notifications", "Show notification", Category::CLIENT) {
	addBoolCheck("Player/join", "Notificcation you if a player has lefts", &playerjoin);
	addClickMeSetting("Save config", "save the current config");
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
void Notifications::onNormalTick(Actor* actor) {
	static Notifications* notificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");

	if (!notificationsMod->isEnabled() || !playerjoin) return;

	Level* level = mc.getLocalPlayer()->getLevel();
	if (!level) {
		notificationsMod->addNotifBox("Unable to access player list.", 3.0f);
		return;
	}

	const auto& playerMap = level->getPlayerMap();
	std::unordered_set<std::string> currentPlayers;

	for (const auto& pair : playerMap) {
		currentPlayers.insert(pair.second.name);
	}

	// Check for new players (joins)
	for (const auto& player : currentPlayers) {
		if (previousPlayers.find(player) == previousPlayers.end()) {
			std::string joinMessage = "[Melody] + " + player;
			notificationsMod->addNotifBox(joinMessage, 3.0f);
			mc.DisplayClientMessage("[%sMelody%s] %s+ %s", DARK_PURPLE, WHITE, GREEN, player.c_str());
		}
	}

	// Check for players who left
	for (const auto& player : previousPlayers) {
		if (currentPlayers.find(player) == currentPlayers.end()) {
			std::string leaveMessage = "[Melody] - " + player;
			notificationsMod->addNotifBox(leaveMessage, 3.0f);
			mc.DisplayClientMessage("[%sMelody%s] %s- %s", DARK_PURPLE, WHITE, RED, player.c_str());
		}
	}

	// Update previous players list
	previousPlayers = currentPlayers;
}
void Notifications::Render(ImDrawList* drawlist) {
	if (!client->isInitialized()) return;
	static Notifications* notifBox = (Notifications*)client->moduleMgr->getModule("Notifications");
	if (!notifBox->isEnabled()) {
		if (notifList.size() != 0) notifList.clear();
		return;
	}
	static const float textSize = 1.f;
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

		ImGuiUtils::fillRectangle(rectPos, UIColor(0, 0, 0, (int)(110 * idk)), 0.f);
		ImGuiUtils::fillRectangle(Vec4(rectPos.x, rectPos.w - 1.f * textSize, rectPos.z, rectPos.w), UIColor(0, 0, 0, (int)(75 * idk)), 0.f);
		if (notifBox->duration != 0.f) {
			Vec4 underline = Vec4(rectPos.x, rectPos.w - 1.f * textSize, rectPos.x + (rectPos.z - rectPos.x) * (notifBox->duration / notifBox->maxDuration), rectPos.w);
			ImGuiUtils::fillRectangle(underline, UIColor(255, 255, 255, (int)(255 * idk)), 0.f);
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