#pragma once
#include "../TextHolder.h"
#include "../Math/UIColor.h"

class ComponentRenderBatch;
class NinesliceInfo;
class HashedString;
//class TextMeasureData;
//class CaretMeasureData;
class TexturePtr;

class TextureData
{
private:
    char pad_0x0[0x18];
    struct PathStruct
    {
        char pad_0x0[0x18];
        TextHolder filePath;
    }*ptrToPath;
    char pad_0x0038[0x20];

public:
    TextHolder GetFilePath() const
    {
        return ptrToPath->filePath;
    }
};

struct TextMeasureData
{ // Stores information about how the text should be displayed
public:
    TextMeasureData(float size, bool showShadow)
    {
        this->textSize = size;
        this->showShadow = showShadow;
        this->id = 0;
        this->inObject = false;
    };

public:
    float textSize;
    int id;
    bool showShadow;
    bool inObject;
};

struct ResourceLocation {
private:
    uint64_t type;        //0x0000
    TextHolder filePath;  //0x0008
public:
    ResourceLocation(std::string filePath, bool external) {
        memset(this, 0, sizeof(ResourceLocation));
        this->filePath.setText(filePath);
        if (external)
            this->type = 2;
    };
};

struct CaretMeasureData
{
    int Position;
    bool isSingleline;// false|insert

    CaretMeasureData()
    {
        CaretMeasureData(0xFFFFFFFF, true);
    };

    CaretMeasureData(int position, bool singlelines)
    {
        this->Position = position;
        this->isSingleline = singlelines;
    };
};


class MinecraftUIRenderContext {
public:
    void* ClientInstance;

private:
    virtual void Destructor();
public:
    virtual float getLineLength(void* font, TextHolder* str, float measureCalculation, bool calculateWidth);
private:
    virtual int getTextAlpha();
    virtual void setTextAlpha();
    virtual void drawDebugText();
public:
    virtual void drawText(void* font, Vector4<float> const& pos, TextHolder* str, UIColor const& colour, float idk, float alinM, TextMeasureData const& textdata, CaretMeasureData const& caretdata);
    virtual void flushText(float flushDelta);
private:
    __int64 drawImage(const TextureData* a2, Vector2<float> const& ImagePos, Vector2<float> const& ImageDimension, Vector2<float> const& uvPos, Vector2<float> const& uvSize)
    {
        return CallVFunc<7, __int64, const TextureData*, Vector2<float> const&, Vector2<float> const&, Vector2<float> const&, Vector2<float> const&>(this, a2, ImagePos, ImageDimension, uvPos, uvSize);
    };
    //virtual void drawImage();
    virtual void drawNineslice();
    virtual void flushImages();
    virtual void beginSharedMeshBatch();
    virtual void endSharedMeshBatch();
public:
    virtual void drawRectangle(Vector4<float> const& rect, UIColor const& colour, float alpha, int width);
    virtual void fillRectangle(Vector4<float> const& rect, UIColor const& colour, float alpha);
};