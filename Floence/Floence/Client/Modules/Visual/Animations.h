#pragma once

class Animations : public Module
{
public:
    Animations(int keybind = 7, bool enabled = false) :
        Module("ItemAnimations", "Visual", "Animations like 5x30.", keybind, enabled)
    {
        addEnum("Swing", "The Swing type", { "1.7", "None", "Slide", "Sigma" }, &BlockType);
//        addEnum("Swing", "The Swing type", { "None", "Noswing(client)"}, &Global::SwingType);
        addSlider("Swing Speed", "The swing speed", &Global::swingSpeed, 1, 70);
//        addBool("CustomSwingAngle", "Changes the swing angle", &CustomSwingAngle);
        addSlider("Swing Angle", "The custom swing angle value", &SwingAngleValue, -360, 360);
        addBool("Aura sneak", "In aura only", &AuraOnly);
        addSlider("Delay", "Delay to wait before sneak", &delay, 0.f, 500);
    }

    int BlockType = 0;
//    bool CustomSwingAngle = false;
    bool isSword = false;
    float SwingAngleValue = -80;
    bool AuraOnly = false;
    float delay = 150.f;

    //void* fluxSwingAddr = (void*)(Memory::findSig("E8 ? ? ? ? F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? 48 8B ? F3 0F ? ? 48 8B"));
    void* fluxSwingAddr = (void*)(Memory::findSig("E8 ? ? ? ? 48 8B 06 F3 0F 10 1D"));
    float* tapSwingVal = nullptr;
    void* ogBytes[5] = { 0 };

    void patchAnimFunc(bool patch = true)
    {
        //static uintptr_t addr = (uintptr_t)Memory::findSig("F3 0F ? ? EB ? 0F 57 ? F3 41");
        static uintptr_t addr = (uintptr_t)Memory::findSig("F3 0F 10 05 ? ? ? ? 48 8B 0F F3 41 0F 5C D1");
        if (addr == 0) return;

        std::vector<uint8_t> bytes = {};

        if (patch) {
            bytes = { 0x90, 0x90, 0x90, 0x90 };
        }
        else {
            bytes = { 0xF3, 0x0F, 0x5C, 0xD0 };
        }

        Memory::writeBytes(addr, bytes, (int)bytes.size());
    }

    void onEnabled() override {

        if (!Address::getClientInstance())
            return;

        if (tapSwingVal == nullptr) {
            //tapSwingVal = reinterpret_cast<float*>(Memory::getXref((uintptr_t)Memory::findSig("F3 44 ? ? ? ? ? ? ? 41 0F ? ? F3 0F ? ? ? ? C6 40 38 ? 48 8B ? ? 41 0F ? ? E8 ? ? ? ? 48 8B"), 5));
            tapSwingVal = reinterpret_cast<float*>(Memory::getXref((uintptr_t)Memory::findSig("F3 44 0F 59 05 ? ? ? ? 4C 8D 4C 24 ? 48 8B D0"), 5));
            Memory::setProtection(reinterpret_cast<uintptr_t>(tapSwingVal), 4, PAGE_READWRITE);
        }

        Memory::readBytes((void*)fluxSwingAddr, ogBytes, 5);

        if (BlockType == 0) {
            if ((GetAsyncKeyState(VK_RBUTTON) || Global::ShouldBlock) && Address::canUseKeys() && isSword) // RenderUtil::GetCTX()->ClientInstance->mcGame->CanUseKeys
            Memory::writeBytes((uintptr_t)fluxSwingAddr, "\x90\x90\x90\x90\x90", 5);
        }

        //patchAnimFunc(false);

    }

