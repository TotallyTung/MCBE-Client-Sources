
class ClientInstanceScreenController : public MinecraftScreenController { /* Size=0xa70 */
  /* 0x0000: fields for MinecraftScreenController */
  /* 0x0a58 */ std::shared_ptr<ClientInstanceScreenModel> mClientInstanceScreenModel;
  /* 0x0a68 */ std::unique_ptr<PlatformMultiplayerRestrictions,std::default_delete<PlatformMultiplayerRestrictions> > mPlatformMultiplayerRestrictions;
  
  ClientInstanceScreenController(std::shared_ptr<ClientInstanceScreenModel>);
  virtual ~ClientInstanceScreenController();
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
