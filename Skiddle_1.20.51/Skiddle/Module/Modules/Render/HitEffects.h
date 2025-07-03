#pragma once

class HitEffects : public Module
{
private:
    bool sound1 = true;
    bool sound2 = false;
    bool sound3 = false;
    bool sound4 = false;

    bool attacked = false;
    Player* attackedPlayer;

    int sound = 0;
public:
    HitEffects(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("HitEffects", "Render", "Customize your effect", keybind, enabled)
    {
        registerEnumSetting("Sound", "Change hit sound", { "None", "Stone" }, &sound);
    }

    void onEvent(AttackEvent* event) {
        attacked = true;
        attackedPlayer = event->Player;
    }

    void onEvent(ImGUIRenderEvent* event) {
        if (!Game::TargetLists::auraList.empty())
        {
            auto target = Game::TargetLists::auraList[0];
            if (target->IsBadPtr())return;
            if (attackedPlayer == target && attacked) {
                switch (sound) {
                case 0: // None
                    break;
                case 1: // Stone Break
                    if (TimeUtil::hasTimeElapsed("hitsounddelay", 1000 / 2, true))
                    {
                        if (sound1) {
                            Util::systemPlay("\\Sounds\\HitEffects\\Attack_1.wav");
                            sound1 = false;
                            sound2 = true;
                            sound3 = false;
                            sound4 = false;
                        }
                        else if (sound2) {
                            Util::systemPlay("\\Sounds\\HitEffects\\Attack_2.wav");
                            sound1 = false;
                            sound2 = false;
                            sound3 = true;
                            sound4 = false;
                        }
                        else if (sound3) {
                            Util::systemPlay("\\Sounds\\HitEffects\\Attack_3.wav");
                            sound1 = false;
                            sound2 = false;
                            sound3 = false;
                            sound4 = true;
                        }
                        else if (sound4) {
                            Util::systemPlay("\\Sounds\\HitEffects\\Attack_4.wav");
                            sound1 = true;
                            sound2 = false;
                            sound3 = false;
                            sound4 = false;
                        }
                    }
                    break;
                }
                attacked = false;
            }
        }
    }

    void onEnabled()
    {
        sound1 = true;
        sound2 = false;
        sound3 = false;
        sound4 = false;
    }
    void onDisabled()
    {
        sound1 = false;
        sound2 = false;
        sound3 = false;
        sound4 = false;
    }
};