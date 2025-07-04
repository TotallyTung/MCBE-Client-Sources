
class PacketSender { /* Size=0x10 */
  /* 0x0008 */ unsigned char mSenderSubId;
  
  PacketSender(const PacketSender&);
  PacketSender(unsigned char);
  virtual ~PacketSender();
  virtual void send(Packet&);
  virtual void sendToServer(Packet&);
  virtual void sendToClient(const NetworkIdentifier&, const Packet&, unsigned char);
  virtual void sendToClients(const std::vector<NetworkIdentifierWithSubId,std::allocator<NetworkIdentifierWithSubId> >&, const Packet&);
  void sendToPrimaryClient(const NetworkIdentifier&, const Packet&);
  virtual void sendBroadcast(const NetworkIdentifier&, unsigned char, const Packet&);
  virtual void sendBroadcast(const Packet&);
  virtual void flush(const NetworkIdentifier&, std::function<void __cdecl(void)>&);
  unsigned char getSenderSubId();
  PacketSender& operator=(const PacketSender&);
  virtual void* __vecDelDtor(uint32_t);
};
