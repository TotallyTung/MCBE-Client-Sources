#include "Hook.h"
#include "../Manager/Manager.h"
#include <Windows.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>

Hook::Hook(Manager* m) {
	this->mgr = m;
	this->mgr->hooks.push_back(this);
};
// FULLY UPDATED TO 1.20.15
