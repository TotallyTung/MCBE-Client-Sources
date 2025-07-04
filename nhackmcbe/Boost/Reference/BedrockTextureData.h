
struct BedrockTextureData { /* Size=0xa0 */
  /* 0x0000 */ mce::ClientTexture mClientTexture;
  /* 0x0010 */ mce::TextureDescription mTextureDescription;
  /* 0x0080 */ IsMissingTexture mIsMissingTexture;
  /* 0x0081 */ TextureLoadState mTextureLoadState;
  /* 0x0088 */ cg::TextureSetImageDescription mTextureSetDescription;
  
  BedrockTextureData(BedrockTextureData&);
  BedrockTextureData(const BedrockTextureData&);
  BedrockTextureData();
  ~BedrockTextureData();
  BedrockTextureData& operator=(BedrockTextureData&);
  BedrockTextureData& operator=(const BedrockTextureData&);
  void* __vecDelDtor(uint32_t);
};
