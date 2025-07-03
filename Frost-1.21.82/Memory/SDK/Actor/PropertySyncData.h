#pragma once

struct PropertySyncData {
public:
    struct PropertySyncIntEntry {
        unsigned int mPropertyIndex;
        int          mData;
    };

    struct PropertySyncFloatEntry {
        unsigned int  mPropertyIndex;
        float         mData;
    };

    std::vector<PropertySyncData::PropertySyncIntEntry>   mIntEntries;
    std::vector<PropertySyncData::PropertySyncFloatEntry> mFloatEntries;
};