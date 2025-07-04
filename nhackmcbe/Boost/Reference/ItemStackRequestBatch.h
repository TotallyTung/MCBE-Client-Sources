
class ItemStackRequestBatch { /* Size=0x18 */
  /* 0x0000 */ std::vector<std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> >,std::allocator<std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > > > mRequests;
  
  ~ItemStackRequestBatch();
  ItemStackRequestBatch(ItemStackRequestBatch&);
  ItemStackRequestBatch(const ItemStackRequestBatch&);
  ItemStackRequestBatch();
  ItemStackRequestBatch& operator=(ItemStackRequestBatch&);
  ItemStackRequestBatch& operator=(const ItemStackRequestBatch&);
  void write(BinaryStream&) const;
  void addRequest(std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> >);
  const std::vector<std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> >,std::allocator<std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > > >& getRequests() const;
  std::vector<std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> >,std::allocator<std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > > > takeRequests() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
  
  static std::unique_ptr<ItemStackRequestBatch,std::default_delete<ItemStackRequestBatch> > read(ReadOnlyBinaryStream&);
};
