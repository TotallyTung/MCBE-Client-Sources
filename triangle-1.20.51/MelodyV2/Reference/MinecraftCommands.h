
class MinecraftCommands { /* Size=0xa0 */
  /* 0x0000 */ std::unique_ptr<CommandOutputSender,std::default_delete<CommandOutputSender> > mOutputSender;
  /* 0x0008 */ std::unique_ptr<CommandRegistry,std::default_delete<CommandRegistry> > mRegistry;
  /* 0x0010 */ CommandPermissionLevel mOpPermissionLevel;
  /* 0x0018 */ Minecraft& mMinecraft;
  /* 0x0020 */ std::function<bool __cdecl(void)> mChatPermissionsCallback;
  /* 0x0060 */ std::unordered_map<HashedString,std::unique_ptr<Command,std::default_delete<Command> >,std::hash<HashedString>,std::equal_to<HashedString>,std::allocator<std::pair<HashedString const ,std::unique_ptr<Command,std::default_delete<Command> > > > > mCompiledCommandMap;
  
  MinecraftCommands(MinecraftCommands&);
  MinecraftCommands(Minecraft&, Automation::AutomationClient&);
  MCRESULT requestCommandExecutionWhatever(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, Player*, bool) const;
  MCRESULT executeCommandWhatever(std::shared_ptr<CommandContext>) const;
  MCRESULT requestCommandExecution(std::unique_ptr<CommandOrigin,std::default_delete<CommandOrigin> >, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, int32_t, bool) const;
  MCRESULT executeCommand(std::shared_ptr<CommandContext>, bool) const;
  void runCommand(Command&, CommandOrigin&);
  void runCommand(const HashedString&, CommandOrigin&, const CurrentCmdVersion);
  Command* compileCommand(const HashedString&, CommandOrigin&, const CurrentCmdVersion, std::function<void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)>);
  CommandPermissionLevel getOpPermissionLevel() const;
  void setOpPermissionLevel(CommandPermissionLevel);
  void setRegistryNetworkUpdateCallback(std::function<void __cdecl(Packet const &)>) const;
  void registerChatPermissionsCallback(std::function<bool __cdecl(void)>);
  void setOutputSender(std::unique_ptr<CommandOutputSender,std::default_delete<CommandOutputSender> >);
  CommandOutputSender& getOutputSender() const;
  void handleOutput(const CommandOrigin&, const CommandOutput&) const;
  const CommandRegistry& getRegistry() const;
  CommandRegistry& getRegistry();
  void initCoreEnums(const IWorldRegistriesProvider&, const ActorFactory&, const Experiments&, const BaseGameVersion&);
  void initCoreEnumsServer(const ActorDefinitionGroup&);
  ~MinecraftCommands();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
  
  static CommandOutputType getOutputType(const CommandOrigin&);
};
