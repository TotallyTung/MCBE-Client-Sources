
struct RenderChunkCoordinatorProxyCallbacks { /* Size=0xc0 */
  /* 0x0000 */ std::function<unsigned __int64 __cdecl(void)> mGetRenderChunkSharedCount;
  /* 0x0040 */ std::function<mce::Color & __cdecl(void)> getFogColor;
  /* 0x0080 */ std::function<float __cdecl(void)> getFogDistance;
  
  RenderChunkCoordinatorProxyCallbacks(RenderChunkCoordinatorProxyCallbacks&);
  RenderChunkCoordinatorProxyCallbacks(const RenderChunkCoordinatorProxyCallbacks&);
  RenderChunkCoordinatorProxyCallbacks();
  ~RenderChunkCoordinatorProxyCallbacks();
  RenderChunkCoordinatorProxyCallbacks& operator=(RenderChunkCoordinatorProxyCallbacks&);
  RenderChunkCoordinatorProxyCallbacks& operator=(const RenderChunkCoordinatorProxyCallbacks&);
  void* __vecDelDtor(uint32_t);
};
