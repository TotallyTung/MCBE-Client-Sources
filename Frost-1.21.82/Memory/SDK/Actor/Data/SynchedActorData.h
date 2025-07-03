#pragma once

class SynchedActorData {
public:
    std::vector<std::unique_ptr<class DataItem>> mItemsArray;

    char filler[0x30];
};