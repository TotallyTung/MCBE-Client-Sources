#include "Module.h"
#include <algorithm>

bool* truePtr = new bool(true); // LOL lfmao this so stupid

SettingEntry::~SettingEntry() {
	delete this->defaultValue;
	delete this->minValue;
	delete this->maxValue;
	delete this->shouldShow;
}

void SettingEntry::makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt() {
	switch (valueType) {
	case ValueType::BOOL_T:
		break;
	case ValueType::INT64_T:
		value->int64 = std::max(minValue->int64, std::min(maxValue->int64, value->int64));
		break;
	case ValueType::DOUBLE_T:
		value->_double = std::max(minValue->_double, std::min(maxValue->_double, value->_double));
		break;
	case ValueType::FLOAT_T:
		value->_float = std::max(minValue->_float, std::min(maxValue->_float, value->_float));
		break;
	case ValueType::INT_T:
		value->_int = std::max(minValue->_int, std::min(maxValue->_int, value->_int));
		break;
	case ValueType::TEXT_T:
		//break;
	default:
		//logF("unrecognized value %i", valueType);
		break;
	}
}

Module::Module(int k, Category c, const char* d) {
	this->keybind = k;
	this->category = c;
	this->description = d;
	registerBoolSetting("Visible", &this->visible, this->visible, "Visible on Arraylist");
	registerKeybindSetting("Keybind", &this->keybind, this->keybind);
	this->toggleMod.addEntry("Normal", 0);
	this->toggleMod.addEntry("Hold", 1);
	registerEnumSetting("Toggle", &this->toggleMod, 0);
}

EnumEntry::EnumEntry(const char* _name, const unsigned char value) {
	name = _name;
	val = value;
}

const char* EnumEntry::GetName() {
	return name;
}

unsigned char EnumEntry::GetValue() {
	return val;
}

SettingEnum::SettingEnum(std::vector<EnumEntry> entr, Module* mod) {
	Entrys = entr;
	owner = mod;
	std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
		return rhs.GetValue() < lhs.GetValue();
		});
}

SettingEnum::SettingEnum(Module* mod) {
	owner = mod;
}

SettingEnum& SettingEnum::addEntry(EnumEntry entr) {
	auto etr = EnumEntry(entr);
	bool SameVal = false;
	for (auto it = this->Entrys.begin(); it != this->Entrys.end(); it++) {
		SameVal |= it->GetValue() == etr.GetValue();
	}
	if (!SameVal) {
		Entrys.push_back(etr);
		std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
			return rhs.GetValue() < lhs.GetValue();
			});
	}
	return *this;
}

SettingEnum& SettingEnum::addEntry(const char* name, int value) {
	auto etr = EnumEntry(name, value);
	bool SameVal = false;
	for (EnumEntry it : Entrys)
		SameVal |= it.GetValue() == etr.GetValue();

	if (!SameVal) {
		Entrys.push_back(etr);
		sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
			return rhs.GetValue() < lhs.GetValue();
			});
	}
	return *this;
}

EnumEntry& SettingEnum::GetEntry(int ind) {
	return Entrys.at(ind);
}

EnumEntry& SettingEnum::GetSelectedEntry() {
	return GetEntry(selected);
}

int SettingEnum::getSelectedValue() {
	return GetEntry(selected).GetValue();
}

int SettingEnum::GetCount() {
	return (int)Entrys.size();
}

int SettingEnum::SelectNextValue(bool back) {
	if (selected == 0 && back) {
		selected = (int)Entrys.size() - 1;
		//return selected;
	}
	else {

		if (back) {
			selected--;
		}
		else {
			selected++;
		}

		if (Entrys.size() <= selected) {
			selected = 0;
		}
	}
	owner->onModifySettingEnum(this);
	return selected;

}

void Module::registerBoolSetting(const char* name, bool* boolPtr, bool defaultValue, const char* description, bool* shouldShow) {
	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::BOOL_T;
	
	setting->value = reinterpret_cast<SettingValue*>(boolPtr);  // Actual value

	SettingValue* defaultVal = new SettingValue();  // Default Value
	defaultVal->_bool = defaultValue;
	setting->defaultValue = defaultVal;

	setting->name = name;    //strcpy_s(setting->name, 19, name);  // Name
	setting->description = description;

	if (shouldShow == nullptr) setting->shouldShow = truePtr;  // shouldShow
	else setting->shouldShow = shouldShow;  // shouldShow

	settings.push_back(setting);  // Add to list
}

