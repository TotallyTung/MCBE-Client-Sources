#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../SDK/Classes/BaseActorRenderContext.h"

#include "../../../Client.h"

class GlintColorHook {
protected:
	using func_t = void(__fastcall*)(ScreenContext*, BaseActorRenderContext*, Actor*, MC_Color*, MC_Color*, MC_Color*, MC_Color*, float, float, float, float, BaseActorRenderContext**, MC_Color*, float, unsigned __int8*);
	static inline func_t func;
	//__int64 screenContext, __int64 entityContext, __int64 entity, __int64 overlay, __int64 changeColor, __int64 changeColor2, __int64 glintColor, int uvOffset1, int uvOffset2, float uvRot1, float uvRot2, _QWORD *glintUVScale, __int128 *uvAnim, float br, char *lightEmission
	static void GlintColorFunc(ScreenContext* screenContext,
        BaseActorRenderContext* entityContext,
        Actor* entity,
        MC_Color* overlay,
        MC_Color* changeColor,
        MC_Color* changeColor2,
        MC_Color* glintColor,
        float uvOffset1,
        float uvOffset2,
        float uvRot1,
        float uvRot2,
        BaseActorRenderContext** glintUVScale,
        MC_Color* uvAnim,
        float br,
        unsigned __int8* lightEmission) {
        //starts

        static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");
        glintColor->b = (float)0.2f;
        return func(screenContext, entityContext, entity, overlay, changeColor, changeColor2, glintColor, uvOffset1, uvOffset2, uvRot1, uvRot2, glintUVScale, uvAnim, br, lightEmission);

	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::glintscolorhook);
		MemoryUtils::CreateHook("GlintColorHook", address, (void*)&GlintColorHook::GlintColorFunc, (void*)&func);
	}
};
