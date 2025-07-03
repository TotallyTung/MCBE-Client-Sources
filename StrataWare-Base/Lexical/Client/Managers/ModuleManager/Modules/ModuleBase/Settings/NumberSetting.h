#pragma once
#include "Setting.h"
#include <type_traits>

enum class ValueType {
	INT_T,
	FLOAT_T,
	UNKNOW_T
};

class SliderSettingBase : public Setting {
public:
	ValueType valueType;
};

template<typename T>
class NumberSetting : public SliderSettingBase {
public:
	T* valuePtr;
	T minValue;
	T maxValue;

	NumberSetting(std::string settingName, std::string des, T* vPtr, T defaultValue, T vMin, T vMax) {
		this->name = settingName;
		this->description = des;
		this->valuePtr = vPtr;
		*this->valuePtr = defaultValue;
		this->minValue = vMin;
		this->maxValue = vMax;

		this->type = SettingType::SLIDER_S;

		if (std::is_same<T, int>::value) {
			this->valueType = ValueType::INT_T;
		}
		else if (std::is_same<T, float>::value) {
			this->valueType = ValueType::FLOAT_T;
		}
	}
};