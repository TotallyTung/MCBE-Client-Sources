#pragma once

class DataItem {
public:
    ::DataItemType mType;
    ::ActorDataIDs mId;
    bool           mDirty = true;

    DataItem() = delete;
protected:
    constexpr DataItem(::DataItemType type, ::ActorDataIDs id) : mType(type), mId(id) {}

public:
    // NOLINTBEGIN
    // vIndex: 0
    virtual ~DataItem();

    // vIndex: 1
    virtual bool isDataEqual(class DataItem const&) const;

    // vIndex: 2
    virtual std::unique_ptr<class DataItem> clone() const = 0;

    // NOLINTEND
};