    void slideBlockRotate(glm::mat4& matrix, float upper) {
        matrix = glm::translate<float>(matrix, glm::vec3(-0.5F, upper, 0.0F));
        matrix = glm::rotate<float>(matrix, 30.0F, glm::vec3(0.0F, 1.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, -80.0F, glm::vec3(1.0F, 0.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    }

    void onEvent(ActorBaseTickEvent* event) override
    {
        PlayerInventory* playerInventory = Address::getLocalPlayer()->getSupplies();
        Inventory* inventory = playerInventory->inventory;
        ItemStack* stack = inventory->getItem(playerInventory->hotbarSlot);
        if (stack->item == nullptr) return;
        if (stack->getItem()->isSword()) isSword = true;
        else isSword = false;

        bool hm = (getModuleByName("aura")->isEnabled() && Global::isAttacking);
        if (AuraOnly && hm) {
            static auto lastTime = std::chrono::high_resolution_clock::now();
            auto currentTime = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() < delay) {
            }
            else {
                //Address::getLocalPlayer()->setSneaking(true);
                lastTime = std::chrono::high_resolution_clock::now();
            }
        }
    }

    void onEvent(ViewBobbingTickEvent* event) override
    {
        float lerpT = 1.f;
        glm::mat4& matrix = *event->Matrix;

        if (BlockType == 0,2,3,4,5) {
            bool hm = ((GetAsyncKeyState(VK_RBUTTON) || Global::ShouldBlock) && Address::canUseKeys() && isSword); // RenderUtil::GetCTX()->ClientInstance->mcGame->CanUseKeys
            if (hm) {
                //Memory::writeBytes((uintptr_t)fluxSwingAddr, "\x90\x90\x90\x90\x90", 5);
            }
            else {
                //Memory::writeBytes((uintptr_t)fluxSwingAddr, (char*)ogBytes, 5);
            }
        }
        if (BlockType == 1) {
            //Memory::writeBytes((uintptr_t)fluxSwingAddr, (char*)ogBytes, 5);
        }

        /*
        if (tapSwingVal)
            *tapSwingVal = SwingAngleValue;
            */

        if ((GetAsyncKeyState(VK_RBUTTON) || Global::ShouldBlock) && Address::canUseKeys() && isSword) // RenderUtil::GetCTX()->ClientInstance->mcGame->CanUseKeys
        {
            switch (BlockType) {
            case 1: // None
                break;
            case 0: // Flux
                matrix = glm::translate<float>(matrix, glm::vec3(0.32222223281, 0.1, 0.0));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.1f, 0.15f, -0.2f));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.25f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.85F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.375F, glm::vec3(4.0F, 0.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
                break;
            case 2: // Slide
                matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.15f, 0.15f, -0.2f));
                slideBlockRotate(matrix, 0.2f);
                break;
            case 3: // Sigma
                matrix = glm::translate<float>(matrix, glm::vec3(0.32222223281, -0.017, 0.025));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.1f, 0.15f, -0.2f));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.25f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.85F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.375F, glm::vec3(4.0F, 0.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
                break;
            case 4: // Exhi
                matrix = glm::translate<float>(matrix, glm::vec3(0.72222223281, -0.2, -0.66666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.0f, 0.45f, -0.4f));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.2f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.69F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.30F, glm::vec3(4.0F, 0.0F, 2.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
            case 5: // Lunaris
                matrix = glm::translate<float>(matrix, glm::vec3(0, -0.2, 0));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.0f, 0.45f, -0.4f));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.2f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.69F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.30F, glm::vec3(4.0F, 0.0F, 2.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
        }
        }

        else {
//            Utils::patchBytes((BYTE*)((uintptr_t)tapSwingVal), (BYTE*)"\xF3\x0F\x51\xF0", 4);
            if (tapSwingVal)
                *tapSwingVal = -80.f;
        }

//        patchAnimFunc(false);
    }

    void onDisabled() override {
        if (!Address::getClientInstance())
            return;

        if (!Address::getClientInstance()->getTimerClass())
            return;

//        if (BlockType == 0) {
            Memory::writeBytes((uintptr_t)fluxSwingAddr, (char*)ogBytes, 5);
//        }

        patchAnimFunc(false);

        *tapSwingVal = -80.f;
    }



 //   std::string getModeName() override {
 //       return " " + std::string("Flux");
//    }
};