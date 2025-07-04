
class ItemStackNetManagerClient : public ItemStackNetManagerBase { /* Size=0x68 */
  /* 0x0000: fields for ItemStackNetManagerBase */
  /* 0x0038 */ bool mIsHandlingResponse;
  /* 0x0040 */ std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > mRequest;
  /* 0x0048 */ std::unique_ptr<ItemStackRequestBatch,std::default_delete<ItemStackRequestBatch> > mRequestBatch;
  /* 0x0050 */ WeakRefT<EntityRefTraits> mRequestScreen;
  
  virtual ~ItemStackNetManagerClient();
  ItemStackNetManagerClient(ItemStackNetManagerClient&);
  ItemStackNetManagerClient(const ItemStackNetManagerClient&);
  ItemStackNetManagerClient(LocalPlayer&, bool);
  virtual SparseContainer* initOpenContainer(BlockSource&, ContainerEnumName, const ContainerWeakRef&);
  void cacheHistoricPrediction(SparseContainer*, const TypedClientNetId<ItemStackRequestIdTag,int,0>&, int32_t, ItemStack&);
  void cacheZeroedOutItem(SparseContainer*, const TypedClientNetId<ItemStackRequestIdTag,int,0>&, int32_t, ItemStack&);
  void clearZeroedOutItem(SparseContainer*, const TypedClientNetId<ItemStackRequestIdTag,int,0>&, int32_t);
  bool hasActiveRequest() const;
  virtual TypedClientNetId<ItemStackRequestIdTag,int,0> getRequestId() const;
  virtual bool allowInventoryTransactionManager() const;
  bool _hasLegacyTransactionRequest() const;
  TypedClientNetId<ItemStackLegacyRequestIdTag,int,0> _getLegacyTransactionRequest() const;
  std::unordered_map<enum ContainerEnumName,std::unordered_set<unsigned char,std::hash<unsigned char>,std::equal_to<unsigned char>,std::allocator<unsigned char> >,std::hash<enum ContainerEnumName>,std::equal_to<enum ContainerEnumName>,std::allocator<std::pair<enum ContainerEnumName const ,std::unordered_set<unsigned char,std::hash<unsigned char>,std::equal_to<unsigned char>,std::allocator<unsigned char> > > > > _getLegacyTransactionSetItemSlots() const;
  ItemStackRequestScope tryBeginRequest();
  OwnedItemStackRequestScope tryBeginOwnedRequest(std::shared_ptr<ItemStackRequestData>, ItemStackRequestScreen);
  void addRequestAction(std::unique_ptr<ItemStackRequestAction,std::default_delete<ItemStackRequestAction> >);
  int32_t addRequestToFilterString(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  void trySendBatch();
  void addContainerToRequest(TypedClientNetId<ItemStackRequestIdTag,int,0>, SparseContainer*);
  void handleItemStackResponse(const std::vector<ItemStackResponseInfo,std::allocator<ItemStackResponseInfo> >&);
  void _initPlayerContainers();
  virtual gsl::final_action<std::function<void __cdecl(void)> > _tryBeginClientLegacyTransactionRequest();
  virtual void _addLegacyTransactionRequestSetItemSlot(ItemStackNetManagerScreen&, ContainerType, int32_t);
  virtual void _initScreen(ItemStackNetManagerScreen&);
  void _beginRequest(ItemStackRequestScreen);
  void _endRequest();
  std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > _endTakeRequest();
  ItemStackNetManagerClient::PredictiveContainer* _getPredictiveContainer(SparseContainer*);
  void _processResponseSlot(const TypedClientNetId<ItemStackRequestIdTag,int,0>&, ItemStackNetManagerClient::PredictiveContainer&, const ItemStackResponseSlotInfo&);
  void _clearPredictiveContainerRequest(const TypedClientNetId<ItemStackRequestIdTag,int,0>&, ItemStackNetManagerClient::PredictiveContainer&, bool);
  const ItemStackNetManagerClient::ClientScreenData* _tryGetCurrentClientScreen() const;
  ItemStackNetManagerClient::ClientScreenData* _tryGetCurrentClientScreen();
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
