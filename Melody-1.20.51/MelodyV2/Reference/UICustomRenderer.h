
class UICustomRenderer { /* Size=0x10 */
  /* 0x0008 */ float mPropagatedAlpha;
  
  UICustomRenderer(const UICustomRenderer&);
  UICustomRenderer();
  virtual ~UICustomRenderer();
  virtual void preRenderSetup(UIRenderContext&);
  virtual std::shared_ptr<UICustomRenderer> clone() const;
  virtual bool update(IClientInstance&, UIControl&, const UIScene&);
  virtual void frameUpdate(UIFrameUpdateContext&, UIControl&);
  virtual void render(UIRenderContext&, IClientInstance&, UIControl&, int32_t, RectangleArea&);
  virtual UIBatchType getBatchType() const;
  virtual int32_t getCustomId() const;
  virtual int32_t getNumRenderPasses() const;
  virtual ResourceLocation getResourceLocation(int32_t, int32_t) const;
  virtual UIMaterialType getUIMaterialType(int32_t) const;
  virtual bool getRequiresPreRenderSetup(int32_t) const;
  virtual void onVisibilityChanged(bool);
  virtual void collectScreenEvents(std::queue<ScreenEvent,std::deque<ScreenEvent,std::allocator<ScreenEvent> > >&);
  void setPropagatedAlpha(float);
  UICustomRenderer& operator=(const UICustomRenderer&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
