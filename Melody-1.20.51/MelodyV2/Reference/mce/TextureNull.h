
class mce::TextureNull : public mce::TextureBase { /* Size=0x78 */
  /* 0x0000: fields for mce::TextureBase */
  
  TextureNull(int32_t, const mce::TextureDescription&);
  TextureNull(int32_t);
  TextureNull(mce::TextureNull&);
  TextureNull(const mce::TextureNull&);
  TextureNull();
  mce::TextureNull& operator=(mce::TextureNull&);
  mce::TextureNull& operator=(const mce::TextureNull&);
  void createTexture(mce::RenderContext&, const mce::TextureDescription&);
  int32_t getInternalTexture() const;
  int32_t getShaderResource();
  ~TextureNull();
  void* __vecDelDtor(uint32_t);
};
