
class LoopbackPacketSender : public PacketSender { /* Size=0x50 */
  /* 0x0000: fields for PacketSender */
  /* 0x0010 */ NetworkHandler& mNetwork;
  /* 0x0018 */ std::vector<NetEventCallback *,std::allocator<NetEventCallback *> > mLoopbackCallbacks;
  /* 0x0030 */ const std::vector<std::unique_ptr<Player,std::default_delete<Player> >,std::allocator<std::unique_ptr<Player,std::default_delete<Player> > > >* mUserList;
  /* 0x0038 */ std::vector<NetworkIdentifierWithSubId,std::allocator<NetworkIdentifierWithSubId> > mTempUserIds;
  
  LoopbackPacketSender(LoopbackPacketSender&);
  LoopbackPacketSender(const LoopbackPacketSender&);
  LoopbackPacketSender(unsigned char, NetworkHandler&);
  void addLoopbackCallback(NetEventCallback&);
  void removeLoopbackCallback(NetEventCallback&);
  void setUserList(const std::vector<std::unique_ptr<Player,std::default_delete<Player> >,std::allocator<std::unique_ptr<Player,std::default_delete<Player> > > >*);
  virtual void send(Packet&);
  virtual void sendToServer(Packet&);
  virtual void sendToClient(const NetworkIdentifier&, const Packet&, unsigned char);
  virtual void sendToClients(const std::vector<NetworkIdentifierWithSubId,std::allocator<NetworkIdentifierWithSubId> >&, const Packet&);
  virtual void sendBroadcast(const NetworkIdentifier&, unsigned char, const Packet&);
  virtual void sendBroadcast(const Packet&);
  virtual void flush(const NetworkIdentifier&, std::function<void __cdecl(void)>&);
  virtual ~LoopbackPacketSender();
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
