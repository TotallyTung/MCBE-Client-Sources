
class mce::RenderContext : public mce::RenderContextNull, protected mce::ResourceBase<mce::RenderContextNull> { /* Size=0x1c0 */
  /* 0x0000: fields for mce::RenderContextNull */
  /* 0x01b8: fields for mce::ResourceBase<mce::RenderContextNull> */
  /* 0x01b8 */ bool mWithinFrame;
  
  RenderContext();
  ~RenderContext();
  mce::RenderDevice* getDevice() const;
  void createWindowSizeDependentResources();
  void clearContextState();
  void lostContext();
  void draw(const mce::PrimitiveMode, const uint32_t, const uint32_t);
  void drawIndexed(const mce::PrimitiveMode, const uint32_t, const uint32_t);
  void drawInstanced(const mce::PrimitiveMode, const uint32_t, const uint32_t, const uint32_t);
  void drawIndexedInstanced(const mce::PrimitiveMode, const uint32_t, const uint32_t, const uint32_t);
  void setViewport(const mce::ViewportInfo&);
  void setScissorRect(const int32_t, const int32_t, const uint32_t, const uint32_t);
  void setViewportWithFullScissor(const mce::ViewportInfo&);
  void clearColorBuffer(const mce::Color&, const mce::ViewportInfo*);
  void clearDepthBuffer(const float, const mce::ViewportInfo*);
  void clearStencilBuffer(const unsigned char, const mce::ViewportInfo*);
  void clearDepthStencilBuffer(const float, const unsigned char, const mce::ViewportInfo*);
  void beginFrame();
  void endFrame();
  bool isWithinFrame();
  void unbindAllTexturesAndShaders();
  void flush();
  void swapBuffers();
  void discardBackbuffer();
  bool captureScreenAsRGB(std::vector<unsigned char,std::allocator<unsigned char> >&, int32_t&, int32_t&);
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
