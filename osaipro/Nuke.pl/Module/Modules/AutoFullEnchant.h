#pragma once
#include "Module.h"
class AutoEnchant : public IModule {
public:
	void enchant(int id, int enchantmentslevel) {
		Utils::sendCommand("enchant %s %i %i", Game.getLocalPlayer()->getNameTag()->getText(), id, enchantmentslevel);
	};
	void enchant(std::string Name, int enchantmentslevel) {
		Utils::sendCommand("enchant %s %s %i", Game.getLocalPlayer()->getNameTag()->getText(), Name, enchantmentslevel);
	};
	AutoEnchant::AutoEnchant() : IModule(0, Category::PLAYER, "AutoEnchant") {
	}
	AutoEnchant::~AutoEnchant() {};
	void AllEnchant() {
		enchant(17, 3);
		enchant(26, 1);
		enchant(28, 1);
	}
	void ArmorEnchantMents() {
		enchant(0, 4);
		enchant(1, 4);
		enchant(3, 4);
		enchant(4, 4);
		enchant(5, 3);
		AllEnchant();
	}
	virtual void onTick(GameMode* gm) override {
		if (Game.getLocalPlayer() == nullptr) return;
		int itemID = Game.getLocalPlayer()->getSelectedItemId();
		if (itemID == 347) {
			ArmorEnchantMents();
			enchant(6, 1);
			enchant(6, 3);
		}
		else if (itemID == 348 || itemID == 349) {
			ArmorEnchantMents();
		}
		else if (itemID == 350) {
			ArmorEnchantMents();
			enchant(2, 4);
			enchant(37, 3);
			enchant(7, 3);
		}
		else if (itemID == 316) {
			AllEnchant();
			enchant(14, 3);
			enchant(18, 5);
			enchant(12, 2);
			enchant(13, 2);
			enchant(9, 5);
			enchant(10, 5);
			enchant(11, 5);
		}
		else if (itemID == 318) {
			AllEnchant();
			enchant(15, 5);
			enchant(18, 3);
		}
	};
	virtual const char* getModuleName() {
		return ("AutoEnchant");
	}

};