
struct dragon::res::ClientTexture : public mce::ClientResourcePointer<mce::ResourcePointer<dragon::res::ResolvedTextureResource,mce::ResourceBlockTemplate<dragon::res::ResolvedTextureResource,mce::PerFrameHandleTracker,dragon::res::TextureDescription>,std::shared_ptr> > { /* Size=0x10 */
  /* 0x0000: fields for mce::ClientResourcePointer<mce::ResourcePointer<dragon::res::ResolvedTextureResource,mce::ResourceBlockTemplate<dragon::res::ResolvedTextureResource,mce::PerFrameHandleTracker,dragon::res::TextureDescription>,std::shared_ptr> > */
  
  ClientTexture();
  ClientTexture(dragon::res::ClientTexture&);
  ClientTexture(const dragon::res::ClientTexture&);
  ClientTexture(mce::ClientResourcePointer<mce::ResourcePointer<dragon::res::ResolvedTextureResource,mce::ResourceBlockTemplate<dragon::res::ResolvedTextureResource,mce::PerFrameHandleTracker,dragon::res::TextureDescription>,std::shared_ptr> >&);
  ~ClientTexture();
  dragon::res::ClientTexture& operator=(dragon::res::ClientTexture&);
  dragon::res::ClientTexture& operator=(const dragon::res::ClientTexture&);
  void* __vecDelDtor(uint32_t);
};
