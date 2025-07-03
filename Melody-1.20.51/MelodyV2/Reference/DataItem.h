class DataItem {
  using ID = unsigned short;

  protected:
    const DataItemType mType;
    ID mId;
    bool mDirty;

  public:
    DataItem(DataItemType, ID);
    virtual ~DataItem(void);
    ID getId(void) const;
    DataItemType getType(void) const;
    bool isDirty(void) const;
    void setDirty(bool);
    virtual bool isDataEqual(const DataItem &) const;
    virtual std::unique_ptr<DataItem, std::default_delete<DataItem> > clone(void) const;

  private:
    typedef uint16_t ID;
}
