
class ItemStackNetManagerScreenStack { /* Size=0x28 */
  /* 0x0000 */ std::deque<std::unique_ptr<ItemStackNetManagerScreen,std::default_delete<ItemStackNetManagerScreen> >,std::allocator<std::unique_ptr<ItemStackNetManagerScreen,std::default_delete<ItemStackNetManagerScreen> > > > mStack;
  
  ~ItemStackNetManagerScreenStack();
  uint64_t size() const;
  ItemStackNetManagerScreen* push(std::unique_ptr<ItemStackNetManagerScreen,std::default_delete<ItemStackNetManagerScreen> >);
  bool pop();
  const ItemStackNetManagerScreen* top() const;
  ItemStackNetManagerScreen* top();
  void foreachScreen(const std::function<bool __cdecl(ItemStackNetManagerScreen &)>);
  ItemStackNetManagerScreen* getScreenForRequest(ItemStackRequestScreen);
  ItemStackNetManagerScreen* getScreenForRequest(const ItemStackRequestData&);
  ItemStackNetManagerScreenStack(const ItemStackNetManagerScreenStack&);
  ItemStackNetManagerScreenStack();
  ItemStackNetManagerScreenStack& operator=(const ItemStackNetManagerScreenStack&);
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
