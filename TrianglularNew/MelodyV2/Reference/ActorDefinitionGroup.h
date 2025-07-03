class ActorDefinitionGroup : public Bedrock::EnableNonOwnerReferences {
  protected:
    std::unordered_set<ActorDefinitionPtr*, std::hash<ActorDefinitionPtr*>, std::equal_to<ActorDefinitionPtr*>, std::allocator<ActorDefinitionPtr*> > mRegisteredPtrs;
  private:
    ActorDefinitionList mDefinitions;
    std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, ActorDefinitionGroup::EDLWrapper, std::hash<std::__cxx11::string>, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, ActorDefinitionGroup::EDLWrapper> > > mTemplateMap;
    ResourcePackManager &mResourcePackManager;
    Bedrock::Threading::Mutex mReferenceMutex;
    IMinecraftEventing &mEventing;
    ActorComponentFactory *mComponentFactory;
    std::unique_ptr<ActorEventResponseFactory, std::default_delete<ActorEventResponseFactory> > mResponseFactory;
    Experiments mExperiments;

  public:
    ActorDefinitionGroup(Level &, ResourcePackManager &, IMinecraftEventing &, const Experiments &);
    virtual ~ActorDefinitionGroup(void);
    ActorDefinitionPtr tryGetDefinition(const std::__cxx11::string &);
    ActorDefinitionPtr tryGetDefinitionGroup(const std::__cxx11::string &, const std::__cxx11::string &);
    const ActorDefinitionList & getList(void) const;
    const std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, ActorDefinitionGroup::EDLWrapper, std::hash<std::__cxx11::string>, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, ActorDefinitionGroup::EDLWrapper> > > & getTemplateMap(void) const;
    static bool tryReadEntityResourceFile(PackInstance &, const Core::Path &, Json::Value &);
    static void loadActorDefinitionFormatVersion(const Json::Value &, SemVersion &);
    static bool loadActorDefinitionIdentifier(const Json::Value &, const SemVersion &, std::__cxx11::string &);
    static bool loadActorDefinitionRuntimeIdentifier(const Json::Value &, const SemVersion &, std::__cxx11::string &);
    std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > buildActorEventList(void) const;
    ActorComponentFactory * getComponentFactory(void);
    void setComponentFactory(ActorComponentFactory *);
  protected:
    void _addRef(ActorDefinitionPtr &);
    void _removeRef(ActorDefinitionPtr &);
  private:
    void _loadTemplates(Level &, const std::__cxx11::string &, DeserializeDataParams);
    void _getResources(Level &);
    void _setupPropertyGroups(Level &);
    void _setupPropertyGroups(Level &, const ActorDefinition &);
    void _removePropertyGroup(PropertyGroupManager &, const std::__cxx11::string &);
    void _setupCommonResourceDefinitionMap(ActorDefinition &, Level &);
    bool _loadDefinitionFromJSON(const SemVersion &, const SemVersion &, const CurrentCmdVersion &, const std::__cxx11::string &, Json::Value &, const std::__cxx11::string &, Level &, LogArea);
    ActorDefinitionGroup::LoadActorResult _loadActorDefinition(Level &, const SemVersion &, const std::__cxx11::string &, const CurrentCmdVersion &, Json::Value &, std::unordered_set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::hash<std::__cxx11::string>, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > &, LogArea);
    bool hasComponent(const HashedString &, const Json::Value &, const Json::Value &) const;
    bool hasPlayerAsTarget(const Json::Value &) const;

    typedef std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::unique_ptr<ActorDefinition, std::default_delete<ActorDefinition> >, std::hash<std::__cxx11::string>, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::unique_ptr<ActorDefinition, std::default_delete<ActorDefinition> > > > > ActorDefinitionList;
}
