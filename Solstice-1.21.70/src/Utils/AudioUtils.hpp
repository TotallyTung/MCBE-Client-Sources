#pragma once
//
// Created by vastrakai on 10/15/2024.
//


class AudioUtils {
public:
    static std::vector<std::string> listMicrophones();
    static void recordVoiceClip(std::function<void(const std::vector<BYTE>&)> onAudioDataReady);
};