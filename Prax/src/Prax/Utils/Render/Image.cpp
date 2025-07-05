//
// Created by Flash on 10/14/2023.
//

#include <Prax/Utils/System/Internet.h>
#include "Image.h"


std::map<std::string, ID3D11ShaderResourceView*> Image::Images;
ID3D11Device* Image::D3DDevice;

void Image::Init(ID3D11Device* device) {
    Logger::Write("Image", "Init", Logger::LogType::Debug);

    D3DDevice = device;

    LoadFromURL("https://cdn.discordapp.com/attachments/1157861980611297410/1163279643168747570/F6_T6MXXoAAH6os.jpg?ex=653eff97&is=652c8a97&hm=f97e31cf78c6edd75ac1e69acdbf0256ff2b08052bceab31ef562d52b2f8b780&", "esp.jpg");
}

void Image::Shutdown() {
    Logger::Write("Image", "Shutdown", Logger::Debug);
}

bool Image::LoadFromURL(const char* url, const char* filename) {

    if (!FileSystem::FileExists(FileSystem::ImageDirectory + "\\" + filename))
        Internet::DownloadFile(url, FileSystem::ImageDirectory + "\\" + filename);

    if (!FileSystem::FileExists(FileSystem::ImageDirectory + "\\" + filename))
        return false;

    ID3D11ShaderResourceView* out_srv = nullptr;

    auto result = LoadFromFileInternal((FileSystem::ImageDirectory + "\\" + filename).c_str(), &out_srv, nullptr, nullptr);

    if (result) {
        Images[filename] = out_srv;
    }

    return result;
}

bool Image::LoadFromFile(const char* filename) {
    if (!FileSystem::FileExists(FileSystem::ImageDirectory + "\\" + filename))
        return false;

    ID3D11ShaderResourceView* out_srv = nullptr;

    auto result = LoadFromFileInternal((FileSystem::ImageDirectory + "\\" + filename).c_str(), &out_srv, nullptr, nullptr);

    if (result) {
        Images[filename] = out_srv;
    }

    return result;
}

bool Image::LoadFromFileInternal(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{

    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;

    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    D3DDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    D3DDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    stbi_image_free(image_data);

    return true;
}