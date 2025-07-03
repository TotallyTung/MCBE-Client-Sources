#pragma once
bool fluxswing = false;

class Animations : public Module
{
public:
    Animations(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Animations", "Render", "Change how you swing", keybind, enabled)
    {
        registerEnumSetting("Animation", "Change your swing speed", { "1.7", "Slide", "Spin", "Exhi", "Lunaris", Game::Skiddle::ClientName.c_str() }, &mode);
        registerBoolSetting("Flux", "Change animations swing", &fluxswing);
        registerFloatSetting("Swing Speed", "Change your swing speed", &Game::Core::AnimationSwingSpeed, 1, 50);

        // ItemModel
        registerBoolSetting("Reset", "unko", &reset);


        registerFloatSetting("PosX", "Changes bobhurt position", &viewhurtItemX, -4, 4);
        registerFloatSetting("PosY", "Changes bobhurt position", &viewhurtItemY, -4, 4);
        registerFloatSetting("PosZ", "Changes bobhurt position", &viewhurtItemZ, -4, 4);

        registerFloatSetting("RotX", "Changes bobhurt rotation", &viewhurtItemRotX, 0, 360);
        registerFloatSetting("RotY", "Changes bobhurt rotation", &viewhurtItemRotY, 0, 360);
        registerFloatSetting("RotZ", "Changes bobhurt rotation", &viewhurtItemRotZ, 0, 360);

        registerFloatSetting("Rot Angle", "Changes bobhurt rotation angle", &viewhurtItemRotAngle, 0, 360);
    }

    int mode = 0;

    bool flux = true;
    bool reset = false;


    // Item Model
    float viewhurtItemX = 0.f;
    float viewhurtItemY = 0.f;
    float viewhurtItemZ = 0.f;

    float viewhurtItemRotX = 0;
    float viewhurtItemRotY = 0;
    float viewhurtItemRotZ = 0;

    float viewhurtItemRotAngle = 0;

    void onDisabled() override
    {
        Game::Core::SwingSpeed = 6;
    }

    void blockRotate(glm::mat4& matrix, float upper) {
        float floatY = -1.30F;
        matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, upper, -0.20F));
        matrix = glm::rotate<float>(matrix, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, -floatY, glm::vec3(4.0F, 0.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    }

    void slideBlockRotate(glm::mat4& matrix, float upper) {
        matrix = glm::translate<float>(matrix, glm::vec3(-0.5F, upper, 0.0F));
        matrix = glm::rotate<float>(matrix, 30.0F, glm::vec3(0.0F, 1.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, -80.0F, glm::vec3(1.0F, 0.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    }

    void ExhiRotate(glm::mat4& matrix, float upper) {
        float floatY = -1.30F;
        matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, upper, -0.20F));
        matrix = glm::rotate<float>(matrix, -1.69F, glm::vec3(0.0F, 1.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, -floatY, glm::vec3(4.0F, 0.0F, 2.0F));
        matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    }

    void LunarisBlockRotate(glm::mat4& matrix, float upper) {
        float floatY = -1.30F;
        matrix = glm::translate<float>(matrix, glm::vec3(-0.26F, upper, -0.27F));
        matrix = glm::rotate<float>(matrix, -2.79F, glm::vec3(0.0F, -1.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, -floatY, glm::vec3(7.2F, 4.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.6F));
    }

    void LunaciaRotate(glm::mat4& matrix, float upper) {
        float floatY = -1.1F;
        matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, upper, -0.20F));
        matrix = glm::rotate<float>(matrix, -1.69F, glm::vec3(0.0F, 1.0F, 0.0F));
        matrix = glm::rotate<float>(matrix, -floatY, glm::vec3(4.0F, 0.0F, 2.0F));
        matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    }

    void onEvent(ViewBobTickEvent* event) override
    {
        glm::mat4& matrix = *event->Matrix;
        glm::mat4 View = matrix;
        matrix = View;
        float lerpT = 1.f;
        float degrees = 1.f;

        auto pos = Game::GetInstance()->getLevelRender()->getOrigin();
        auto player = Game::GetLocalPlayer();
        
        if (GetAsyncKeyState(VK_RBUTTON) && Game::GetInstance()->mcGame->canUseMoveKeys() || Game::Skiddle::ShouldBlock) // RenderUtil::GetCTX()->ClientInstance->mcGame->CanUseKeys
        {
            nopBytes((unsigned char*)fluxSwing, 6);

            switch (mode) {
            case 0: // 1.7
                matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.1f, 0.15f, -0.2f));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, 0.25f, -0.20F));
                matrix = glm::rotate<float>(matrix, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 1.30F, glm::vec3(4.0F, 0.0F, 0.0F));
                matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
                break;
            case 1: // Slide
                matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.15f, 0.15f, -0.2f));
                slideBlockRotate(matrix, 0.3f);
                break;
            case 2: // Spin
                degrees = fmodf(player->getRenderPositionComponent()->renderPos.lerp(player->GetPosition(), lerpT, lerpT, lerpT).x, 5) - 2.5f;
                degrees *= 180 / 2.5f;
                matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(0, 0, 1));
                break;
            case 3: // Exhi
                lerpT = 0.f;
                matrix = glm::translate<float>(matrix, glm::vec3(0.72222223281, -0.2, -0.66666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.0f, 0.45f, -0.4f));
                ExhiRotate(matrix, 0.25f);
                break;
            case 4: // Lunaris
                matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.15f, -0.10f, -0.9f));
                LunarisBlockRotate(matrix, 0.25f);
                break;
            case 5: // Lunacia
                lerpT = 0.f;
                matrix = glm::translate<float>(matrix, glm::vec3(0.72222223281, 0.0, -0.66666666269302368));
                matrix = glm::translate<float>(matrix, glm::vec3(-0.0f, 0.45f, -0.4f));
                LunaciaRotate(matrix, 0.2f);
                break;
            }
        }
        else
        {
            patchBytes((unsigned char*)((uintptr_t)fluxSwing), (unsigned char*)"\x0F\x84\xBD\x04\x00\x00", 6);

            if (Game::GetInstance()->mcGame->canUseMoveKeys()) {
                switch (mode) {
                case 2: // Spin
                    degrees = fmodf(player->getRenderPositionComponent()->renderPos.lerp(player->GetPosition(), lerpT, lerpT, lerpT).x, 5) - 2.5f;
                    degrees *= 180 / 2.5f;
                    matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(0, 0, 1));
                    break;
                }
            }
        }

        // Item Model
        matrix = glm::translate<float>(matrix, glm::vec3(viewhurtItemX, viewhurtItemY, viewhurtItemZ));

        if (viewhurtItemRotAngle != 0 || viewhurtItemRotX != 0 || viewhurtItemRotY != 0 || viewhurtItemRotZ != 0)
        {
            matrix = glm::rotate<float>(matrix, viewhurtItemRotAngle, glm::vec3(viewhurtItemRotX, viewhurtItemRotY, viewhurtItemRotZ));
        }
    }

    void onEvent(UpdateEvent* event) {
        Game::Core::SwingSpeed = Game::Core::AnimationSwingSpeed;

        if (reset) {
            viewhurtItemX = 0.f;
            viewhurtItemY = 0.f;
            viewhurtItemZ = 0.f;

            viewhurtItemRotAngle = 0;
            viewhurtItemRotX = 0;
            viewhurtItemRotY = 0;
            viewhurtItemRotZ = 0;

            reset = false;
        }
    }
};