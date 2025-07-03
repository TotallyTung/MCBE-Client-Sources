
class FontHandle { /* Size=0x30 */
  /* 0x0000 */ std::shared_ptr<FontRepository> mFontRepository;
  /* 0x0010 */ std::shared_ptr<Font> mDefaultFont;
  /* 0x0020 */ uint64_t mFontId;
  /* 0x0028 */ bool mIsDummyHandle;
  static FontHandle INVALID_FONT_HANDLE;
  static FontHandle TEST_FONT_HANDLE;
  
  FontHandle(const FontHandle&);
  FontHandle(bool);
  FontHandle();
  FontHandle(FontHandle, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  FontHandle(std::shared_ptr<Font>, const uint64_t, std::shared_ptr<FontRepository>);
  ~FontHandle();
  Font& getFont() const;
  uint64_t getFontId() const;
  bool isLoaded() const;
  bool isValid() const;
  FontHandle& operator=(const FontHandle&);
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
