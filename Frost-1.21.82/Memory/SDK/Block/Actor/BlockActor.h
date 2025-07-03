#pragma once

class Tag
{
public:
    enum class Type : uint8_t {
        End,
        Byte,
        Short,
        Int,
        Int64,
        Float,
        Double,
        ByteArray,
        String,
        List,
        Compound,
        IntArray,
    };

    virtual ~Tag();
    virtual void deleteChildren();
    virtual void Func2();
    virtual void load(class IDataInput&);
    virtual std::string toString();
    virtual Type getId();
    virtual bool equals(const Tag&) const;
    virtual void print(class PrintStream&) const;
    virtual void print(std::string const&, class PrintStream&) const;
    virtual std::unique_ptr<Tag> copy() const = 0;
    virtual void Func10();
};

class CompoundTagVariant
{
    char pad_0000[40]; //0x0000
    Tag::Type type; //0x0028
    char pad_002C[7]; //0x002C
public:
    inline Tag::Type getTagType() {
        return this->type;
    }
    class StringTag : public Tag
    {
    public:
        std::string val;
    };
    inline StringTag* asStringTag() {
        return (StringTag*)this;
    }
};


class CompoundTag {
public:
    std::map<std::string, CompoundTagVariant> data;

    virtual ~CompoundTag();
    virtual void deleteChildren(void);
    virtual void write(class IDataOutput&);
    virtual void load(class IDataInput&);
    virtual std::string toString(void);
    virtual void getId(void);
    virtual void equals(class Tag const&);
    virtual void print(class PrintStream&);
    virtual void print(std::string const&, class PrintStream&);
    virtual void copy(void);
    virtual void hash(void);
};

class BlockActor {
private:
	virtual void Destructor(); // BlockActor Destructor
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};