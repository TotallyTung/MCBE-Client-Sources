
class IMinecraftApp { /* Size=0x8 */
  
  virtual ~IMinecraftApp();
  virtual Minecraft& getPrimaryMinecraft();
  virtual Automation::AutomationClient& getAutomationClient() const;
  virtual bool isEduMode() const;
  virtual bool isDedicatedServer() const;
  virtual void onNetworkMaxPlayersChanged(uint32_t);
  virtual IGameModuleShared& getGameModule();
  IMinecraftApp(const IMinecraftApp&);
  IMinecraftApp();
  IMinecraftApp& operator=(const IMinecraftApp&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
