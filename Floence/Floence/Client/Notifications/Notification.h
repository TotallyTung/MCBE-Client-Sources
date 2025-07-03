#pragma once

#include "NotificationsSettings.h"

class Notifications {
public:
	static void renderNotifications() {
		static NotificationsManager noti;
		auto& notifications = noti.getList();
		float yOffset = Address::getClientInstance()->getGuiData()->getmcResolution().y - 40;
		const float textSize = 1.25f; //1.1f;

		for (auto& notif : notifications) {
			if (notif->title.back() == ':') {
				notif->title = notif->title.substr(0, notif->title.length() - 1);
			}
			float elapsedTime = notif->getElapsedTime();
			float duration = notif->duration;
			float textlen = ImRenderUtil::getTextWidth(&notif->message, textSize - 0.08) + 47;
			float titlelen = ImRenderUtil::getTextWidth(&notif->title, textSize) + 47;

			float thelen = textlen;

			if (titlelen >= textlen)
				thelen = titlelen;

			const float firstPos = Address::getClientInstance()->getGuiData()->getmcResolution().x - thelen - 8.f;
			const float lastPos = Address::getClientInstance()->getGuiData()->getmcResolution().x + 8.f;
			const float speed = 1.f;
			static float fade = 0;

			EasingUtil easing;
			easing.percentage = (elapsedTime > duration) ? 1.0f - ((elapsedTime - duration) / speed) : elapsedTime / speed;
			float posX = (easing.percentage < 1) ? Math::lerp(lastPos, firstPos, (elapsedTime > duration) ? easing.easeInExpo() : easing.easeOutElastic()) : ((elapsedTime > duration) ? lastPos : firstPos);
			fade = (easing.percentage < 1) ? Math::lerp(0, 1.f, (elapsedTime > duration) ? easing.easeInExpo() : easing.easeOutElastic()) : 1.f;

			if (elapsedTime > duration && easing.percentage <= 0) notif->isOpen = false;

			Vector4<float> rect(posX, yOffset - 25, posX + thelen, yOffset + 25);
			Vector4<float> rect2(rect.x, rect.y, rect.x + 14, rect.w);
			Vector4<float> rect3(posX, yOffset + 25, posX + thelen, yOffset + 24);

			ImRenderUtil::Blur(rect, 5, 0.f);
			ImRenderUtil::fillRectangle(rect3, UIColor(255, 255, 255), 1.f, 0.f);
			ImRenderUtil::fillShadowRectangle(rect3, UIColor(255, 255, 255), 50.f, 12.5f, ImDrawFlags_ShadowCutOutShapeBackground);
			//ImRenderUtil::fillRectangle(rect, UIColor(0, 0, 0), 0.25f, 0.f );
			ImRenderUtil::fillShadowRectangle(rect, UIColor(0, 0, 0), 5.f, 25.f, ImDrawFlags_ShadowCutOutShapeBackground);

			{
				if (Global::interfont == 0)
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
			}
			if (Global::interfont == 1) {
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
			}
			if (Global::interfont == 2) {
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[6]);
			}

			ImRenderUtil::drawText(Vector2<float>(posX + 12.5, yOffset - 12.5), &notif->message, UIColor(255, 255, 255), textSize - 0.08, fade, true);
			ImGui::PopFont();

			yOffset = Math::lerp(yOffset, yOffset - 40 - 16, (elapsedTime > duration) ? easing.easeInExpo() : easing.easeOutExpo());
		}
	}
};

