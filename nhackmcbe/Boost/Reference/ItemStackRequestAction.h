
class ItemStackRequestAction { /* Size=0x10 */
  /* 0x0008 */ ItemStackRequestActionType mActionType;
  static const BidirectionalUnorderedMap<enum ItemStackRequestActionType,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > actionTypeMap;
  
  virtual ~ItemStackRequestAction();
  ItemStackRequestAction(ItemStackRequestAction&);
  ItemStackRequestAction(const ItemStackRequestAction&);
  ItemStackRequestAction(const ItemStackRequestActionType);
  ItemStackRequestAction& operator=(ItemStackRequestAction&);
  ItemStackRequestAction& operator=(const ItemStackRequestAction&);
  void write(BinaryStream&) const;
  ItemStackRequestActionType getActionType() const;
  virtual const ItemStackRequestActionCraftBase* getCraftAction() const;
  virtual int32_t getFilteredStringIndex() const;
  virtual void _write(BinaryStream&) const;
  virtual bool _read(ReadOnlyBinaryStream&);
  virtual void* __vecDelDtor(uint32_t);
  
  static std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> > read(ReadOnlyBinaryStream&);
  static const std::basic_string<char,std::char_traits<char>,std::allocator<char> > getActionTypeName(ItemStackRequestActionType);
};
