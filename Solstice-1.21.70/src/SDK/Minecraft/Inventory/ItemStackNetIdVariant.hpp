//
// Created by vastrakai on 11/19/2024.
//

#pragma once

class ItemStackNetIdVariant {
public:
    int id1;
    PAD(4)
    int id2;
    PAD(4)
    uint8_t type;
    PAD(7)

    ItemStackNetIdVariant() {
        memset(this, 0, sizeof(ItemStackNetIdVariant));
    }

    __forceinline int getID() {
        if (type == 0) return id1;
        else if (type <= 2) return id2;
        else return 0;
    }

    __forceinline void setID(int id) {
        if (type == 0) id1 = id;
        else if (type <= 2) id2 = id;
    }

    ItemStackNetIdVariant(int id) {
        memset(this, 0, sizeof(ItemStackNetIdVariant));
        setID(id);
    }
};

