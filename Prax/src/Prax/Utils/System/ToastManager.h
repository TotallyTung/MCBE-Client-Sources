//
// Created by Flash on 9/2/2023.
//

#pragma once

#include <string>

class ToastManager {
public:
    static void Init();
    static void ShowToast(std::string title, std::string message);
};
