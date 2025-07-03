
class mce::TextureBase { /* Size=0x78 */
  /* 0x0000 */ mce::TextureDescription textureDescription;
  /* 0x0070 */ bool created;
  /* 0x0071 */ bool mOwnsResource;
  
  const mce::TextureDescription& getDescription() const;
  TextureBase(const mce::TextureDescription&);
  TextureBase(mce::TextureBase&);
  TextureBase(const mce::TextureBase&);
  TextureBase();
  mce::TextureBase& operator=(mce::TextureBase&);
  mce::TextureBase& operator=(const mce::TextureBase&);
  ~TextureBase();
  void deleteTexture();
  void createTexture(mce::RenderContext&, const mce::TextureDescription&);
  void bindTexture(mce::CommandList&, const uint32_t, const uint32_t) const;
  void bindTexture(mce::RenderContext&, const uint32_t, const uint32_t) const;
  void unbindTexture(mce::RenderContext&, const uint32_t) const;
  void subBuffer(mce::RenderContext&, const void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  void subBuffer(mce::RenderContext&, const void*);
  void subBuffer(mce::RenderContext&, const void*, uint32_t, uint32_t, uint32_t, uint32_t, const uint32_t);
  void copyTexture(mce::RenderContext&, mce::Texture*, uint32_t, uint32_t, uint32_t, uint32_t);
  void resolveTexture(mce::RenderContext&, mce::Texture*);
  void createMipStorage(mce::RenderContext&, const void*, const uint32_t, const uint32_t, const uint32_t);
  void copyTextureImage(mce::RenderContext&, mce::Texture*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  void convertToMipmapedTexture(const uint32_t);
  bool isCreated() const;
  bool ownsResource() const;
  void transitionToPixelShaderResourceState() const;
  void transitionToNonPixelShaderResourceState() const;
  bool isCompressedTexture();
  void* __vecDelDtor(uint32_t);
  
  static void _moveBase(mce::TextureBase&, mce::TextureBase&);
};
