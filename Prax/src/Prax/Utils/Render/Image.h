//
// Created by Flash on 10/14/2023.
//

#pragma once

#include <Prax/Utils/System/Logger.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>
#include <map>

#include <stb_image.h>

class Image {
public:
    static std::map<std::string, ID3D11ShaderResourceView*> Images;
    static ID3D11Device* D3DDevice;

    static void Init(ID3D11Device* device);
    static void Shutdown();

    static bool LoadFromURL(const char *url, const char *filename);
    static bool LoadFromFile(const char *filename);

private:
    // Util used to load images
    // Don't use this one outside this class
    static bool LoadFromFileInternal(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
};