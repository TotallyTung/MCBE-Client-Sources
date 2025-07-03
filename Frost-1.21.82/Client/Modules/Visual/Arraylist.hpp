#pragma once

class ArrayList : public Module
{
public:
    ArrayList() :
        Module("ArrayList", "Visual", "Shows your enabled modules on the screen.", Keys::NONE, true)
    {
        EnumSetting("Style", "Change the look of the array list.", { "Outline", "Flux", "Simple", "None" }, &mStyle);
        EnumSetting("Animation", "The animation of enabling and disabling", { "Lerp", "Expo", "Bounce", "Elastic" }, &mAnimationMode);
        SliderSetting("Offset", "The offset of the array list.", &mOffset, 0, 50);
        SliderSetting("Text Size", "The text size of the arraylis.t", &mTextSize, 0.8f, 1.5f, SliderType::DoubleFloat);
        BoolSetting("Text Shadow", "Render shadows for the texts", &mTextShadow);
        BoolSetting("Fill Shadow", "Fill the rectangle with shadow", &mShadowFilled);
        SliderSetting("Shadow strenght", "The strenght of the shadow", &mShadowIterations, 0, 200);
        SliderSetting("Shadow opacity", "The opacity of the shadow", &mShadowOpacity, 0, 100);
        SliderSetting("Bar opacity", "The opacity of the bar shadow", &mBarOpacity, 0, 100);
        SliderSetting("Opacity", "The opacity of the arraylist", &mOpacity, 0, 100);
        BoolSetting("Modes", "Display the array list modes.", &mDisplayMode);
    }

private:
    int mStyle = 0;
    int mAnimationMode = 0;
    float mTextSize = 1.2f;
    float mOpacity = 0.15f;
    float mOffset = 0;
    bool mTextShadow = true;
    bool mShadowFilled = true;
    float mShadowIterations = 100;
    float mShadowOpacity = 50; // divide by 100
    float mBarOpacity = 100; // divide my 100
    bool mDisplayMode = true;
    bool mSortLength = true;

    enum Style
    {
        Outline = 0,
        Flux    = 1,
        Simple  = 3
    };
    
    enum AnimationMode
    {
        Lerp    = 0,
        Expo    = 1,
        Bounce  = 3,
        Elastic = 4
    };
    
