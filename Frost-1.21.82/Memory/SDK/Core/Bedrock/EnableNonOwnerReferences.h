#pragma once

namespace Bedrock {
    class EnableNonOwnerReferences {
    public:
        struct ControlBlock {
            EnableNonOwnerReferences* mPtr;
        };

        std::shared_ptr<ControlBlock> mControlBlock; // this+0x8

        // prevent constructor by default
        EnableNonOwnerReferences& operator=(EnableNonOwnerReferences const&);

    public:
        // vIndex: 0
        virtual ~EnableNonOwnerReferences();
    };
}