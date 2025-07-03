
class mce::MeshContext { /* Size=0x70 */
  /* 0x0000 */ mce::RenderContext& renderContext;
  /* 0x0008 */ mce::Camera& camera;
  /* 0x0010 */ mce::GlobalConstantBuffers& constantBuffers;
  /* 0x0018 */ mce::GlobalConstantBufferManager& constantBufferManager;
  /* 0x0020 */ ShaderColor& currentShaderColor;
  /* 0x0028 */ ShaderColor& currentShaderDarkColor;
  /* 0x0030 */ mce::BufferResourceService& bufferResourceService;
  /* 0x0038 */ mce::QuadIndexBuffer& currentQuadIndexBuffer;
  /* 0x0040 */ mce::ServerResourcePointer<mce::ResourcePointer<mce::ImmediateBuffer,mce::ResourceBlockTemplate<mce::ImmediateBuffer,mce::PerFrameHandleTracker,dragon::BufferDescription>,std::shared_ptr>,mce::AssertResourceServiceErrorHandler> immediateBuffer;
  /* 0x0050 */ std::optional<glm::tvec4<float,0> > normalizedClipRegion;
  /* 0x0064 */ unsigned char subClientId;
  /* 0x0065 */ bool isDrawingUI;
  /* 0x0066 */ bool isDrawingFirstPersonObjects;
  /* 0x0067 */ bool isDrawingInLevelCubeMap;
  /* 0x0068 */ bool isDrawingEnvironmentalText;
  /* 0x0069 */ bool isUsingVRPatch;
  
  MeshContext(mce::RenderContext&, const unsigned char&, mce::Camera&, mce::GlobalConstantBuffers&, mce::GlobalConstantBufferManager&, ShaderColor&, ShaderColor&, mce::BufferResourceService&, mce::QuadIndexBuffer&, mce::ClientResourcePointer<mce::ResourcePointer<mce::ImmediateBuffer,mce::ResourceBlockTemplate<mce::ImmediateBuffer,mce::PerFrameHandleTracker,dragon::BufferDescription>,std::shared_ptr> >&);
  MeshContext(mce::MeshContext&);
  MeshContext(const mce::MeshContext&);
  mce::MeshContext& operator=(mce::MeshContext&);
  mce::MeshContext& operator=(const mce::MeshContext&);
  ~MeshContext();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
