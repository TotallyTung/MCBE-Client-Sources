#pragma once

enum class SettingType {
    BOOL, SLIDER, ENUM, COLOR, BUTTON, TEXTBOX
};

class Setting {
public:
    std::string mName;
    std::string mDescription;
    SettingType mType;
    void* mValue = nullptr;
    float mMin = 0.0f;
    float mMax = 0.0f;
    int mNumbers = 0.0f;
    std::vector<std::string> mEnumValues;
    int* mIterator = nullptr;
    std::function<bool()> mRender = nullptr;

    ~Setting() {
        if (mType == SettingType::ENUM) {
            delete mIterator;
        }
        else if (mValue) {
            delete mValue;
            mValue = nullptr;
        }
    }

    Setting(const std::string& name, const std::string& description, float* value,
        float min, float max, int numbers, std::function<bool()> render)
        : mName(name), mDescription(description), mType(SettingType::SLIDER), mValue(value), mMin(min), mMax(max), mNumbers(numbers), mRender(render) {}

    Setting(const std::string& name, const std::string& description, const std::vector<std::string>& enumValues,
        int* iterator, std::function<bool()> render)
        : mName(name), mDescription(description), mType(SettingType::ENUM), mEnumValues(enumValues), mIterator(iterator), mRender(render) {}

    Setting(const std::string& name, const std::string& description, void* state, std::function<bool()> render)
        : mName(name), mDescription(description), mType(SettingType::BOOL), mValue(state), mRender(render) {}

    std::string getName() const { return mName; }
    std::string getDescription() const { return mDescription; }
    SettingType getType() const { return mType; }
    float getMin() const { return mMin; }
    float getMax() const { return mMax; }
    void* getValue() { return mValue; }
    float getNumber() const { return mNumbers; }
    std::vector<std::string> getEnumValues() const { return mEnumValues; }
    int* getIterator() const { return mIterator; }
    bool shouldRender() const { return mRender(); }

    float mSliderEase = 0;
    float mBoolScale = 0;
    bool mIsDragging = false;

    bool mEnumExtended = false;
    float mEnumSlide = 0;
};