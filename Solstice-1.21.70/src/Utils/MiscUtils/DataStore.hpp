#pragma once
#include <fstream>
//
// Created by vastrakai on 8/17/2024.
//

template <typename T> // T is the type of object to be stored
class DataStore {
public:
    std::string mName;
    std::vector<T> mObjects = {};

    explicit DataStore(std::string name) : mName(std::move(name)) {}
    ~DataStore() = default;

    [[nodiscard]] std::string getPath() const
    {
        return FileUtils::getSolsticeDir() + "\\Databases\\" + mName + ".json";
    }

    void load()
    {
        const int64_t loadStart = NOW;
        mObjects.clear();
        if (FileUtils::fileExists(getPath()))
        {
            nlohmann::json json;
            std::ifstream file(getPath());
            file >> json;
            file.close();
            fromJson(json);
        } else
        {
            spdlog::warn("[db] {}.json does not exist, creating new json    ", mName);
            save();
        }

        spdlog::info("[db] Loaded {} entries from {}.json in {}ms", mObjects.size(), mName, NOW - loadStart);

    }

    void save()
    {
        const nlohmann::json json = toJson();
        std::ofstream file(getPath());
        file << json.dump();
        file.close();
    }

    nlohmann::json toJson()
    {
        nlohmann::json json;
        for (auto& object : mObjects)
        {
            json.push_back(object.toJson());
        }
        return json;
    }

    void fromJson(nlohmann::json json)
    {
        for (auto& object : json)
        {
            T obj;
            obj.fromJson(object);
            mObjects.push_back(obj);
        }
    }
};

// All data objects should inherit from this class
class DataObject {
public:
    DataObject() = default;
    virtual ~DataObject() = default;

    virtual nlohmann::json toJson()
    {
        spdlog::critical("toJson not implemented for {}", typeid(*this).name());
        return {};
    }
    virtual void fromJson(nlohmann::json json)
    {
        spdlog::critical("fromJson not implemented for {}", typeid(*this).name());
    }
};
