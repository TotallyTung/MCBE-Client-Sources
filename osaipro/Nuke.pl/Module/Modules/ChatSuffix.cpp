#include "ChatSuffix.h"
#include "../../../Memory/Hooks.h"
bool toint = true;
ChatSuffix::ChatSuffix() : IModule(0, Category::MISC, "Adds the client suffix") {
	registerEnumSetting("Suffix", &suffixchanger, 4);
	registerEnumSetting("Bypass", &bypass, 1);
	registerEnumSetting("Color", &ColorChanger, 1);
	registerEnumSetting("Font", &fontchanger, 0);
	fontchanger
		.addEntry("default", 0)
		.addEntry("Circular", 1)
		.addEntry("All Caps", 2)
		.addEntry("Landing", 3)
		.addEntry("Black Pill", 4)
		.addEntry("Bold Text", 5)
		.addEntry("Bubble Text", 6)
		.addEntry("Funky Text", 7)
		.addEntry("Random Font", 8);
	suffixchanger.addEntry("None", 0);
	suffixchanger.addEntry("OsaiPRO", 1);
	suffixchanger.addEntry("osai.pro", 2);
	suffixchanger.addEntry("Nuke.jp", 3);
	suffixchanger.addEntry("nuke.japan", 4);
	suffixchanger.addEntry("Victory+", 5);
	suffixchanger.addEntry("TeamFrost", 6);
	bypass
		.addEntry("None", 0)
		.addEntry("Back", 1)
		.addEntry("Rear", 2)
		.addEntry("Both", 3);
	ColorChanger
		.addEntry("None", 0)
		.addEntry("Green 2b2e", 1);
	registerIntSetting("Bypass Length", &bypasslength, bypasslength, 1, 40);
	registerBoolSetting("toInterger", &toint, false);
	registerBoolSetting("Reset", &reset, reset);
}
std::string Sentence;
std::string release;
std::string Suffix;
ChatSuffix::~ChatSuffix() {
}

const char* ChatSuffix::getModuleName() {
	return "Chat+";
}

void ChatSuffix::onTick(GameMode* gm) {
	if (reset) {
		ColorChanger.selected = 1;
		fontchanger.selected = 0;
		bypass.selected = 1;
		suffix = true;
		bypasslength = 5;
		reset = false;
	}
}

void ChatSuffix::onSendPacket(Packet* packet) {
	if (Game.getLocalPlayer() != nullptr) {
		if (packet->isInstanceOf<C_TextPacket>()) {
			C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
			std::string start;
			std::string end;
			switch (suffixchanger.getSelectedValue()) {
			case 1:
				Suffix = "OsaiPRO";
				break;
			case 2:
				Suffix = "osai.pro";
				break;
			case 3:
				Suffix = "Nuke.jp";
				break;
			case 4:
				Suffix = "nuke.japan";
				break;
			case 5:
				Suffix = "Victory+ #Team Kaeru";
				break;
			case 6:
				Suffix = "TeamFrost";
				break;
			}
			if (Game.getLocalPlayer()->getNameTag()->getText() == "SakuraUwU3916")
				Suffix += " | Developer";
			if (bypass.getSelectedValue() == 1 || bypass.getSelectedValue() == 3)  start += " | ";
			if (suffixchanger.getSelectedValue() != 0) end += " | " + Suffix;
			if ((bypass.getSelectedValue() == 2 || bypass.getSelectedValue() == 3)) end += " | ";
			Sentence = start + funy->message.getText() + end;
			bool RandomFonts = false;
			if (fontchanger.getSelectedValue() == 8) {
				fontchanger.selected = rand() % (7 - 1 + 1) + 1;
				RandomFonts = true;
			}
			if (toint) Sentence = FontChange::integer(Sentence);
			switch (fontchanger.getSelectedValue()) {
			case 1:
				Sentence = FontChange::circle(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			case 2:
				Sentence = FontChange::allcaps(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			case 3:
				Sentence = FontChange::landing(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			case 4:
				Sentence = FontChange::BlackPill(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			case 5:
				Sentence = FontChange::Bold(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			case 6:
				Sentence = FontChange::Bubble(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			case 7:
				Sentence = FontChange::Funky(Sentence);
				if (RandomFonts) fontchanger.selected = 8;
				break;
			};
			if (bypass.getSelectedValue() == 1 || bypass.getSelectedValue() == 3) {
				Sentence = Utils::randomString(bypasslength) + Sentence;
			}
			if (bypass.getSelectedValue() == 2 || bypass.getSelectedValue() == 3) {
				Sentence = Sentence + Utils::randomString(bypasslength);
			}
			if (ColorChanger.getSelectedValue() == 1) Sentence = u8"§a> " + Sentence;
			funy->message.resetWithoutDelete();
			funy->message = Sentence;
		}
	}
}