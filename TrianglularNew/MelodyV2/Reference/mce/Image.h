type = struct mce::Image {
    mce::ImageFormat imageFormat;
    uint32_t mWidth;
    uint32_t mHeight;
    mce::ImageUsage mUsage;
    Storage mImageBytes;
  public:
    Image(void);
    Image(const uint32_t, const uint32_t, const mce::ImageFormat, const mce::ImageUsage);
    Image(const mce::Image &);
    Image(mce::Image &&);
    Image(Storage &&);
    ~Image(void);
    mce::Image & operator=(const mce::Image &);
    mce::Image & operator=(mce::Image &&);
    void setRawImage(Storage &&);
    void setImageDescription(const uint32_t, const uint32_t, const mce::ImageFormat, const mce::ImageUsage);
    void copyRawImage(const Storage &);
    mce::Image clone(void) const;
    bool isEmpty(void) const;
    void resizeImageBytesToFitImageDescription(void);

    typedef mce::Blob Storage;
}
