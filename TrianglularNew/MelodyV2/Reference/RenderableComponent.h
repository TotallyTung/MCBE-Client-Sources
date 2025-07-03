
class RenderableComponent : public UIComponent { /* Size=0x18 */
  /* 0x0000: fields for UIComponent */
  /* 0x0010 */ float mPropagatedAlpha;
  
  RenderableComponent(const RenderableComponent&);
  RenderableComponent(UIControl&);
  virtual ~RenderableComponent();
  void setPropagatedAlpha(const float);
  float getPropagatedAlpha() const;
  virtual void updateUI(const UIMeasureStrategy&);
  virtual void reset();
  virtual bool isRenderableComponent();
  virtual bool overridesLayoutAxisOffset(const LayoutVariableType) const;
  virtual float getLayoutAxisOffsetOverride(const LayoutVariableType) const;
  virtual void* __vecDelDtor(uint32_t);
  
  static const HashedString& getUIMaterialName(UIMaterialType);
};
