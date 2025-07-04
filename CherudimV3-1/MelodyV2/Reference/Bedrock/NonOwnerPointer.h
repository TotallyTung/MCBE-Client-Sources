template<typename T>
class Bedrock::NonOwnerPointer<T> { /* Size=0x10 */
  /* 0x0000 */ std::shared_ptr<T::ControlBlock> mControlBlock;
  
  NonOwnerPointer<T>(Bedrock::NonOwnerPointer<T>&);
  NonOwnerPointer<T>(const Bedrock::NonOwnerPointer<T>&);
  NonOwnerPointer<T>(T&);
  NonOwnerPointer<T>(T*);
  NonOwnerPointer<T>();
  ~NonOwnerPointer<T>();
  Bedrock::NonOwnerPointer<T>& operator=(void*);
  Bedrock::NonOwnerPointer<T>& operator=(Bedrock::NonOwnerPointer<T>&);
  Bedrock::NonOwnerPointer<T>& operator=(const Bedrock::NonOwnerPointer<T>&);
  Bedrock::NonOwnerPointer<T>& operator=(T&);
  Bedrock::NonOwnerPointer<T>& operator=(T*);
  void reset();
  T* operator->() const;
  T* operator->();
  T& operator*() const;
  T& operator*();
  T* access() const;
  bool operator bool() const;
  bool isValid() const;
  void swap(Bedrock::NonOwnerPointer<T>&);
  bool operator==(void*) const;
  bool operator==(const T*) const;
  bool operator==(const Bedrock::NonOwnerPointer<T>&) const;
  bool operator!=(void*) const;
  bool operator!=(const T*) const;
  bool operator!=(const Bedrock::NonOwnerPointer<T>&) const;
  void _set(T*);
  T* _get() const;
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
