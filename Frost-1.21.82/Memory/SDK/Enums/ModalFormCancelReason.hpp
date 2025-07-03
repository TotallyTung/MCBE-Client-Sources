#pragma once

enum class ModalFormCancelReason : signed char {
    UserClosed = 0x0,
    UserBusy   = 0x1,
};