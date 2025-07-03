
struct mce::ClientTexture : public mce::ClientResourcePointer<mce::ResourcePointer<std::variant<std::monostate,mce::Texture,dragon::res::ClientTexture>,mce::ResourceBlockTemplate<std::variant<std::monostate,mce::Texture,dragon::res::ClientTexture>,mce::PerFrameHandleTracker,mce::ResourceServiceTextureDescription>,std::shared_ptr> > { /* Size=0x10 */
  /* 0x0000: fields for mce::ClientResourcePointer<mce::ResourcePointer<std::variant<std::monostate,mce::Texture,dragon::res::ClientTexture>,mce::ResourceBlockTemplate<std::variant<std::monostate,mce::Texture,dragon::res::ClientTexture>,mce::PerFrameHandleTracker,mce::ResourceServiceTextureDescription>,std::shared_ptr> > */
  
  ClientTexture();
  ClientTexture(mce::ClientTexture&);
  ClientTexture(const mce::ClientTexture&);
  ClientTexture(mce::ClientResourcePointer<mce::ResourcePointer<std::variant<std::monostate,mce::Texture,dragon::res::ClientTexture>,mce::ResourceBlockTemplate<std::variant<std::monostate,mce::Texture,dragon::res::ClientTexture>,mce::PerFrameHandleTracker,mce::ResourceServiceTextureDescription>,std::shared_ptr> >&);
  ~ClientTexture();
  mce::ClientTexture& operator=(mce::ClientTexture&);
  mce::ClientTexture& operator=(const mce::ClientTexture&);
  void* __vecDelDtor(uint32_t);
};
