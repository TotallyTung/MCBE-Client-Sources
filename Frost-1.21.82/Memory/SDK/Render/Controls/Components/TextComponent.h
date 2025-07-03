#pragma once

class TextComponent : public UIComponent {
public:
    std::byte padding16[288]; //  this+0x010
    std::string       mLabel; //  this+0x130       

    // 1.20.71.1 - 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D B9 ? ? ? ? 48 8B DA 48 83 7F
    // 1.21.50 - 48 89 5C 24 ? 48 89 6C 24 ? 57 48 83 EC 30 48 8D 99 ? ? ? ? 48 8B E9 48 8B 43 18 48 8B FA
    void setText(const std::string& text) {
        static void* signature = SigManager::TextComponent_setText;
        Memory::CallFunc<void, void*, std::string>(signature, this, text);
    }
};