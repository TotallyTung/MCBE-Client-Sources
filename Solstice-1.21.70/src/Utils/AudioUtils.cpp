//
// Created by vastrakai on 10/15/2024.
//

#include <Audioclient.h>
#include <codecvt>
#include <Mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>
#include <comdef.h>
#include <mmsystem.h>     // Multimedia API (for HWA

#include "Audio.h"

std::vector<std::string> AudioUtils::listMicrophones()
{
    std::vector<std::string> microphones;

    // Initialize COM
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM" << std::endl;
        return microphones;
    }

    // Get the device enumerator
    IMMDeviceEnumerator* pEnumerator = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        std::cerr << "Failed to create device enumerator" << std::endl;
        CoUninitialize();
        return microphones;
    }

    // Get the audio input devices
    IMMDeviceCollection* pCollection = nullptr;
    hr = pEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) {
        std::cerr << "Failed to enumerate audio devices" << std::endl;
        pEnumerator->Release();
        CoUninitialize();
        return microphones;
    }

    UINT count;
    pCollection->GetCount(&count);

    for (UINT i = 0; i < count; i++) {
        IMMDevice* pDevice = nullptr;
        hr = pCollection->Item(i, &pDevice);
        if (FAILED(hr)) {
            continue;
        }

        // Get the device name
        IPropertyStore* pProps = nullptr;
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (SUCCEEDED(hr)) {
            PROPVARIANT varName;
            PropVariantInit(&varName);
            hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
            if (SUCCEEDED(hr)) {
                microphones.push_back(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(varName.pwszVal));
                PropVariantClear(&varName);
            }
            pProps->Release();
        }
        pDevice->Release();
    }

    pCollection->Release();
    pEnumerator->Release();
    CoUninitialize();

    return microphones;
}

void writeWavHeader(std::ofstream &file, int sampleRate, int bitsPerSample, int channels, int dataSize) {
    // WAV header format
    file.write("RIFF", 4);  // ChunkID
    int32_t fileSize = 36 + dataSize;  // 36 + dataSize (header + data size)
    file.write(reinterpret_cast<const char*>(&fileSize), 4);  // ChunkSize
    file.write("WAVE", 4);  // Format
    file.write("fmt ", 4);  // Subchunk1ID
    int32_t subchunk1Size = 16;  // Subchunk1Size (16 for PCM)
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    int16_t audioFormat = 1;  // Audio format (1 for PCM)
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&channels), 2);  // NumChannels
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);  // SampleRate
    int32_t byteRate = sampleRate * channels * bitsPerSample / 8;
    file.write(reinterpret_cast<const char*>(&byteRate), 4);  // ByteRate
    int16_t blockAlign = channels * bitsPerSample / 8;
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);  // BlockAlign
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);  // BitsPerSample
    file.write("data", 4);  // Subchunk2ID
    file.write(reinterpret_cast<const char*>(&dataSize), 4);  // Subchunk2Size
}

#pragma comment(lib, "Ws2_32.lib")

void AudioUtils::recordVoiceClip(std::function<void(const std::vector<BYTE>&)> onAudioDataReady) {
    // Set up spdlog (file logging)
    spdlog::info("Starting voice recording...");

    // Variables for audio capture
    HWAVEIN hWaveIn;
    WAVEFORMATEX waveFormat;
    WAVEHDR waveHeader;
    MMRESULT result;
    const int bufferSize = 44100 * 2; // 1 second of 44100 Hz, 16-bit mono (2 bytes per sample)
    std::vector<BYTE> audioBuffer(bufferSize);

    // Set the format (PCM 16-bit, 44100 Hz, mono)
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample / 8);
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;
    spdlog::info("Audio format set to PCM 16-bit, 44100 Hz, mono");

    // Open the waveform input device
    result = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR) {
        spdlog::error("Failed to open waveform input device: {}", result);
        return;
    }
    spdlog::info("Waveform input device opened");

    // Set up the waveform header
    waveHeader.lpData = reinterpret_cast<LPSTR>(audioBuffer.data());
    waveHeader.dwBufferLength = bufferSize;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;

    // Prepare the header
    result = waveInPrepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        spdlog::error("Failed to prepare waveform header: {}", result);
        waveInClose(hWaveIn);
        return;
    }
    spdlog::info("Waveform header prepared");

    // Start recording
    result = waveInAddBuffer(hWaveIn, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        spdlog::error("Failed to add buffer: {}", result);
        waveInClose(hWaveIn);
        return;
    }

    result = waveInStart(hWaveIn);
    if (result != MMSYSERR_NOERROR) {
        spdlog::error("Failed to start recording: {}", result);
        waveInClose(hWaveIn);
        return;
    }
    spdlog::info("Recording started");

    // Capture audio in chunks and trigger the callback
    for (int i = 0; i < 5; ++i) { // Record 5 chunks (approx 5 seconds)
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Send the captured audio data using the callback function
        if (onAudioDataReady) {
            onAudioDataReady(audioBuffer);
        }
    }

    // Stop recording
    waveInStop(hWaveIn);
    spdlog::info("Recording stopped");

    // Unprepare the header and close the device
    waveInUnprepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));
    waveInClose(hWaveIn);
    spdlog::info("Waveform input device closed");
}
