
class mce::Texture : public mce::TextureNull, protected mce::ResourceBase<mce::TextureNull> { /* Size=0x98 */
  /* 0x0000: fields for mce::TextureNull */
  /* 0x0078: fields for mce::ResourceBase<mce::TextureNull> */
  /* 0x0078 */ std::shared_ptr<mce::DragonLifetime> mDragonTextureLifetime;
  /* 0x0088 */ std::optional<unsigned __int64> mRenderDragonTexture;
  
  Texture(int32_t, const mce::TextureDescription&);
  Texture(int32_t);
  Texture(mce::Texture&);
  Texture();
  ~Texture();
  mce::Texture& operator=(mce::Texture&);
  void deleteTexture();
  void safelyDeleteTexture();
  void createTexture(mce::RenderContext&, const mce::TextureDescription&);
  void safelyCreateTexture(mce::RenderContext&, const mce::TextureDescription&);
  void bindTexture(mce::CommandList&, const uint32_t, const uint32_t) const;
  void bindTexture(mce::RenderContext&, const uint32_t, const uint32_t) const;
  void unbindTexture(mce::RenderContext&, const uint32_t) const;
  void subBuffer(mce::RenderContext&, const void*, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t);
  void subBuffer(mce::RenderContext&, const void*);
  void subBuffer(mce::RenderContext&, const void*, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t);
  void copyTexture(mce::RenderContext&, mce::Texture*, const uint32_t, const uint32_t, const uint32_t, const uint32_t);
  void resolveTexture(mce::RenderContext&, mce::Texture*);
  void loadMipMap(mce::RenderContext&, const void*, const uint32_t);
  void copyTextureImage(mce::RenderContext&, mce::Texture*, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint32_t);
  bool isCreated() const;
  int32_t getInternalTexture() const;
  int32_t getShaderResource();
  const mce::TextureDescription& getDescription() const;
  bool ownsResource() const;
  void transitionToPixelShaderResourceState() const;
  void transitionToNonPixelShaderResourceState() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
