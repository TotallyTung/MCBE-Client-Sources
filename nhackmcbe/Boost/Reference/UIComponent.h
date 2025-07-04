
class UIComponent { /* Size=0x10 */
  /* 0x0008 */ UIControl& mOwner;
  
  UIComponent(const UIComponent&);
  UIComponent(UIControl&);
  virtual ~UIComponent();
  virtual void OnScreenPop();
  std::basic_string<char,std::char_traits<char>,std::allocator<char> > getScriptComponentPath();
  virtual std::unique_ptr<UIComponent,std::default_delete<UIComponent> > clone(UIControl&) const;
  const UIControl& getOwner() const;
  UIControl& getOwner();
  bool isLocked() const;
  virtual ComponentReceiveActionType receive(const ScreenEvent&);
  virtual ComponentReceiveActionType receive(VisualTree&, ScreenInputContext&, UIAnimationController&, const ScreenEvent&);
  virtual void onNotifyChildAdded();
  virtual void onNotifyChildRemoved();
  virtual void onRemoved();
  virtual void onAdded();
  virtual void onVisibilityChanged(bool);
  virtual void onEnabledChanged(bool);
  virtual bool isRenderableComponent();
  virtual bool onLayoutChange();
  virtual void reset();
  virtual void reload(const UIComponent&);
  virtual const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getTextToSpeechComponentValue() const;
  virtual void* __vecDelDtor(uint32_t);
};
