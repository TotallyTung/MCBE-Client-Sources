
class LevelRendererProxy { /* Size=0xc0 */
  /* 0x0000 */ LevelRendererProxyCallbacks mCallbacks;
  
  LevelRendererProxy(const LevelRendererProxy&);
  LevelRendererProxy(const LevelRendererProxyCallbacks&);
  ~LevelRendererProxy();
  RenderChunkCoordinatorProxy* getRenderChunkCoordinator(AutomaticID<Dimension,int>) const;
  bool isBuildingRenderChunks() const;
  std::shared_ptr<ClientBlockPipeline::SchematicsRepository> getSchematicsRepository();
  LevelRendererProxy& operator=(const LevelRendererProxy&);
  void* __vecDelDtor(uint32_t);
};
