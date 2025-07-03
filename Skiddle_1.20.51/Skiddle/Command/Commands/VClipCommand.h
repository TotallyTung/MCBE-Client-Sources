#pragma once
#include <cctype> // 必要な場合、文字の判定に利用

class VClipCommand : public Command
{
public:

    VClipCommand() : Command("vclip", "Change the y pos", {
        "ypos"
        })
    {}

    bool execute(std::vector<std::string> cmd) override
    {
        if (cmd.size() < 1)
            return false; 

        bool containsInvalidChars = false;

        for (char c : cmd[0]) {
            if (!std::isdigit(c) && c != '+' && c != '-' && !std::isspace(c)) {
                containsInvalidChars = true;
                break;
            }
        }

        if (containsInvalidChars) {
            ChatUtil::sendMessage(combine("Invaild Argment! ", GRAY, "(", cmd[0], ")"));
            return false;
        }


        Player* player = Game::GetLocalPlayer();

        auto pos = Game::GetLocalPlayer()->GetPosition();
        Vector3<float> pos2 = Vector3<float>(pos.x, pos.y + std::stof(cmd[0]), pos.z);
        player->SetPos(pos2);

        ChatUtil::sendMessage(combine("VClipped! ", GRAY,  "(", std::stof(cmd[0]), ")"));
        // return false;
    }
};
