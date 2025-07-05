#pragma once

#include "UIElement.h"

CategoryUIElement::CategoryUIElement(std::string name, ImVec2 initialPos,
                                     std::vector<std::pair<Module*, ModuleUIElement*>> modules)
        : UIElement(name) {
    this->Name = name;
    this->Position = initialPos;
    this->Modules = modules;
}
