
class GameCallbacks { /* Size=0x8 */
  
  virtual ~GameCallbacks();
  virtual void onLevelCorrupt();
  virtual void onGameModeChanged();
  virtual void onBeforeSimTick();
  virtual void onTick(int32_t, int32_t);
  virtual void onInternetUpdate();
  virtual void onGameSessionReset();
  virtual void onLevelExit();
  virtual void updateScreens();
  GameCallbacks(const GameCallbacks&);
  GameCallbacks();
  GameCallbacks& operator=(const GameCallbacks&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
