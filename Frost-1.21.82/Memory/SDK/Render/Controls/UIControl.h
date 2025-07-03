#pragma once

class UIControl {
public:
private:
    char pad_0x0000[0x18];           // this+0x0000
public:
    char ControlName[16];            // this+0x0018
private:
    char pad_0x0028[0x10];           // this+0x0028
public:
    Vector2<float> Offset;           // this+0x0038
    Vector2<float> Size;             // this+0x0040
    Vector2<float> minSize;          // this+0x0048
    Vector2<float> maxSize;          // this+0x0050
public:
    float Opacity;                   // this+0x0058
private:
    char pad_0x005C[0x1C];           // this+0x005C
public:
    Vector2<float> Position;         // this+0x0078
private:
    char pad_0x0080[0x10];           // this+0x0080
    UIControl* ChildrenRef;          // this+0x0090
public:
    UIControl* Parent;               // this+0x0098
private:
    char pad_0x00A0[0x10];           // this+0x00A0
public:
    std::vector<std::shared_ptr<UIControl>> children; // this+0x00B0
    std::vector<std::unique_ptr<class UIComponent>> components; // this+0x00C0
public:
    //CLASS_FIELD(std::string, LayerName, 0x18);
    //CLASS_FIELD(float, scale, 0x40);
    //CLASS_FIELD(Vector2<float>, Position, 0x78);
    //CLASS_FIELD(std::vector<std::shared_ptr<UIControl>>, children, 0xA0);

    void getAllControls(std::vector<std::shared_ptr<UIControl>>& list) {
        for (auto& control : this->children) {
            list.emplace_back(control);
            control->getAllControls(list);
        }
    }

    void forEachControl(std::function<void(std::shared_ptr<UIControl>&)> func) {

        std::vector<std::shared_ptr<UIControl>> writeList;
        this->getAllControls(writeList);

        for (auto& control : writeList) {
            func(control);
        }
    }

    UIControl* GetChild(int index) {
        return ChildrenRef + (0x150 * index);
    }

    const char* getName() {
        return ControlName;
    }

    bool CmpName(const char* str) {
        const char* src = getName();
        return strcmp(str, src) == 0;
    }
};

/*
public:
    Vector2<float> mCachedPosition;
    bool           : 0;
    bool           mHover : 1;
    bool           mVisible : 1;
    bool           mVisibleInStackPanel : 1;
    bool           mIsVisibleInTree : 1;
    bool           mEnabled : 1;
    bool           mAllAncestorsEnabled : 1;
    bool           mSelected : 1;
    bool           mClipsChildren : 1;
    bool           mAllowClipping : 1;
    bool           mIsClipped : 1;
    bool           mEnableClippingScissorTest : 1;
    bool           : 0;
    bool           mIsTemplate : 1;
    bool           mPropagateAlpha : 1;
    bool           mDelayCollectionUpdate : 1;
    bool           mTextToSpeechTriggered : 1;
    bool           mCachedPositionDirty : 1;
    std::string    mName;                   // this+0x0018
    Vector2<float> mParentRelativePosition; //
    Vector2<float> mOffset;                 // this+0x0038
    Vector2<float> mSize;                   // this+0x0040
    Vector2<float> mMinSize;                // this+0x0048
    Vector2<float> mMaxSize;                // this+0x0050
    float          mOpacity;                // this+0x0058
private:
    char pad_0x005C[0x1C];                  // this+0x005C
public:
    Vector2<float> mPosition;               // this+0x0078
private:
    char pad_0x0080[0x10];                  // this+0x0080
    UIControl* ChildrenRef;                 // this+0x0090
public:
    UIControl* Parent;                      // this+0x0098
private:
    char pad_0x00A0[0x10];                  // this+0x00A0
public:
    std::vector<std::shared_ptr<UIControl>> mChildren; // this+0x00B0
    std::vector<std::unique_ptr<class UIComponent>> mComponents; // this+0x00C0
public:
*/