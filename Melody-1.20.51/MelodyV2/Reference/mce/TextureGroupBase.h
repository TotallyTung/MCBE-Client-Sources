
class mce::TextureGroupBase { /* Size=0x8 */
  
  TextureGroupBase(const mce::TextureGroupBase&);
  TextureGroupBase();
  virtual ~TextureGroupBase();
  virtual mce::TexturePtr getTexture(const ResourceLocation&, bool);
  mce::TextureGroupBase& operator=(const mce::TextureGroupBase&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
