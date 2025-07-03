//
// Created by vastrakai on 7/5/2024.
//

#pragma once

#include <string>
#include <map>
#include <memory>

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
    virtual void unknown();
    virtual void load(class IDataInput&);
    virtual std::string toString();
    virtual Type getId();
    virtual bool equals(const Tag&) const;
    virtual void print(class PrintStream&) const;
    virtual void print(std::string const&, class PrintStream&) const;
    virtual std::unique_ptr<Tag> copy() const = 0;
    virtual void unknown2();
};

class EndTag : public Tag
{

};

class ByteTag : public Tag
{
public:
    unsigned char val;
};

class ShortTag : public Tag
{
public:
    short val;
};

class IntTag : public Tag
{
public:
    int val;
};

class Int64Tag : public Tag
{
public:
    int64_t val;
};

class FloatTag : public Tag
{
public:
    float val;
};

class DoubleTag : public Tag
{
public:
    double val;
};

class StringTag : public Tag
{
public:
    std::string val;
};

class ListTag : public Tag
{
public:
    std::vector<Tag*> val;
    Tag::Type elementType;
};

struct TagMemoryChunk
{
    size_t capacity = 0;
    size_t size = 0;
    std::unique_ptr<char[]> data;
};

class ByteArrayTag : public Tag
{
public:
    TagMemoryChunk val;
};

class IntArrayTag : public Tag
{
public:
    TagMemoryChunk val;
};

class CompoundTag;
#pragma pack(push, 1)
class CompoundTagVariant
{
public:
    char pad_0000[40]; //0x0000
    Tag::Type type; //0x0028
    char pad_002C[7]; //0x002C
    Tag::Type getTagType() {
        return this->type;
    }

    Tag* asTag() {
        return reinterpret_cast<Tag*>(this);
    }

    EndTag* asEndTag() {
        return reinterpret_cast<EndTag*>(this);
    }

    ByteTag* asByteTag() {
        return reinterpret_cast<ByteTag*>(this);
    }

    ShortTag* asShortTag() {
        return reinterpret_cast<ShortTag*>(this);
    }

    IntTag* asIntTag() {
        return reinterpret_cast<IntTag*>(this);
    }

    Int64Tag* asInt64Tag() {
        return reinterpret_cast<Int64Tag*>(this);
    }

    FloatTag* asFloatTag() {
        return reinterpret_cast<FloatTag*>(this);
    }

    DoubleTag* asDoubleTag() {
        return reinterpret_cast<DoubleTag*>(this);
    }

    StringTag* asStringTag() {
        return reinterpret_cast<StringTag*>(this);
    }

    ListTag* asListTag() {
        return reinterpret_cast<ListTag*>(this);
    }

    CompoundTag* asCompoundTag() {
        return reinterpret_cast<CompoundTag*>(this);
    }

    ByteArrayTag* asByteArrayTag() {
        return reinterpret_cast<ByteArrayTag*>(this);
    }

    IntArrayTag* asIntArrayTag() {
        return reinterpret_cast<IntArrayTag*>(this);
    }

    std::string toString() {
        switch (this->type) {
        case Tag::Type::End:
            return "EndTag";
        case Tag::Type::Byte:
            return "ByteTag: " + std::to_string(this->asByteTag()->val);
        case Tag::Type::Short:
            return "ShortTag: " + std::to_string(this->asShortTag()->val);
        case Tag::Type::Int:
            return "IntTag: " + std::to_string(this->asIntTag()->val);
        case Tag::Type::Int64:
            return "Int64Tag: " + std::to_string(this->asInt64Tag()->val);
        case Tag::Type::Float:
            return "FloatTag: " + std::to_string(this->asFloatTag()->val);
        case Tag::Type::Double:
            return "DoubleTag: " + std::to_string(this->asDoubleTag()->val);
        case Tag::Type::String:
            return "StringTag: " + this->asStringTag()->val;
        case Tag::Type::List:
            return "ListTag (size: " + std::to_string(this->asListTag()->val.size()) + ")";
        case Tag::Type::Compound:
            return "CompoundTag";
        case Tag::Type::ByteArray:
            return "ByteArrayTag (size: " + std::to_string(this->asByteArrayTag()->val.size) + ")";
        case Tag::Type::IntArray:
            return "IntArrayTag (size: " + std::to_string(this->asIntArrayTag()->val.size) + ")";
        default:
            return "UnknownTag";
        }
    }
};
#pragma pack(pop)

static_assert(sizeof(CompoundTagVariant) == 0x30);


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
    // get
    CompoundTagVariant* get(const std::string& key) {
        auto it = data.find(key);
        if (it == data.end()) return nullptr;
        return &it->second;
    }
};
