
class Core::StorageAreaStateListener { /* Size=0x68 */
  /* 0x0008 */ std::shared_ptr<Core::FileStorageArea> mFileStorageArea;
  /* 0x0018 */ std::mutex mMutex;
  
  StorageAreaStateListener();
  virtual ~StorageAreaStateListener();
  virtual void onExtendDiskSpace(const bool, std::weak_ptr<Core::FileStorageArea>&, const uint64_t&, std::function<void __cdecl(void)>);
  virtual void onLowDiskSpace(const bool);
  virtual void onOutOfDiskSpace(const bool);
  virtual void onCriticalDiskError(const bool, const Core::LevelStorageState&);
  void initListener(std::shared_ptr<Core::FileStorageArea>);
  void removeListener();
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
