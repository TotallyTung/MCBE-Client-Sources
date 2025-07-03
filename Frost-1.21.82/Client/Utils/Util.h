#pragma once

class Util {
public:
    template <typename T>
    static std::string combine(T t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    template <typename T, typename... Args>
    static std::string combine(T t, Args... args)
    {
        std::stringstream ss;
        ss << t << combine(args...);
        return ss.str();
    }

    static void setTitle(std::string title)
    {
        auto w = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, [title]() {
            winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(winrt::to_hstring(title));
            });
    }

    inline static std::string getUsername()
    {
        char username[UNLEN + 1];
        DWORD username_len = UNLEN + 1;

        GetUserNameA(username, &username_len);

        return std::string(username);
    }

    static inline bool mLeftClick, mLeftDown, mRightClick, mRightDown, mMiddleClick, mMiddleDown;

    static void onMouseClick(int key, bool isDown)
    {
        switch (key)
        {
        case 1:
            mLeftClick = isDown;
            mLeftDown = isDown ? true : mLeftDown;
            break;
        case 2:
            mRightClick = isDown;
            mRightDown = isDown ? true : mRightDown;
            break;
        case 3:
            mMiddleClick = isDown;
            mMiddleDown = isDown ? true : mMiddleDown;
            break;
        }
    }

    static bool invalidChar(char c) {
        return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
    }

    static std::string sanitize(std::string text) {
        std::string out;
        bool wasValid = true;
        for (char c : text) {
            bool isValid = !invalidChar(c);
            if (wasValid) {
                if (!isValid) {
                    wasValid = false;
                }
                else {
                    out += c;
                }
            }
            else {
                wasValid = isValid;
            }
        }
        return out;
    }
};