
class ISubChunkBlockStoragePaletted : public SubChunkBlockStorage { /* Size=0x8 */
  /* 0x0000: fields for SubChunkBlockStorage */
  
  virtual gsl::span<unsigned int const ,-1> getBlocks() const;
  virtual gsl::span<unsigned int,-1> getBlocks();
  virtual uint64_t getBitsPerBlock() const;
  virtual gsl::span<Block const * const,-1> getPaletteSnapshot() const;
  ISubChunkBlockStoragePaletted(ISubChunkBlockStoragePaletted&);
  ISubChunkBlockStoragePaletted(const ISubChunkBlockStoragePaletted&);
  ISubChunkBlockStoragePaletted();
  virtual ~ISubChunkBlockStoragePaletted();
  ISubChunkBlockStoragePaletted& operator=(ISubChunkBlockStoragePaletted&);
  ISubChunkBlockStoragePaletted& operator=(const ISubChunkBlockStoragePaletted&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
  
  static uint64_t getBlocksPerWord(uint64_t);
};
