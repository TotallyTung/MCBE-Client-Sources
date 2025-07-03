
class mce::TexturePtr { /* Size=0x48 */
  /* 0x0000 */ std::shared_ptr<BedrockTextureData const > mClientTexture;
  /* 0x0010 */ ResourceLocation mResourceLocation;
  static const mce::TexturePtr NONE;
  static const BedrockTextureData mDefaultTextureData;
  
  TexturePtr(mce::TexturePtr&);
  TexturePtr(const mce::TexturePtr&);
  TexturePtr(mce::TextureGroupBase&, const ResourceLocation&);
  TexturePtr(const BedrockTexture&, const ResourceLocation&);
  TexturePtr();
  ~TexturePtr();
  mce::TexturePtr& operator=(mce::TexturePtr&);
  mce::TexturePtr& operator=(const mce::TexturePtr&);
  bool operator==(const mce::TexturePtr&) const;
  const mce::ClientTexture& operator*() const;
  bool operator bool() const;
  const mce::ClientTexture& getClientTexture() const;
  const mce::TextureDescription& getTextureDescription() const;
  const cg::ImageDescription& getImageDescription() const;
  IsMissingTexture isMissingTexture() const;
  TextureLoadState getTextureLoadState() const;
  const ResourceLocation& getResourceLocation() const;
  uint64_t hashCode() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
