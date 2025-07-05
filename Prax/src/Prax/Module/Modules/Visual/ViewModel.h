//
// Created by vastrakai on 4/19/2024.
//


#pragma once


#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Module.h>

class ViewModel : public ModuleBase<ViewModel> {
public:

    BoolSetting Translate = BoolSetting("Translate", "Translate the viewmodel", true);
    NumberSetting TranslateX = NumberSetting("Translate X", "The X translation", -100.f, 100.f, 0.f, 0.1f);
    NumberSetting TranslateY = NumberSetting("Translate Y", "The Y translation", -100.f, 100.f, 0.f, 0.1f);
    NumberSetting TranslateZ = NumberSetting("Translate Z", "The Z translation", -100.f, 100.f, 0.f, 0.1f);

    BoolSetting Rotate = BoolSetting("Rotate", "Rotate the viewmodel", true);
    NumberSetting RotateX = NumberSetting("Rotate X", "The X rotation", -180.f, 180.f, 0.f, 0.1f);
    NumberSetting RotateY = NumberSetting("Rotate Y", "The Y rotation", -180.f, 180.f, 0.f, 0.1f);
    NumberSetting RotateZ = NumberSetting("Rotate Z", "The Z rotation", -180.f, 180.f, 0.f, 0.1f);

    ViewModel() : ModuleBase("ViewModel", "Changes the viewmodel", "Visual") {
        AddSetting(&Translate);
        AddSetting(&TranslateX);
        AddSetting(&TranslateY);
        AddSetting(&TranslateZ);

        AddSetting(&Rotate);
        AddSetting(&RotateX);
        AddSetting(&RotateY);
        AddSetting(&RotateZ);
    };


    void OnRenderMatrix(glm::mat4* matrix) override;

};