#pragma once

class ConfigurationManager {
public:
    nlohmann::json GetSettings(std::vector<Setting*> _settings)
    {
        nlohmann::json arr = nlohmann::json::array();

        for (const auto& obj : _settings) {
            nlohmann::json obj_json = nlohmann::json::object();

            obj_json["name"] = obj->mName;
            obj_json["type"] = obj->mType;

            switch (obj->mType)
            {
            case SettingType::BOOL:
                if (!obj->mValue)
                    break;
                {
                    bool myBool = *reinterpret_cast<bool*>(obj->mValue);
                    int intBool = myBool ? 1 : 0;

                    obj_json["value"] = intBool;
                }

                break;
            case SettingType::ENUM:
                if (!obj->getIterator())
                    break;

                obj_json["value"] = *obj->getIterator();
                break;
            case SettingType::SLIDER:
                if (!obj->mValue)
                    break;

                obj_json["value"] = *reinterpret_cast<float*>(obj->mValue);
                break;
            }

            arr.push_back(obj_json);
        }

        return arr;
    }

    void SetSettings(std::vector<Setting*> _settings, nlohmann::json settings)
    {
        for (nlohmann::json::iterator it = settings.begin(); it != settings.end(); ++it)
        {
            nlohmann::json item = it.value();

            std::string itemname = item["name"].get<std::string>();

            for (Setting* setting : _settings)
            {
                if (strcmp(setting->getName().c_str(), itemname.c_str()) == 0)
                {
                    switch (setting->getType())
                    {
                    case SettingType::BOOL:
                        *(bool*)setting->getValue() = item["value"].get<int>();
                        break;
                    case SettingType::SLIDER:
                        *(float*)setting->getValue() = item["value"].get<float>();
                        break;
                    case SettingType::ENUM:
                        int value = item["value"].get<int>();

                        if (value >= 0 && value < setting->getEnumValues().size())
                            *setting->getIterator() = value;
                        break;
                    }
                }
            }
        }
    }

    void SaveConfig(std::string config)
    {
        nlohmann::json top_config;

        nlohmann::json global_config;
        global_config["version"] = "V2.1";
        global_config["name"] = "Frost";
        global_config["owner"] = "Isam Eldein Farid";
        top_config.push_back(global_config);

        for (const auto& mod : mModules)
        {
            nlohmann::json mod_config;
            mod_config["name"] = mod->getName();
            mod_config["keybind"] = mod->getKeybind();
            mod_config["enabled"] = mod->isEnabled();
            mod_config["visible"] = mod->isVisible();
            mod_config["settings"] = GetSettings(mod->getSettings());

            top_config.push_back(mod_config);
        }

        std::ofstream file(FileUtil::getClientPath() + "configurations\\" + config + ".json");
        if (file.is_open())
        {
            file << std::setw(4) << top_config;
            file.close();
        }

        ChatUtil::sendMessage(Util::combine("Successfully saved config ", TextFormat::getColorCode(TextFormat::Color::GRAY), config.c_str(), TextFormat::getStyleCode(TextFormat::Style::RESET), "."));
    }

    void LoadConfig(std::string config)
    {
        std::ifstream file(FileUtil::getClientPath() + "configurations\\" + config + ".json");

        if (file.good()) // Check if the file exists and can be opened
        {
            nlohmann::json _config = nlohmann::json::parse(file);

            for (nlohmann::json::iterator it = _config.begin(); it != _config.end(); ++it)
            {
                nlohmann::json item = it.value();

                if (item.contains("version"))
                {
                    //Game::Iris::ClientColor = item["version"].get<int>();
                    //Game::Iris::ClientName = item["name"].get<std::string>();

                    //_logf("Config has global config (loading...)");

                    continue; // skip cuz global config (probably)
                }

                //continue;

                std::string itemname = item["name"].get<std::string>();

                for (const auto& mod : mModules)
                {
                    if (strcmp(mod->getName().c_str(), itemname.c_str()) == 0)
                    {
                        // correct module so lets unload the json data

                        mod->setKeybind(item["keybind"].get<int>());

                        if (item["enabled"].get<bool>() != mod->isEnabled()) {
                            mod->toggle();
                        }

                        mod->setVisible(item["visible"].get<bool>());

                        SetSettings(mod->getSettings(), item["settings"]);
                    }
                }
            }
            ChatUtil::sendMessage(Util::combine("Successfully loaded configuration ", TextFormat::getColorCode(TextFormat::Color::GRAY), config.c_str(), TextFormat::getStyleCode(TextFormat::Style::RESET), "!"));
        }
        else
        {
            ChatUtil::sendMessage(Util::combine(TextFormat::getColorCode(TextFormat::Color::RED), "Could not load configuration ", TextFormat::getColorCode(TextFormat::Color::GRAY), config.c_str(), TextFormat::getColorCode(TextFormat::Color::RED), "!"));
        }
    }
};