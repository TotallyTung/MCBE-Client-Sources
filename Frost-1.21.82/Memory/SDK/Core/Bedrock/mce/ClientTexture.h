#pragma once

namespace mce {
    class ClientTexture {
    public:
        std::shared_ptr<class data> mResourcePointerBlock;
        //std::shared_ptr<mce::ResourceBlockTemplate<std::variant<std::monostate, mce::Texture, dragon::res::ClientTexture>, mce::PerFrameHandleTracker, mce::ResourceServiceTextureDescription>> mResourcePointerBlock;
    public:
        virtual ~ClientTexture() = default;

        bool operator==(const ClientTexture& other) const {
            return mResourcePointerBlock == other.mResourcePointerBlock;
        }

        bool operator!=(const ClientTexture& other) const {
            return mResourcePointerBlock != other.mResourcePointerBlock;
        }
    };
};