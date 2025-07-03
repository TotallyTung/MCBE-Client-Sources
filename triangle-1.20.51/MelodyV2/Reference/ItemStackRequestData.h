
class ItemStackRequestData { /* Size=0x38 */
  /* 0x0000 */ TypedClientNetId<ItemStackRequestIdTag,int,0> mClientRequestId;
  /* 0x0008 */ std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > mStringsToFilter;
  /* 0x0020 */ std::vector<std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> >,std::allocator<std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> > > > mActions;
  
  ~ItemStackRequestData();
  ItemStackRequestData(ItemStackRequestData&);
  ItemStackRequestData(const ItemStackRequestData&);
  ItemStackRequestData(const TypedClientNetId<ItemStackRequestIdTag,int,0>&);
  ItemStackRequestData();
  ItemStackRequestData& operator=(ItemStackRequestData&);
  ItemStackRequestData& operator=(const ItemStackRequestData&);
  void write(BinaryStream&) const;
  const TypedClientNetId<ItemStackRequestIdTag,int,0>& getRequestId() const;
  const std::vector<std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> >,std::allocator<std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> > > >& getActions() const;
  const ItemStackRequestAction* tryFindAction(ItemStackRequestActionType) const;
  void addAction(std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> >);
  int32_t addStringToFilter(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  const std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >& getStringsToFilter() const;
  TypedClientNetId<ItemStackRequestIdTag,int,0> getClientRequestId() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
  
  static std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > read(ReadOnlyBinaryStream&);
};
