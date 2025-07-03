
class HashedString { /* Size=0x30 */
  /* 0x0000 */ uint64_t mStrHash;
  /* 0x0008 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mStr;
  /* 0x0028 */ const HashedString* mLastMatch;
  static HashedString defaultErrorValue;
  
  HashedString(uint64_t, std::basic_string<char,std::char_traits<char>,std::allocator<char> >);
  HashedString(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  HashedString(const char*);
  HashedString(uint64_t, const char*);
  HashedString(HashedString&);
  HashedString(const HashedString&);
  HashedString(void*);
  HashedString();
  HashedString& operator=(HashedString&);
  HashedString& operator=(const HashedString&);
  bool operator==(const HashedString&) const;
  bool operator!=(const HashedString&) const;
  bool operator<(const HashedString&) const;
  gsl::basic_string_span<char const ,-1> operator class gsl::basic_string_span<char const ,-1>() const;
  uint64_t getHash() const;
  const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getString() const;
  const char* c_str() const;
  bool isEmpty() const;
  bool empty() const;
  void clear();
  ~HashedString();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
  
  static const HashedString& getEmptyString();
  static uint64_t computeHash(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static uint64_t computeHash(const char*);
};
