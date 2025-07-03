#pragma once

class BookEditPacket : public ::Packet {
public:
    BookEditAction mAction;     // this+0x30
    int            mBookSlot;   // this+0x34
    int            mPageIndex1; // this+0x38
    int            mPageIndex2; // this+0x3C
    std::string    mText1;      // this+0x40
    std::string    mText2;      // this+0x60
    std::string    mXuid;       // this+0x80
};