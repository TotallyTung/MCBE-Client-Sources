
class mce::MeshData { /* Size=0xf0 */
  /* 0x0000 */ mce::PrimitiveMode mMode;
  /* 0x0001 */ bool mMeshNotFree;
  /* 0x0008 */ std::vector<glm::tvec3<float,0>,std::allocator<glm::tvec3<float,0> > > mPositions;
  /* 0x0020 */ std::vector<glm::tvec4<float,0>,std::allocator<glm::tvec4<float,0> > > mNormals;
  /* 0x0038 */ std::vector<unsigned int,std::allocator<unsigned int> > mIndices;
  /* 0x0050 */ std::vector<unsigned int,std::allocator<unsigned int> > mColors;
  /* 0x0068 */ std::vector<unsigned short,std::allocator<unsigned short> > mBoneId0s;
  /* 0x0080 */ std::vector<glm::tvec2<float,0>,std::allocator<glm::tvec2<float,0> > > mTextureUVs[3];
  /* 0x00c8 */ std::vector<unsigned short,std::allocator<unsigned short> > mPBRTextureIndices;
  /* 0x00e0 */ std::array<bool,12> mFieldEnabled;
  
  MeshData(const mce::MeshData&);
  MeshData(mce::MeshData&);
  MeshData();
  mce::MeshData& operator=(const mce::MeshData&);
  mce::MeshData& operator=(mce::MeshData&);
  void clear();
  mce::MeshData clone() const;
  const uint32_t getVertexCount() const;
  const uint64_t size() const;
  void freeMemory();
  void reserveVertices(uint64_t);
  void enableField(const mce::VertexField);
  bool hasField(const mce::VertexField) const;
  uint64_t getPolygonCount() const;
  void setMeshNotFree(bool);
  ~MeshData();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
