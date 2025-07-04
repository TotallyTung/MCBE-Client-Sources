
class LevelListener : public BlockSourceListener { /* Size=0x8 */
  /* 0x0000: fields for BlockSourceListener */
  
  virtual ~LevelListener();
  virtual void allChanged();
  virtual Particle* addParticle(ParticleType, const Vec3&, const Vec3&, int32_t, const CompoundTag*, bool);
  virtual void sendServerLegacyParticle(ParticleType, const Vec3&, const Vec3&, int32_t);
  virtual void addParticleEffect(const HashedString&, const Actor&, const HashedString&, const Vec3&, const MolangVariableMap&);
  virtual void addParticleEffect(const HashedString&, const Vec3&, const MolangVariableMap&);
  virtual void addTerrainParticleEffect(const BlockPos&, const Block&, const Vec3&, float, float, float);
  virtual void addTerrainSlideEffect(const BlockPos&, const Block&, const Vec3&, float, float, float);
  virtual void addBreakingItemParticleEffect(const Vec3&, ParticleType, const TextureUVCoordinateSet&, bool);
  virtual void playMusic(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const Vec3&, float, float);
  virtual void playStreamingMusic(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, int32_t, int32_t, int32_t);
  virtual void onEntityAdded(Actor&);
  virtual void onEntityRemoved(Actor&);
  virtual void onChunkLoaded(ChunkSource&, LevelChunk&);
  virtual void onChunkUnloaded(LevelChunk&);
  virtual void onLevelDestruction(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  virtual void levelEvent(LevelEvent, const CompoundTag&);
  virtual void levelEvent(LevelEvent, const Vec3&, int32_t);
  virtual void levelSoundEvent(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const Vec3&, float, float);
  virtual void levelSoundEvent(LevelSoundEvent, const Vec3&, int32_t, const ActorDefinitionIdentifier&, bool, bool);
  virtual void stopSoundEvent(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  virtual void stopAllSounds();
  virtual void takePicture(cg::ImageBuffer&, Actor*, Actor*, ScreenshotOptions&);
  virtual void playerListChanged();
  LevelListener(const LevelListener&);
  LevelListener();
  LevelListener& operator=(const LevelListener&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
