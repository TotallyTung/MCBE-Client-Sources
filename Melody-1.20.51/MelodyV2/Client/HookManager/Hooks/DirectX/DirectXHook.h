// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../Libs/ImGui/imgui.h"
#include "../../../../Libs/Kiero/kiero.h"
#include <dxgi1_4.h>

class DirectXHook {
protected:
	static HRESULT presentCallback(IDXGISwapChain3* pSwapChain, UINT syncInterval, UINT flags);
	using present_t = HRESULT(__thiscall*)(IDXGISwapChain3*, UINT, UINT);
	static inline present_t oPresent;

	static HRESULT resizeBuffersHook(IDXGISwapChain* ppSwapChain, int bufferCount, int width, int height, DXGI_FORMAT newFormat, int swapChainFlags);
	using resizeBuffers_t = HRESULT(__thiscall*)(IDXGISwapChain*, int, int, int, DXGI_FORMAT, int);
	static inline resizeBuffers_t oResizeBuffers;

	static __int64 gameBgfxCallback(__int64 a1, __int64 a2, __int64 a3, __int64 a4);
	using gameBgfxFunc_t = __int64(_fastcall*)(__int64, __int64, __int64, __int64);
	static inline gameBgfxFunc_t gameBgfxFunc;

	static void Render(ImDrawList* drawlist);
private:
	static inline bool initImgui = false;
public:
	static inline IDXGISwapChain3* pSwapChainRestore = nullptr;
	static void init();
};
