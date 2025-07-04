
class GuiMessage { /* Size=0x98 */
  /* 0x0000 */ GuiMessage::MessageType mType;
  /* 0x0008 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mMessage;
  /* 0x0028 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mTTSMessage;
  /* 0x0048 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mUsername;
  /* 0x0068 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mFullString;
  /* 0x0088 */ bool mForceVisible;
  /* 0x008c */ float mDuration;
  /* 0x0090 */ bool mHasBeenSeen;
  
  GuiMessage(GuiMessage&);
  GuiMessage(const GuiMessage&);
  GuiMessage(GuiMessage::MessageType, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, float, bool);
  GuiMessage();
  bool isCommand() const;
  GuiMessage::MessageType getType() const;
  const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getString() const;
  const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getUser() const;
  const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getMessage() const;
  const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getTTSMessage() const;
  bool isForceVisible() const;
  bool isAlive() const;
  void decrementTime(float);
  float getDuration() const;
  bool hasBeenSeen() const;
  void setHasBeenSeen();
  ~GuiMessage();
  GuiMessage& operator=(GuiMessage&);
  GuiMessage& operator=(const GuiMessage&);
  void* __vecDelDtor(uint32_t);
};
