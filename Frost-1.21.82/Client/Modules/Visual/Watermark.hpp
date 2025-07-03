#pragma once

class Watermark : public Module
{
public:
    Watermark() :
        Module("Watermark", "Visual", "Draws the Frost logo on your screen.", Keys::NONE, true)
    {
        EnumSetting("Style", "The visual style", { "Simple", "Solaris" }, &mStyle);
    }

private:
    int mStyle = 0;
public:
    void onEvent(ImRenderEvent* event) override {
        auto mPlayer = InstanceManager::getLocalPlayer();

        std::string mClientName = "Frost";
        std::string mVersion = GlobalVariables::mClientVersion;
        static std::string mIgn = "Loading...";
        static std::string mServerIP = "Local";
        static int mFps = static_cast<int>(ImGui::GetIO().Framerate);

        if (TimeUtil::hasTimeElapsed("FpsUpdateTime", 1000, true)) {
            mFps = static_cast<int>(ImGui::GetIO().Framerate);
        }

        if (mPlayer) {
            std::string mUser(mPlayer->getNametag()->c_str());
            mUser = Util::sanitize(mUser).substr(0, mUser.find('\n'));

            mIgn = mUser;
        }

        auto drawText = [&](const std::string& text, const Vector2<float>& position, const float& size, const float& alpha, const float& thickness) {
            Vector2<float> currentPosition = Vector2<float>(position.x - 10, position.y - 10);  // Start at the given position

            for (int i = 0; i < text.size(); ++i) {
                std::string character(1, text[i]);
                int colorIndex = i * 80;
                float charWidth = ImRenderUtil::getTextWidth(&character, size);
                float charHeight = ImRenderUtil::getTextHeight(size);
                Color color = ColorUtil::getClientColor(1.9, 1, 1, colorIndex);

                // Draw shadow and text
                ImRenderUtil::drawShadowSquare(Vector2<float>(6.f + currentPosition.x + charWidth / 2, currentPosition.y - 1 + charHeight / 1.2f), 7.142857142857143 * size, color, alpha, thickness, 0);
                ImRenderUtil::drawText(Vector2<float>(currentPosition.x + 10, currentPosition.y + 10), &character, color, size, 1, true);

                currentPosition.x += charWidth;  // Update position for next character
            }
            };

        switch (mStyle) {
        case 0: // Simple
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            drawText(mClientName, Vector2<float>(6.f, 6.f), 2.1f, 1, 70);
            ImGui::PopFont();
            break;
        case 1: // Frost
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            std::string mFPS = Util::combine(std::to_string(mFps), "fps");
            std::string mText = Util::combine(" | ", mVersion, " | ", mIgn, " | ", mFPS, " | ", mServerIP);
            std::string mCombinedText = Util::combine("Frost", mText);
            
            float mTextLength = ImRenderUtil::getTextWidth(&mCombinedText, 1.2f);
            float mTextHeight = ImRenderUtil::getTextHeight(1.2f);
            float mOtherTextLength = ImRenderUtil::getTextWidth(&mClientName, 1.2f);

            float inScale = 0;

            Vector4<float> mLineRect = Vector4<float>(9.5f, 10, 1 + mTextLength, 10 + 2);
            Vector4<float> mRect = Vector4<float>(10, 10, 1 + mTextLength, 12 + mTextHeight);

            Vector4<float> mMovingRect = Vector4<float>(mLineRect.x + (mTextLength * inScale), mLineRect.y, mLineRect.z, mLineRect.w);

            ImRenderUtil::fillRectangle(mRect, Color(0, 0, 0), 0.5f);

            ImClippingUtil::beginClipping(mMovingRect);
            ImRenderUtil::fillGradientRectangle(mLineRect, ColorUtil::getClientColor(1.5f, 1, 1, 1), ColorUtil::getClientColor(1.5f, 1, 1, 400), 1, 1);
            ImClippingUtil::restoreClipping();

            drawText(mClientName, Vector2<float>(mRect.x + 4.f, mRect.y + 2.f), 1.1f, 0.75f, 70);

            ImRenderUtil::drawText(Vector2<float>(mOtherTextLength + mRect.x + 4.f, mRect.y + 2.f), &mText, Color(255, 255, 255), 1.1f, 1, true);
            ImGui::PopFont();
            break;
        }
    }
};