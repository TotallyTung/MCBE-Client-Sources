
struct mce::TextureDescription : public cg::TextureDescription { /* Size=0x70 */
  /* 0x0000: fields for cg::TextureDescription */
  /* 0x0048 */ mce::SampleDescription mSampleDescription;
  /* 0x0050 */ mce::Color mClearColor;
  /* 0x0060 */ float mOptimizedClearDepth;
  /* 0x0064 */ unsigned char mOptimizedClearStencil;
  /* 0x0068 */ mce::BindFlagsBit mBindFlags;
  /* 0x006c */ bool mIsStaging;
  
  TextureDescription(const cg::ImageDescription&, mce::BindFlagsBit, const mce::SampleDescription&);
  TextureDescription(const mce::TextureDescription&);
  TextureDescription();
  mce::TextureDescription& operator=(const mce::TextureDescription&);
  bool operator==(const mce::TextureDescription&) const;
  bool operator!=(const mce::TextureDescription&) const;
  uint32_t getMipWidth(const uint32_t) const;
  uint32_t getMipHeight(const uint32_t) const;
  ~TextureDescription();
  void* __vecDelDtor(uint32_t);
};
