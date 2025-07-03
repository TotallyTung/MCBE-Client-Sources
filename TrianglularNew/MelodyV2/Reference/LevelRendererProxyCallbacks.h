
struct LevelRendererProxyCallbacks { /* Size=0xc0 */
  /* 0x0000 */ std::function<RenderChunkCoordinatorProxy * __cdecl(AutomaticID<Dimension,int>)> mGetRenderChunkCoordinator;
  /* 0x0040 */ std::function<bool __cdecl(void)> isBuildingRenderChunks;
  /* 0x0080 */ std::function<std::shared_ptr<ClientBlockPipeline::SchematicsRepository> __cdecl(void)> getSchematicsRepository;
  
  LevelRendererProxyCallbacks(LevelRendererProxyCallbacks&);
  LevelRendererProxyCallbacks(const LevelRendererProxyCallbacks&);
  LevelRendererProxyCallbacks();
  ~LevelRendererProxyCallbacks();
  LevelRendererProxyCallbacks& operator=(LevelRendererProxyCallbacks&);
  LevelRendererProxyCallbacks& operator=(const LevelRendererProxyCallbacks&);
  void* __vecDelDtor(uint32_t);
};
