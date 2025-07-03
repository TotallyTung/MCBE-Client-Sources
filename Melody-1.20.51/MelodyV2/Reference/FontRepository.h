
class FontRepository : public AppPlatformListener, public std::enable_shared_from_this<FontRepository> { /* Size=0x180 */
  /* 0x0000: fields for AppPlatformListener */
  /* 0x0010: fields for std::enable_shared_from_this<FontRepository> */
  /* 0x0020 */ bool mIsInitialized;
  /* 0x0028 */ std::vector<std::shared_ptr<Font>,std::allocator<std::shared_ptr<Font> > > mLoadedFonts;
  /* 0x0040 */ std::unordered_map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,unsigned __int64,std::hash<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::equal_to<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,unsigned __int64> > > mFontNameToIdentifier;
  /* 0x0080 */ uint64_t mDefaultFontId;
  /* 0x0088 */ uint64_t mRuneFontId;
  /* 0x0090 */ uint64_t mUnicodeFontId;
  /* 0x0098 */ uint64_t mSmoothFontLatinFontId;
  /* 0x00a0 */ uint64_t mUIFontId;
  /* 0x00a8 */ uint64_t mSmoothSmallFontID;
  /* 0x00b0 */ std::unordered_map<unsigned __int64,std::shared_ptr<Font>,std::hash<unsigned __int64>,std::equal_to<unsigned __int64>,std::allocator<std::pair<unsigned __int64 const ,std::shared_ptr<Font> > > > mOriginalMinecraftFonts;
  /* 0x00f0 */ std::unordered_map<unsigned __int64,std::shared_ptr<Font>,std::hash<unsigned __int64>,std::equal_to<unsigned __int64>,std::allocator<std::pair<unsigned __int64 const ,std::shared_ptr<Font> > > > mOverriddenFonts;
  /* 0x0130 */ std::vector<std::shared_ptr<Bedrock::Threading::IAsyncResult<void> >,std::allocator<std::shared_ptr<Bedrock::Threading::IAsyncResult<void> > > > mFontLoadingTaskTrackers;
  /* 0x0148 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mLanguageCode;
  /* 0x0168 */ std::vector<std::shared_ptr<Font>,std::allocator<std::shared_ptr<Font> > > mDelayedFontDelete;
  static uint64_t INVALID_FONT;
  
  FontRepository(const FontRepository&);
  FontRepository();
  virtual ~FontRepository();
  void loadDefaultFonts(mce::TextureGroup&);
  bool haveDefaultFontsLoaded();
  void parseAndLoadMetadataFonts(mce::TextureGroup&);
  bool isRepositoryInitialized();
  void getDefaultFontHandles(FontHandle&, FontHandle&, FontHandle&, FontHandle&, FontHandle&);
  uint64_t addPreloadedFont(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, std::shared_ptr<Font>);
  void setFont(uint64_t, std::shared_ptr<Font>);
  FontHandle getFontHandle(const uint64_t);
  uint64_t getFontIdentifier(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  bool isFontIdInRepository(const uint64_t);
  void setUseFontOverrides(bool);
  void setMinecraftSmoothFontStyle(float, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  void setMinecraftUIFontStyle(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  void clearStringCache();
  void resetPerFontData();
  virtual void onAppSuspended();
  virtual void onDeviceLost();
  void reloadFontTextures(bool);
  std::vector<ResourceLocation,std::allocator<ResourceLocation> > getReloadFontTextures() const;
  bool isReloadingTextures();
  void onLanguageChanged(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  void clearDeleteFonts();
  void prepForShutdown();
  Font& getFont(uint64_t);
  void _setDefaultFont(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const uint64_t, std::shared_ptr<Font>);
  void _setFontIfOverride(const uint64_t, std::shared_ptr<Font>);
  FontRepository& operator=(const FontRepository&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
