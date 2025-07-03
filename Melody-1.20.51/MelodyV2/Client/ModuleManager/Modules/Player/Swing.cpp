// Created by Tony on 2024-10-10 10:01:54

#include "Swing.h"

Swing::Swing() : Module("Swing", "Modify your swings speed.", Category::PLAYER) {
	addSlider<int>("Speed", "NULL", ValueType::INT_T, &swingSpeed, 1, 20);
}