void Module::registerIntSetting(const char* name, int* intPtr, int defaultValue, int minValue, int maxValue, const char* description, bool* shouldShow) {

	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::INT_T;
	setting->value = reinterpret_cast<SettingValue*>(intPtr);  // Actual Value

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value
	SettingValue* minVal = new SettingValue();
	minVal->_int = minValue;
	setting->minValue = minVal;

	// Max Value
	SettingValue* maxVal = new SettingValue();
	maxVal->_int = maxValue;
	setting->maxValue = maxVal;

	// Name
	setting->name = name;    //strcpy_s(setting->name, 19, name);  // Name
	setting->description = description;
	if (shouldShow == nullptr) setting->shouldShow = truePtr;  // shouldShow
	else setting->shouldShow = shouldShow;  // shouldShow

	settings.push_back(setting);  // Add to list
}

void Module::registerFloatSetting(const char* name, float* floatPtr, float defaultValue, float minValue, float maxValue, const char* description, bool* shouldShow) {

	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::FLOAT_T;

	setting->value = reinterpret_cast<SettingValue*>(floatPtr);

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_float = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value
	SettingValue* minVal = new SettingValue();
	minVal->_float = minValue;
	setting->minValue = minVal;

	// Max Value
	SettingValue* maxVal = new SettingValue();
	maxVal->_float = maxValue;
	setting->maxValue = maxVal;

	// Name
	setting->name = name;    //strcpy_s(setting->name, 19, name);  // Name
	setting->description = description;
	if (shouldShow == nullptr) setting->shouldShow = truePtr;  // shouldShow
	else setting->shouldShow = shouldShow;  // shouldShow

	settings.push_back(setting);  // Add to list
}

void Module::registerEnumSetting(const char* name, SettingEnum* ptr, int defaultValue, const char* description, bool* shouldShow) {
	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::ENUM_T;
	if (defaultValue < 0 || defaultValue >= ptr->GetCount())
		defaultValue = 0;

	// Actual Value
	setting->value = reinterpret_cast<SettingValue*>(&ptr->selected);
	setting->value->_int = defaultValue;

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value (is Extended)
	SettingValue* minVal = new SettingValue();
	minVal->_bool = false;
	setting->minValue = minVal;

	// Enum data
	setting->extraData = ptr;

	// Name
	setting->name = name;    //strcpy_s(setting->name, 19, name);  // Name
	setting->description = description;
	if (shouldShow == nullptr) setting->shouldShow = truePtr;  // shouldShow
	else setting->shouldShow = shouldShow;  // shouldShow

	settings.push_back(setting);
}

void Module::registerKeybindSetting(const char* name, int* intPtr, int defaultValue, const char* description, bool* shouldShow) {

	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::KEYBINT_T;
	setting->value = reinterpret_cast<SettingValue*>(intPtr);  // Actual Value

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	// Name
	setting->name = name;    //strcpy_s(setting->name, 19, name);  // Name
	setting->description = description;
	if (shouldShow == nullptr) setting->shouldShow = truePtr;  // shouldShow
	else setting->shouldShow = shouldShow;  // shouldShow

	settings.push_back(setting);  // Add to list
}

Module::~Module() {
	for (SettingEntry* setting : this->settings) {
		delete setting;
	}
}

const char* Module::getModuleName() {
	return nullptr;
}

const char* Module::getModName() {
	return nullptr;
}

int Module::getKeybind() {
	return this->keybind;
}

bool Module::isEnabled() {
	return this->enabled;
}

bool Module::isVisible() {
	return this->visible;
}

bool Module::isHoldMode() {
	//return false;
	return this->toggleMod.getSelectedValue();
}

void Module::setEnabled(const bool& enabled) {
	if (this->enabled != enabled) {
		this->enabled = enabled;

		if (enabled) this->onEnable();
		else this->onDisable();
	}
}

void Module::setKeybind(const int& newKey) {
	this->keybind = newKey;
}

void Module::onEnable() {
}

void Module::onDisable() {
}

void Module::toggle() {
	setEnabled(!this->enabled);
}

void Module::onModifySettingEnum(SettingEnum* se) {
}

void Module::onKeyUpdate(const int& key, const bool& isDown) {
	if (this->keybind == key) {
		if (isHoldMode()) setEnabled(isDown);
		else if (isDown) toggle();
	}
}

void Module::onImGuiRender(ImDrawList* d) {
}

void Module::onImGui3DRender(ImDrawList* d, glmatrixf* refdef) {
}