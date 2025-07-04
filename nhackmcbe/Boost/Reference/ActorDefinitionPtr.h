class ActorDefinitionPtr {
  public:
    static const ActorDefinitionPtr NONE;
  protected:
    ActorDefinitionGroup *mGroup;
    ActorDefinition *mPtr;

  public:
    ActorDefinitionPtr(void);
    ActorDefinitionPtr(ActorDefinitionGroup &, ActorDefinition &);
    ActorDefinitionPtr(ActorDefinitionPtr &&);
    ActorDefinitionPtr(const ActorDefinitionPtr &);
    ~ActorDefinitionPtr(void);
    ActorDefinitionPtr & operator=(ActorDefinitionPtr &&);
    ActorDefinitionPtr & operator=(const ActorDefinitionPtr &);
    const ActorDefinition & operator*(void) const;
    ActorDefinition & operator*(void);
    const ActorDefinition * operator->(void) const;
    ActorDefinition * operator->(void);
    operator bool(void) const;
    void onGroupReloaded(void);
    void onGroupDestroyed(void);
  protected:
    ActorDefinitionPtr & _move(ActorDefinitionPtr &&);
    void _deref(void);
}
