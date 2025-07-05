//
// Created by Flash on 8/23/2023.
//

#pragma once

class ConfigManager {
public:
    static inline bool Loading = false;
    static inline std::string CurrentConfig = "";

    static void Init();
    static void Shutdown();
    static bool Load(const std::string& name);
    static void Save(const std::string& name);
    static void SetDefault(const std::string& name);
};
