
class MinecraftGraphics : public AppPlatformListener { /* Size=0x180 */
  /* 0x0000: fields for AppPlatformListener */
  /* 0x0010 */ std::unique_ptr<mce::ShaderGroup,std::default_delete<mce::ShaderGroup> > mShaderGroup;
  /* 0x0018 */ std::unique_ptr<mce::QuadIndexBuffer,std::default_delete<mce::QuadIndexBuffer> > mQuadBuffer;
  /* 0x0020 */ mce::ClientResourcePointer<mce::ResourcePointer<mce::ImmediateBuffer,mce::ResourceBlockTemplate<mce::ImmediateBuffer,mce::PerFrameHandleTracker,dragon::BufferDescription>,std::shared_ptr> > mImmediateBuffer;
  /* 0x0030 */ MinecraftGameplayGraphicsResources mMinecraftGameplayGraphicsResources;
  
  MinecraftGraphics(const MinecraftGraphics&);
  MinecraftGraphics(mce::RenderContext&);
  virtual ~MinecraftGraphics();
  MinecraftGraphics& operator=(const MinecraftGraphics&);
  virtual void onAppSuspended();
  virtual void onAppTerminated();
  void frameUpdate(FrameUpdateContext&);
  mce::QuadIndexBuffer& getGlobalQuadBuffer();
  mce::ClientResourcePointer<mce::ResourcePointer<mce::ImmediateBuffer,mce::ResourceBlockTemplate<mce::ImmediateBuffer,mce::PerFrameHandleTracker,dragon::BufferDescription>,std::shared_ptr> >& getImmediateBuffer();
  MinecraftGameplayGraphicsResources& getMinecraftGameplayGraphicsResources();
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