    //bool blur = true;
    //static inline float blurStrength = 9;
    //static inline float padding = 10.5;
   

    
public:
    void onEvent(ImRenderEvent* event) override
    {
        if (!ImGui::GetBackgroundDrawList()) return;
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        static std::vector<std::shared_ptr<Module>> sortedModules;
        static std::vector<std::shared_ptr<Module>> currentModules;

        sortedModules.clear();
        for (const auto& mod : mModules) {
            if ((!mod->isEnabled() && mod->mAnimationPercentage <= 0.01f) || !mod->isVisible()) continue;
            sortedModules.push_back(mod);
        }

        if (mSortLength) {
            sortModules(sortedModules);
        }
        else {
            sortModulesAlphabetically(sortedModules);
        }

        currentModules = sortedModules;

        const float mTextHeight = ImRenderUtil::getTextHeight(mTextSize);
        const float mPadding = 0.5f * mTextSize;
        float mTextPosY = -1 + mOffset;
        int mIndex = 0;

        Vector4<float> mUnderline;
        float mTextWid = 0;
        float mEaseAnim = 0;
        float mBarOffsetLen = mStyle == 1 ? 3.f : 0;

        float mGetOpacity = mOpacity / 100;
        float mGetShadowOpacity = mShadowOpacity / 100;
        float mGetBarOpacity = mBarOpacity / 100;

        float mAnimationSpeed = mAnimationMode == 0 ? 10 : 10;

        std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float, float>> mShadowDraw;

        for (auto& mod : currentModules) {
            float targetAnim = mod->isEnabled() ? 1.f : 0.f;
            mod->mAnimationPercentage = MathUtil::animate(targetAnim, mod->mAnimationPercentage, ImRenderUtil::getDeltaTime() * mAnimationSpeed);
            mod->mAnimationPercentage = MathUtil::clamp(mod->mAnimationPercentage, 0.f, 1.f);

            switch (mAnimationMode) {
            case AnimationMode::Lerp:
                mEaseAnim = mod->mAnimationPercentage;
                break;
            case AnimationMode::Expo:
                mEaseAnim = easeInOutExpo(mod->mAnimationPercentage);
                break;
            case AnimationMode::Bounce:
                mEaseAnim = easeInOutBounce(mod->mAnimationPercentage);
                break;
            case AnimationMode::Elastic:
                mEaseAnim = easeInOutElastic(mod->mAnimationPercentage);
                break;
            }

            std::string mModuleName = mod->getName();
            std::string mModuleMode = " " + mod->getModeName();
            float modWidth = ImRenderUtil::getTextWidth(&mModuleName, mTextSize);
            float modeWidth = !mModuleMode.empty() ? ImRenderUtil::getTextWidth(&mModuleMode, mTextSize) : 0.f;

            if (!mDisplayMode) {
                modeWidth = 0;
            }

            float modulePosX = ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - mPadding - mBarOffsetLen - mOffset;
            mod->mArrayListPos.x = MathUtil::lerp(ImGui::GetIO().DisplaySize.x + 5.f, modulePosX, mEaseAnim);
            mod->mArrayListPos.y = mTextPosY;

            Color arrayColor = ColorUtil::getClientColor(1.8, 1, 1, mIndex * 40);

            Vector4<float> boxRect(mod->mArrayListPos.x - mPadding - 2.f, mTextPosY,
                (mod->mArrayListPos.x - mPadding - 2.f) + modWidth + modeWidth + 5,
                mTextPosY + mTextHeight);

            Vector4<float> rightSide(boxRect.x + modWidth + modeWidth + 1.f + mBarOffsetLen, boxRect.y, boxRect.x + modWidth + modeWidth + 3 + mBarOffsetLen, boxRect.w);
            Vector4<float> leftSide(boxRect.x - 1.f, boxRect.y, boxRect.x, boxRect.w + 1);
            mUnderline = Vector4<float>(boxRect.z - mTextWid, boxRect.y, boxRect.x, boxRect.y + 1.f);

            mShadowDraw.push_back(std::make_tuple(boxRect, mModuleName, mModuleMode, Vector2<float>(mod->mArrayListPos.x, mod->mArrayListPos.y), modWidth, modeWidth, mEaseAnim));

            mTextWid = boxRect.z - boxRect.x + 1.f;
            mTextPosY = MathUtil::lerp(mTextPosY, mTextPosY + mTextHeight, mEaseAnim);
            mUnderline = Vector4<float>(boxRect.x, boxRect.w, boxRect.z + 1.f, boxRect.w + 1.f);

            ++mIndex;
        }

        int mColorIndex = 0;

        if (mStyle == Style::Outline) 
        {
            ImRenderUtil::fillRectangle(Vector4<float>(mUnderline), Color(0, 255, 255), mEaseAnim);
        }

        for (const auto& pos : mShadowDraw) {
            Vector4<float> mBoxPos = std::get<0>(pos);
            std::string mModuleName = std::get<1>(pos);
            std::string mModuleMode = std::get<2>(pos);
            Vector2<float> mModulePos = std::get<3>(pos);
            float mModWidth = std::get<4>(pos);
            float mModeWidth = std::get<5>(pos);
            float mAnim = std::get<6>(pos);

            Color mArrayListColor = ColorUtil::getClientColor(1.8, 1, 1, mColorIndex * 40);

            Vector4<float> mRightSide(mBoxPos.x + mModWidth + mModeWidth + 1.f + mBarOffsetLen, mBoxPos.y, mBoxPos.x + mModWidth + mModeWidth + 4 + mBarOffsetLen, mBoxPos.w);

            if (mStyle == Style::Outline) 
            {
                ImRenderUtil::fillRectangle(mRightSide, Color(0, 255, 255), mAnim, 0);
                ImRenderUtil::fillShadowRectangle(mBoxPos, Color(0, 0, 0), mAnim * mShadowOpacity, mShadowIterations, 0);
                ImRenderUtil::fillShadowRectangle(mRightSide, Color(0, 255, 255), mAnim * mShadowOpacity, mShadowIterations, 0);
            }

            if (mStyle == Style::Flux)
            {
                ImRenderUtil::fillShadowRectangle(mBoxPos, Color(0, 0, 0), mGetShadowOpacity * mAnim, mShadowIterations, mShadowFilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, 0);
                ImRenderUtil::fillShadowRectangle(mRightSide, mArrayListColor, mGetBarOpacity * mAnim, mShadowIterations, mShadowFilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, 0);
                ImRenderUtil::fillRectangle(mBoxPos, Color(0, 0, 0), mGetOpacity * mAnim, 0);
                ImRenderUtil::fillRectangle(mRightSide, mArrayListColor, mAnim, 0);
            }

            ImRenderUtil::drawText(mModulePos, &mModuleName, mArrayListColor, mTextSize, mAnim, true);

            if (mDisplayMode)
                ImRenderUtil::drawText(Vector2<float>(mModulePos.x + mModWidth, mModulePos.y), &mModuleMode, Color(170, 170, 170), mTextSize, mAnim, true);

            mColorIndex++;
        }

        sortedModules.clear();
        ImGui::PopFont();
    }
private:
    static inline void sortModules(std::vector<std::shared_ptr<Module>>& sortedModules) {
        std::sort(sortedModules.begin(), sortedModules.end(), [](const std::shared_ptr<Module>& a, const std::shared_ptr<Module>& b) {
            std::string nameA = a->getName() + a->getModeName();
            std::string nameB = b->getName() + b->getModeName();
            float textSizeA = ImRenderUtil::getTextWidth(&nameA, 1.2f);
            float textSizeB = ImRenderUtil::getTextWidth(&nameB, 1.2f);
            return textSizeA > textSizeB;
            });
    }

    static inline void sortModulesAlphabetically(std::vector<std::shared_ptr<Module>>& sortedModules) {
        std::sort(sortedModules.begin(), sortedModules.end(), [](const std::shared_ptr<Module>& a, const std::shared_ptr<Module>& b) {
            std::string nameA = a->getName() + a->getModeName();
            std::string nameB = b->getName() + b->getModeName();
            return nameA < nameB;
            });
    }

    inline float easeInOutExpo(float x) {
        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5 ? pow(2, 20 * x - 10) / 2
            : (2 - pow(2, -20 * x + 10)) / 2;
    };

    inline float easeInOutBounce(float x) {
        if (x < 0.5) {
            return 8 * pow(2, 8 * (x - 1)) * abs(sin(x * PI * 7));
        }
        else {
            return 1 - 8 * pow(2, -8 * x) * abs(sin(x * PI * 7));
        }
    };

    __forceinline double easeInOutElastic(float x) {
        double t2;
        if (x < 0.45) {
            t2 = x * x;
            return 8 * t2 * t2 * sin(x * PI * 9);
        }
        else if (x < 0.55) {
            return 0.5 + 0.75 * sin(x * PI * 4);
        }
        else {
            t2 = (x - 1) * (x - 1);
            return 1 - 8 * t2 * t2 * sin(x * PI * 9);
        }
    }
};
