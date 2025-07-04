
class dragon::res::ResolvedTextureResource { /* Size=0xb0 */
  /* 0x0000 */ bgfx::TextureHandle mTextureHandle;
  /* 0x0008 */ dragon::res::TextureDescription mTextureDescription;
  /* 0x0078 */ std::optional<cg::TextureMetaData> mTextureMetaData;
  /* 0x00a0 */ mpmc::Sender<std::function<void __cdecl(void)> > mDestructionQueue;
  
  ResolvedTextureResource(dragon::res::ResolvedTextureResource&);
  ResolvedTextureResource(const dragon::res::ResolvedTextureResource&);
  ResolvedTextureResource();
  dragon::res::ResolvedTextureResource& operator=(dragon::res::ResolvedTextureResource&);
  dragon::res::ResolvedTextureResource& operator=(const dragon::res::ResolvedTextureResource&);
  ~ResolvedTextureResource();
  dragon::result::Result<dragon::res::ResolvedTextureResource &,enum dragon::res::TextureError> createTexture(const mpmc::Sender<std::function<void __cdecl(void)> >&, const dragon::res::TextureDescription&, const bgfx::Memory*);
  dragon::result::Result<dragon::res::ResolvedTextureResource &,enum dragon::res::TextureError> updateTexture(const bgfx::Memory*);
  dragon::result::Result<dragon::res::ResolvedTextureResource &,enum dragon::res::TextureError> updateTextureRegion(uint32_t, uint32_t, uint32_t, uint32_t, const bgfx::Memory*);
  dragon::result::Result<dragon::res::TextureRead,enum dragon::res::TextureError> readTexture(unsigned char) const;
  dragon::result::Result<void,enum dragon::res::TextureError> copyTexture(dragon::res::ResolvedTextureResource&) const;
  void createWrappedTexture(const mpmc::Sender<std::function<void __cdecl(void)> >&, bgfx::TextureHandle, dragon::res::TextureDescription);
  bgfx::TextureHandle getHandle() const;
  const dragon::res::TextureDescription& getDescription() const;
  const std::optional<cg::TextureMetaData>& getMetaData() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
