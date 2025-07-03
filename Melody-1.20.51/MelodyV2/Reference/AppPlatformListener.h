
class AppPlatformListener { /* Size=0x10 */
  /* 0x0008 */ bool mListenerRegistered;
  
  AppPlatformListener(const AppPlatformListener&);
  AppPlatformListener(bool);
  AppPlatformListener();
  virtual ~AppPlatformListener();
  void initListener(float);
  virtual void onLowMemory();
  virtual void onAppPaused();
  virtual void onAppUnpaused();
  virtual void onAppPreSuspended();
  virtual void onAppSuspended();
  virtual void onAppResumed();
  virtual void onAppFocusLost();
  virtual void onAppFocusGained();
  virtual void onAppTerminated();
  virtual void onOperationModeChanged(const OperationMode);
  virtual void onPerformanceModeChanged(const bool);
  virtual void onPushNotificationReceived(const PushNotificationMessage&);
  virtual void onResizeBegin();
  virtual void onResizeEnd();
  virtual void onDeviceLost();
  void terminate();
  AppPlatformListener& operator=(const AppPlatformListener&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
