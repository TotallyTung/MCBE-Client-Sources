class BuoyancyComponent : public IEntityComponent {
  private:
    float mBaseBuoyancy;
    float mDragDownOnRemoval;
    float mBigWaveProbability;
    float mBigWaveSpeedMultiplier;
    bool mSimulateWaves;
    bool mApplyGravity;
    double mTimer;
    std::vector<BlockDescriptor, std::allocator<BlockDescriptor> > mLiquidBlocks;

  public:
    BuoyancyComponent(void);
    void loadData(const std::__cxx11::string &, const SemVersion &);
    bool canFloat(const Actor &) const;
    bool needToResurface(const Actor &) const;
    const std::vector<BlockDescriptor, std::allocator<BlockDescriptor> > & getLiquidBlockDescriptors(void) const;
    float getBaseBuoyancy(void) const;
    float getDragDownOnRemoval(void) const;
    float getBigWaveProbability(void) const;
    float getBigWaveSpeedMultiplier(void) const;
    bool shouldSimulateWaves(void) const;
    bool shouldApplyGravity(void) const;
    double getTimer(void) const;
    void increaseTimer(float);
  private:
    bool _isValidLiquidBlock(const Block &) const;
}
