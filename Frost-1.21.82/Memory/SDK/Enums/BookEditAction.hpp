#pragma once

enum class BookEditAction : unsigned char {
    Replace = 0x0,
    Add     = 0x1,
    Delete  = 0x2,
    Swap    = 0x3,
    Sign    = 0x4,
};