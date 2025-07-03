
class mce::RenderContextStateBase { /* Size=0x110 */
  /* 0x0000 */ mce::BlendStateDescription blendStateDescription;
  /* 0x0008 */ mce::DepthStencilStateDescription depthStencilStateDescription;
  /* 0x0020 */ mce::RasterizerStateDescription rasterizerStateDescription;
  /* 0x002c */ mce::SamplerStateDescription samplerStateDescription[8];
  /* 0x008c */ bool textureUnitActive[8];
  /* 0x0094 */ glm::tvec4<int,0> currentScissor;
  /* 0x00a4 */ mce::ViewportInfo currentViewport;
  /* 0x00bc */ bool initializedSamplerState[8];
  /* 0x00c8 */ std::array<mce::TextureBase const *,8> lastBoundTexture;
  /* 0x0108 */ mce::RenderTargetState* renderTargetState;
  
  RenderContextStateBase();
  void removeIfBound(const mce::TextureBase&);
  ~RenderContextStateBase();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
