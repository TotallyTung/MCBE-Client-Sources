//
// Created by Flash on 8/10/2023.
//

#include "KeyManager.h"

std::map<uint64_t, bool> KeyManager::Keys;

uint64_t KeyManager::GetFirstDown() {
    for (auto key : Keys) {
        if (key.second) {

            if (key.first == VK_SPACE) // Space
                return 0;

            // Escape
            if (key.first == VK_ESCAPE)
                return 0;

            if (key.first == VK_CONTROL)
                continue;

            return key.first;
        }
    }

    return -1;
}