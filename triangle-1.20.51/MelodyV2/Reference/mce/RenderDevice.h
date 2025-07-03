
class mce::RenderDevice : public mce::RenderDeviceNull, protected mce::ResourceBase<mce::RenderDeviceNull>, public mce::Singleton<mce::RenderDevice> { /* Size=0x118 */
  /* 0x0000: fields for mce::RenderDeviceNull */
  /* 0x0110: fields for mce::ResourceBase<mce::RenderDeviceNull> */
  /* 0x0111: fields for mce::Singleton<mce::RenderDevice> */
  static int16_t WaterTessellateLevel;
  
  RenderDevice();
  ~RenderDevice();
  void createDevice(mce::RenderContext&);
  mce::RenderContext& getRenderContext();
  const mce::RenderContext& getRenderContextAsConst() const;
  void lostDevice();
  void getMultisampleQualityLevels(const mce::TextureFormat, std::vector<mce::SampleDescription,std::allocator<mce::SampleDescription> >&) const;
  mce::SampleDescription getClosestSupportedSampleDescription(const mce::TextureFormat, const mce::SampleDescription&) const;
  bool checkFeatureSupport(const mce::RenderFeature) const;
  uint32_t getMaxVertexCount();
  bool deviceWasLostOnPresent() const;
  void createAdapterFromLUID(uint32_t, int32_t);
  void waitForGPUIdle();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
