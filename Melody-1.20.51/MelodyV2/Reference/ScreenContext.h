
class ScreenContext : public UIScreenContext, public mce::MeshContext { /* Size=0xe0 */
  /* 0x0000: fields for UIScreenContext */
  /* 0x0010: fields for mce::MeshContext */
  /* 0x0080 */ mce::RenderDevice& renderDevice;
  /* 0x0088 */ mce::RendererSettings& rendererSettings;
  /* 0x0090 */ mce::FrameBufferObject* frameBufferObject;
  /* 0x0098 */ const mce::ViewportInfo& viewport;
  /* 0x00a0 */ const GuiData& guiData;
  /* 0x00a8 */ const mce::Clock& clock;
  /* 0x00b0 */ Tessellator& tessellator;
  /* 0x00b8 */ MinecraftGraphicsPipeline& minecraftGraphicsPipeline;
  /* 0x00c0 */ MinecraftGraphics& minecraftGraphics;
  /* 0x00c8 */ const UIProfanityContext& uiProfanityContext;
  /* 0x00d0 */ CommandListQueue& commandListQueue;
  /* 0x00d8 */ LinearAllocator<FrameRenderObject>& frameAllocator;
  
  ScreenContext(mce::RenderDevice&, mce::RendererSettings&, const mce::ViewportInfo&, const GuiData&, const mce::Clock&, Tessellator&, MinecraftGraphicsPipeline&, MinecraftGraphics&, const UIProfanityContext&, CommandListQueue&, LinearAllocator<FrameRenderObject>&, mce::MeshContext&, const int32_t, const int32_t, const float);
  ScreenContext(ScreenContext&);
  ScreenContext(const ScreenContext&);
  ScreenContext& operator=(ScreenContext&);
  ScreenContext& operator=(const ScreenContext&);
  ~ScreenContext();
  MinecraftGameplayGraphicsResources& getMinecraftGameplayGraphicsResources();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
