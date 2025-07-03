
class BedrockTexture { /* Size=0x10 */
  /* 0x0000 */ std::shared_ptr<BedrockTextureData> mBedrockTextureData;
  
  BedrockTexture(BedrockTexture&);
  BedrockTexture(const BedrockTexture&);
  BedrockTexture();
  BedrockTexture& operator=(BedrockTexture&);
  BedrockTexture& operator=(const BedrockTexture&);
  ~BedrockTexture();
  const mce::ClientTexture& operator const struct mce::ClientTexture &() const;
  const mce::ClientTexture& getClientHandle() const;
  const std::shared_ptr<BedrockTextureData const > getSharedBedrockTextureData() const;
  const cg::ImageDescription& getImageDescription() const;
  const mce::TextureDescription& getTextureDescription() const;
  bool load(mce::TextureResourceService&, const gsl::not_null<std::shared_ptr<cg::TextureSetDefinition> >, const gsl::basic_string_span<char const ,-1>);
  bool load(mce::TextureResourceService&, const mce::TextureContainer&, IsMissingTexture, const gsl::basic_string_span<char const ,-1>);
  void loadMetadata(const cg::TextureSetImageDescription&, const std::optional<enum IsMissingTexture>);
  void loadMetadata(const gsl::not_null<std::shared_ptr<cg::TextureSetDefinition> >);
  void loadMetadata(const mce::TextureDescription&, const std::optional<enum IsMissingTexture>);
  void unload();
  IsMissingTexture isMissingTexture() const;
  TextureLoadState getTextureLoadState() const;
  cg::TextureSetImageDescription getTextureSetImageDescription() const;
  dragon::res::ServerTexture getDragonServerTexture() const;
  dragon::res::ClientTexture getDragonClientTexture() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
