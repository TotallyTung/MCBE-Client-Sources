#pragma once

enum class PlayerPermissionLevel : signed char {
    Visitor  = 0x0,
    Member   = 0x1,
    Operator = 0x2,
    Custom   = 0x3,
};