// Created by Tony on 2024-10-10 10:01:54

// Spotify.h
#pragma once
#include "../Module.h"
#include <string>
#include <vector>
#include "../../../../Utils/AudioSystem.h"
#include "../../../../Utils/Internet.h"

class Spotify : public Module {
public:
    Spotify();
    ~Spotify();

    void Render(ImDrawList* drawlist);
    virtual bool isVisible() override;
    virtual void onEnable() override;
    virtual void onDisable() override;

private:
    Vec2<float> mousePos;
    float textHeight;
    Vec2<float> windowPos;
    float scrollOffset = 0.f;
    std::string searchQuery;
    bool isSearchFocused = false;
    std::vector<std::string> musicList;
    std::string currentlyPlaying;
    bool isPlaying = false;
    AudioSystem audioSystem;

    // Add these constants
    static constexpr float textSize = 1.f;
    static constexpr float textPadding = 2.f * textSize;

    void extractAudioFromLink(const std::string& link);
    void playMusic(const std::string& musicName);
    void stopMusic();
    void updateMusicList();
    void renderMusicList(ImDrawList* drawlist, const Vec4<float>& contentRect);
    void renderPlayerControls(ImDrawList* drawlist, const Vec4<float>& controlsRect);
};