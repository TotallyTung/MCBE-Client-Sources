#pragma once

//
// Created by alteik on 15/10/2024.
//

class ClickTp : public ModuleBase<ClickTp> {
public:

    ClickTp() : ModuleBase("ClickTp", "Teleports you to the block you're looking at", ModuleCategory::Player, 0, false)
    {
        mNames = {
            {Lowercase, "clicktp"},
              {LowercaseSpaced, "click tp"},
              {Normal, "ClickTp"},
              {NormalSpaced, "Click Tp"}
        };
    };

    void onEnable() override;
};
