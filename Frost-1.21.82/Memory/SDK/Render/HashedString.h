#pragma once

class HashedString {
public:
    uint64_t            mStrHash;   // this+0x0
    std::string         mStr;       // this+0x8
    const HashedString* mLastMatch; // this+0x28

    HashedString(uint64_t inputhash, std::string inputtext) {
        //memset(this, 0x0, sizeof(HashedString));
        this->mStrHash = inputhash;
        this->mStr = inputtext;
    }

    HashedString(const std::string& text) {
        //memset(this, 0x0, sizeof(HashedString));
        this->mStr = text;
        this->computeHash();
    }

    HashedString() {
        //memset(this, 0x0, sizeof(HashedString));
    }

    void setText(std::string text) {
        this->mStr = text;
        this->computeHash();
    }

    void computeHash() {
        mStrHash = 0xCBF29CE484222325i64;
        if (this->mStr.length() <= 0)
            return;
        char* textP = this->mStr.data();
        auto c = *textP;

        do {
            mStrHash = c ^ 0x100000001B3i64 * mStrHash;
            c = *++textP;
        } while (*textP);
    }

    bool operator==(HashedString& rhs) {
        if (this->mStr == rhs.mStr) {
            return ((this->mLastMatch == &rhs) && (rhs.mLastMatch == this));
        }
        return false;
    }
    bool operator!=(HashedString& rhs) {
        return !(*this == rhs);
    }
    bool operator<(HashedString& rhs) {
        if (this->mStrHash < rhs.mStrHash) {
            return true;
        }
        if (this->mStrHash <= rhs.mStrHash) {
            return (strcmp(this->mStr.data(), rhs.mStr.data()) < 0);
        }
        return false;
    }
};