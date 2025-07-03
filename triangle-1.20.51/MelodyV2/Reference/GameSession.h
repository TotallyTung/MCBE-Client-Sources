
class GameSession { /* Size=0x38 */
  /* 0x0000 */ NetworkHandler& mNetworkHandler;
  /* 0x0008 */ std::unique_ptr<Level,std::default_delete<Level> > mLevel;
  /* 0x0010 */ std::unique_ptr<ServerNetworkHandler,std::default_delete<ServerNetworkHandler> > mServerNetworkHandler;
  /* 0x0018 */ std::unique_ptr<NetEventCallback,std::default_delete<NetEventCallback> > mLegacyClientNetworkHandler;
  /* 0x0020 */ std::unique_ptr<NetEventCallback,std::default_delete<NetEventCallback> > mClientNetworkHandler;
  /* 0x0028 */ LoopbackPacketSender& mLoopbackPacketSender;
  /* 0x0030 */ unsigned char mClientSubId;
  
  GameSession(NetworkHandler&, std::unique_ptr<ServerNetworkHandler,std::default_delete<ServerNetworkHandler> >, LoopbackPacketSender&, std::unique_ptr<NetEventCallback,std::default_delete<NetEventCallback> >, std::unique_ptr<Level,std::default_delete<Level> >, unsigned char);
  GameSession(NetworkHandler&, std::unique_ptr<NetEventCallback,std::default_delete<NetEventCallback> >, LoopbackPacketSender&, unsigned char);
  ~GameSession();
  void tick();
  void setLevel(std::unique_ptr<Level,std::default_delete<Level> >);
  void startLeaveGame();
  bool isLeaveGameDone();
  Level* getLevel() const;
  NetEventCallback& getNetEventCallback();
  ServerNetworkHandler* getServerNetworkHandler();
  void subClientJoinWorldInProgress();
  unsigned char getClientSubId() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
