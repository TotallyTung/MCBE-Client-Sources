#pragma once

namespace bgfx {
	struct d3d12
	{
		struct BufferHeap {};
		struct BufferHeapBlock {};
		struct BufferD3D12 {};
		struct ShaderD3D12 {};
		struct TextureD3D12 {};
		struct TimerQueryD3D12 {};
		struct CommandContextD3D12 {};
		struct FrameBufferD3D12 {};
		struct BatchD3D12 {};

		struct RenderContextD3D12 : bgfx::RendererContextI
		{
			ID3D12CommandQueue* getCommandQueue() {
				return hat::member_at<ID3D12CommandQueue*>(this, OffsetManager::bgfx_d3d12_RendererContextD3D12_m_commandQueue);
			}
		};
	};
}