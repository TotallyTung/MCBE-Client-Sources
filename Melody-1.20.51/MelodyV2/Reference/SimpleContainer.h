
class SimpleContainer : public Container { /* Size=0x100 */
  /* 0x0000: fields for Container */
  /* 0x00e0 */ int32_t mSize;
  /* 0x00e8 */ std::vector<ItemStack,std::allocator<ItemStack> > mItems;
  
  SimpleContainer(SimpleContainer&);
  SimpleContainer(const SimpleContainer&);
  SimpleContainer(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, bool, int32_t, ContainerType);
  virtual const ItemStack& getItem(int32_t) const;
  virtual void setItem(int32_t, const ItemStack&);
  virtual int32_t getContainerSize() const;
  virtual int32_t getMaxStackSize() const;
  virtual void startOpen(Player&);
  virtual void stopOpen(Player&);
  void clearContent();
  virtual void serverInitItemStackIds(int32_t, int32_t, std::function<void __cdecl(int,ItemStack const &)>);
  virtual ~SimpleContainer();
  SimpleContainer& operator=(SimpleContainer&);
  SimpleContainer& operator=(const SimpleContainer&);
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
