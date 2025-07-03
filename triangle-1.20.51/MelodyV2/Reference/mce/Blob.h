class mce::Blob {
  private:
    pointer_type mBlob;
    size_type mSize;

    static iterator defaultAllocator(size_type);
    static void defaultDeleter(iterator);
  public:
    Blob(void);
    Blob(const mce::Blob &);
    Blob(mce::Blob &&);
    Blob(size_type);
    Blob(iterator, size_type);
    Blob(pointer_type &&, size_type);
    mce::Blob & operator=(const mce::Blob &);
    mce::Blob & operator=(mce::Blob &&);
    ~Blob(void);
    iterator begin(void);
    iterator end(void);
    iterator cbegin(void) const;
    iterator cend(void) const;
    void swap(mce::Blob &);
    size_type size(void) const;
    size_type max_size(void) const;
    bool empty(void) const;
    iterator data(void);
    iterator data(void) const;
    iterator get(void);
    iterator get(void) const;
    gsl::span<value_type, -1> getSpan(void);
    gsl::span<value_type const, -1> getSpan(void) const;
    pointer_type release(void);
    void fillBlob(value_type);

  private:
    typedef std::unique_ptr<unsigned char [], mce::Blob::Deleter> pointer_type;
    typedef void (*delete_function)(mce::Blob::value_type *);
    typedef uint8_t value_type;
    typedef std::size_t size_type;
    typedef mce::Blob::value_type *iterator;
    typedef const mce::Blob::value_type *const_iterator;
}
