// Created by Tony on 2024-10-10 10:01:54

// ModuleQueue.h
#pragma once
#include <queue>
#include <functional>

class ModuleQueue {
private:
    std::queue<std::function<void()>> actionQueue;

public:
    void addAction(std::function<void()> action) {
        actionQueue.push(action);
    }

    void processQueue() {
        if (!actionQueue.empty()) {
            actionQueue.front()();
            actionQueue.pop();
        }
    }

    bool isEmpty() const {
        return actionQueue.empty();
    }
};