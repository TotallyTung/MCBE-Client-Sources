
class CustomRenderComponent : public RenderableComponent { /* Size=0x28 */
  /* 0x0000: fields for RenderableComponent */
  /* 0x0018 */ std::shared_ptr<UICustomRenderer> mRenderer;
  static const int32_t mInvalidCustomId;
  
  CustomRenderComponent(const CustomRenderComponent&);
  CustomRenderComponent(UIControl&);
  virtual ~CustomRenderComponent();
  virtual std::unique_ptr<UIComponent,std::default_delete<UIComponent> > clone(UIControl&) const;
  virtual void reset();
  void frameUpdate(UIFrameUpdateContext&, UIControl&);
  bool update(UIRenderContext&);
  bool updateCustom(IClientInstance&, const UIScene&);
  void render(UIRenderContext&, int32_t, RectangleArea&);
  void renderCustom(UIRenderContext&, IClientInstance&, int32_t, RectangleArea&);
  void setRenderer(std::shared_ptr<UICustomRenderer>);
  std::shared_ptr<UICustomRenderer> getRender();
  virtual void onVisibilityChanged(bool);
  void preRenderSetup(UIRenderContext&);
  virtual UIBatchType getBatchType() const;
  virtual int32_t getCustomId() const;
  virtual int32_t getNumRenderPasses() const;
  virtual UIMaterialType getUIMaterialType(int32_t) const;
  virtual ResourceLocation getResourceLocation(int32_t, int32_t) const;
  virtual bool getRequiresPreRenderSetup(int32_t) const;
  virtual void collectScreenEvents(std::queue<ScreenEvent,std::deque<ScreenEvent,std::allocator<ScreenEvent> > >&);
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
  
  static int32_t getInvalidCustomId();
};
