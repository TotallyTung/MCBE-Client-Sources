#pragma once

class UIControl;

class UIComponent {
public:
   uintptr_t** vtable; // this+0x00
   UIControl&  mOwner; // this+0x08
};

#include "TextComponent.h"