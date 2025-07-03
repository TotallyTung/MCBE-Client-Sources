#pragma once

void* __o__keypress;
void* __o__SendMouseEvent;

void KeypressDetour(int key, bool held) {
	Game::Keymap[key] = held;

	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('L') & 0x8000)) Game::isRunning = false;

	bool cancelled = false;

	KeyboardEvent event{ &key, &held };
	event.cancelled = &cancelled;
	DispatchEvent(&event);

	for (auto module : modules)

	for (auto module : modules)
	{
		if (held == true)
		{
			if (module->getKeybind() == key)
			{
				if (module->ingameOnly)
				{
					if (Game::GetInstance() != nullptr && Game::GetInstance()->minecraftGame->canMoveKey)
					{
						module->toggle();
					}
				}
				else module->toggle();
			}
		}
	}

	if (!cancelled)
	{
		CallFunc<void*, int, bool>(
			__o__keypress,
			key,
			held
		);
	}
}

void MouseDetour(
	void* _this,
	__int8 mouseButton,
	bool isDown,
	__int16 mouseX,
	__int16 mouseY,
	__int16 relX,
	__int16 relY,
	int a8
)
{
	if (ImGui::GetCurrentContext() != nullptr)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (mouseButton == 0)
			io.MousePos = ImVec2(mouseX, mouseY);
		switch (mouseButton)
		{
		case 1:
			io.MouseDown[0] = isDown;
			break;
		case 2:
			io.MouseDown[1] = isDown;
			break;
		case 3:
			io.MouseDown[2] = isDown;
			break;
		case 4:
			io.MouseWheel = isDown > 130 ? -.4f : .4f; //For scrolling
			break;
		}

		if (io.WantCaptureMouse)
			return;
	}

	CallFunc<void*, void*, __int8, bool, __int16, __int16, __int16, __int16, char>(
		__o__SendMouseEvent,
		_this,
		mouseButton,
		isDown,
		mouseX,
		mouseY,
		relX,
		relY,
		a8
	);
}


class KeymapHook : public FuncHook {
public:
	bool Initialize() override {
		uintptr_t keymapAddr = findSig(xorstr_("48 83 EC 48 ? ? C1 4C 8D"), "KeyMap");
		uintptr_t mousemapAddr = findSig(xorstr_("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24"), "MouseMap");

		if (not HookFunction((void*)keymapAddr, &KeypressDetour, &__o__keypress, xorstr_("Keymap")))
			return false;

		if (not HookFunction((void*)mousemapAddr, &MouseDetour, &__o__SendMouseEvent, xorstr_("Mousemap")))
			return false;

		return true;
	}

	static KeymapHook& Get() {
		static KeymapHook single;
		return single;
	}
};