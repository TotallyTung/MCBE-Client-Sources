#pragma once

DEFINE_NOP_PATCH_FUNC(patchFluxSwing, (uintptr_t)SigManager::FluxSwing, 0x5);

class Animations : public Module
{
public:
    Animations() :
        Module("Animations", "Visual", "Modifies how your first person viewmodel moves.")
    {
        EnumSetting("Swing", "The swing type.", { "Flux", "None" }, &swingType);
        EnumSetting("Block", "The block animation type.", { "None", "1.7", "Exhi" }, &blockType);
        BoolSetting("No Slot Slide", "Disables switch animations.", &noSlotSlide);
        SliderSetting("Swing Speed", "The custom swing speed value.", &mSwingSpeed, 1, 90, SliderType::Int);
        BoolSetting("Swing Angle", "Changes the swing angle.", &CustomSwingAngle);
        BoolSetting("Change on Block", "Only changes the swing angle on block.", &onBlockCustomSwing, [this] { return CustomSwingAngle; });
        SliderSetting("Angle", "The custom swing angle value.", &SwingAngleSet, -360, 360, SliderType::Float, [this] { return CustomSwingAngle; });
    }

    int swingType = 0;
    int blockType = 1;

    enum BlockType
    {
        None = 0,
        Java = 1,
        Exhi = 2,
    };

    bool noSlotSlide = true;
    bool CustomSwingAngle = true;
    bool onBlockCustomSwing = true;
    float SwingAngleSet = 12;

    float mSwingSpeed = 20;

    float* mSwingAngle = nullptr;

    void onEnabled() {
        if (!mSwingAngle) {
            mSwingAngle = reinterpret_cast<float*>(SigManager::TapSwingAnim);
            Memory::setProtection(reinterpret_cast<uintptr_t>(mSwingAngle), sizeof(float), PAGE_READWRITE);
        }

        patchFluxSwing(swingType == 0);
    }

    void onDisabled() {
        patchFluxSwing(false);

        if (mSwingAngle) *mSwingAngle = glm::radians(-80.f);
    }

    void onEvent(BaseTickEvent* event) {
        patchFluxSwing(swingType == 0);

        if (onBlockCustomSwing) {
            if (GetAsyncKeyState(VK_RBUTTON) && InstanceManager::isAllowedToUseKeys() || GlobalVariables::mShouldBlock) {
                *mSwingAngle = glm::radians(CustomSwingAngle ? SwingAngleSet : -80.f);
            }
            else {
                *mSwingAngle = glm::radians(-80.f);
            }
        }
        else {
            *mSwingAngle = glm::radians(CustomSwingAngle ? SwingAngleSet : -80.f);
        }
    }

    void onEvent(BobHurtEvent* event) {
        glm::mat4& matrix = *event->mMatrix;

        if (GetAsyncKeyState(VK_RBUTTON) && InstanceManager::isAllowedToUseKeys() || GlobalVariables::mShouldBlock)
        {
            if (blockType == BlockType::Java) {
                matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.1f, 0.15f, -0.2f));

                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.25f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.30F, glm::vec3(4.0F, 0.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
            }

            if (blockType == BlockType::Exhi) {
                matrix = glm::translate<float>(matrix, glm::vec3(0.72222223281, -0.2, -0.66666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.0f, 0.45f, -0.4f));

                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.2f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.69F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.30F, glm::vec3(4.0F, 0.0F, 2.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
            }
        }
    }
};