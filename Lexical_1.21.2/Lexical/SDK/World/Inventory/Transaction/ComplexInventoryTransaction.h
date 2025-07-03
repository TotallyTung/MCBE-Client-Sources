#pragma once
#include "../../../../Utils/MemoryUtil.h"
#include "../../../../Utils/Maths.h"
#include "InventoryTransaction.h"

class ComplexInventoryTransaction {
private:
    uintptr_t** VTable;
public:
    enum class Type : int {
        NormalTransaction = 0x0,
        InventoryMismatch = 0x1,
        ItemUseTransaction = 0x2,
        ItemUseOnEntityTransaction = 0x3,
        ItemReleaseTransaction = 0x4,
    };
    Type type;
    InventoryTransaction data;

    ComplexInventoryTransaction() {
        this->VTable = (uintptr_t**)Addresses::ComplexInventoryTransactionVTable;
    }
};