//
// Created by Flash on 10/15/2023.
//

#pragma once

#include <Prax/Utils/System/Memory.h>
#include <Prax/Utils/System/Internet.h>
#include <Prax/Utils/System/FileSystem.h>
#include <Prax/SDK/Game/Misc/Structs.h>


class ResourceLocation {
public:
    uint64_t type;        //0x0000
    std::string filePath;  //0x0008

    ResourceLocation(std::string filePath, bool external) {
        memset(this, 0, sizeof(ResourceLocation));
        this->filePath = std::move(filePath);
        if (external)
            this->type = 2;
    };
};

class TexturePtr {
private:
    char padding[0x10];
public:
    std::shared_ptr<ResourceLocation> texture;
};

class NinesliceData {
public:
    float x;
    float y;
    float width;
    float height;
    float u;
    float v;
    float uWidth;
    float vHeight;
};

class NinesliceInfo {
public:
    NinesliceData topLeft;
    NinesliceData topRight;
    NinesliceData bottomLeft;
    NinesliceData bottomRight;
    float width;
    float height;
};


class ScreenView {
public:
    BUILD_ACCESS(this, float, deltaTime, 0x4);
};

class HashedString {
public:
    uint64_t hash;
    TextHolder text;
    const HashedString* unk;

    HashedString(uint64_t inputhash, std::string inputtext) {
        memset(this, 0x0, sizeof(HashedString));
        this->hash = inputhash;
        this->text.setText(inputtext);
    }

    HashedString(const std::string& text) {
        memset(this, 0x0, sizeof(HashedString));
        this->text.setText(text);
        this->computeHash();
    }

    void computeHash() {
        hash = 0xCBF29CE484222325i64;
        if (this->text.getTextLength() <= 0)
            return;
        char* textP = this->text.getText();
        auto c = *textP;

        do {
            hash = c ^ 0x100000001B3i64 * hash;
            c = *++textP;
        } while (*textP);
    }

    bool operator==(HashedString& rhs) {
        if (this->text.getText() == rhs.text.getText()) {
            return ((this->unk == &rhs) && (rhs.unk == this));
        }
        return false;
    }
    bool operator!=(HashedString& rhs) {
        return !(*this == rhs);
    }
    bool operator<(HashedString& rhs) {
        if (this->hash < rhs.hash) {
            return true;
        }
        if (this->hash <= rhs.hash) {
            return (strcmp(this->text.getText(), rhs.text.getText()) < 0);
        }
        return false;
    }
};

class UIScene {
public:
    BUILD_ACCESS(this, uintptr_t, vtable, 0x0);
    BUILD_ACCESS_STRING(this, name, 0x28);
};

class MinecraftUIRenderContext {
public:
    BUILD_ACCESS(this, uintptr_t, vtable, 0x0);
    BUILD_ACCESS(this, class ClientInstance*, clientInstance, 0x8);
    BUILD_ACCESS(this, class ScreenContext*, screenContext, 0x10);
    BUILD_ACCESS(this, float, globalAlpha, 0x28);
    BUILD_ACCESS(this, class UIScene*, UIScene, 0xF0);

    virtual void decons(void);
    virtual float getLineLength(class Font* font, class TextHolder* text, float textSize, bool unknown);
    virtual float getTextAlpha();
    virtual void setTextAlpha(float alpha);
    virtual __int64 drawDebugText(const float* pos, class TextHolder* text, float* color, float alpha, unsigned int textAlignment, class TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
    virtual __int64 drawText(class Font* font, struct Rect* position, class TextHolder* text, rgb colour, float alpha, unsigned int textAlignment, TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
    virtual void flushText(float timeSinceLastFlush);
    virtual __int64 drawImage(TexturePtr* texturePtr, Vector2* ImagePos, Vector2* ImageDimension, Vector2* uvPos, Vector2* uvSize);
    virtual __int64 drawNineslice(TexturePtr *texture, const NinesliceInfo *info, int degree);
    virtual void flushImages(rgb color, float opacity, HashedString string);
    virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch);
    virtual void endSharedMeshBatch(uintptr_t ComponentRenderBatch);
    virtual void drawRectangle(Rect position, rgb color, float alpha, int lineWidth);
    virtual void fillRectangle(Rect position, rgb color, float alpha);
    virtual void increaseStencilRef(void);
    virtual void decreaseStencilRef(void);
    virtual void resetStencilRef(void);
    virtual void fillRectangleStencil(Rect const&);
    virtual void enableScissorTest(Rect  const&);
    virtual void disableScissorTest(void);
    virtual void setClippingRectangle(Rect const&);
    virtual void setFullClippingRectangle(void);
    virtual void saveCurrentClippingRectangle(void);
    virtual void restoreSavedClippingRectangle(void);
    virtual int getFullClippingRectangle(void);
    virtual void updateCustom(uintptr_t CustomRenderComponent);
    virtual void renderCustom(uintptr_t CustomRenderComponent, int, Rect&);
    virtual void cleanup(void);
    virtual void removePersistentMeshes(void);
    virtual TexturePtr* getTexture(TexturePtr* ptr, ResourceLocation* location, bool a1);
    virtual int getZippedTexture(uintptr_t Path, uintptr_t ResourceLocation, bool);
    virtual void unloadTexture(uintptr_t ResourceLocation);
    virtual int getUITextureInfo(uintptr_t ResourceLocation, bool);
    virtual void touchTexture(uintptr_t ResourceLocation);
    virtual int getMeasureStrategy(void);
    virtual void snapImageSizeToGrid(Vector2 const&);
    virtual void snapImagePositionToGrid(Vector2 const&);
    virtual void notifyImageEstimate(ULONG);


    TexturePtr* CreateTexture(std::string path) {
        auto resource = ResourceLocation(std::move(path), true);
        TexturePtr* texture = new TexturePtr;
        getTexture(texture, &resource, false);

        return texture;
    }

    void DrawImageFromUrl(const std::string& url, const std::string& name, Vector2* pos, Vector2* size, Vector2* uvPos, Vector2* uvSize) {
        auto path = FileSystem::ResourcesDirectory + "\\" + name;



        if (!FileSystem::FileExists(path)) {
            Internet::DownloadFile(url, path);
        }

        auto texture = CreateTexture(path);

        drawImage(texture, pos, size, uvPos, uvSize);
    }


    void FlushCustomImage() {
        flushImages(rgb(),  1.0f, HashedString(0xA99285D21E94FC80, "ui_flush"));
    }
};