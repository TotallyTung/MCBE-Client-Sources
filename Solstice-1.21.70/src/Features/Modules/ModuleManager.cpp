//
// Created by vastrakai on 6/28/2024.
//

#include <build_info.h>
#include <Features/Modules/ModuleManager.hpp>
#include <Utils/OAuthUtils.hpp>

#include "Combat/Aura.hpp"
#include "Combat/AutoClicker.hpp"
#include "Combat/Criticals.hpp"
#include "Combat/InfiniteAura.hpp"
#include "Combat/Reach.hpp"
#include "Combat/TriggerBot.hpp"
#include "Combat/HitBoxes.hpp"

#include "Misc/AntiBot.hpp"
#include "Misc/Anticheat.hpp"
#include "Misc/AntiCheatDetector.hpp"
#include "Misc/AutoAccept.hpp"
#include "Misc/AutoCosmetic.hpp"
#include "Misc/AutoDodge.hpp"
#include "Misc/AutoLootbox.hpp"
#include "Misc/AutoMessage.hpp"
#include "Misc/AutoQueue.hpp"
#include "Misc/AutoReport.hpp"
#include "Misc/AutoSnipe.hpp"
#include "Misc/AutoVote.hpp"
#include "Misc/CostumeSpammer.hpp"
#include "Misc/Desync.hpp"
#include "Misc/DeviceSpoof.hpp"
#include "Misc/Disabler.hpp"
#include "Misc/EditionFaker.hpp"
#include "Misc/Friends.hpp"
//#include "Misc/IRC.hpp"
#include "Misc/KickSounds.hpp"
#include "Misc/Killsults.hpp"
#include "Misc/NetSkip.hpp"
#include "Misc/NoFilter.hpp"
#include "Misc/NoPacket.hpp"
#include "Misc/PacketLogger.hpp"
#include "Misc/PartySpammer.hpp"
#include "Misc/SkinBlinker.hpp"
#include "Misc/SkinStealer.hpp"
#include "Misc/Spammer.hpp"
#include "Misc/StaffAlert.hpp"
#include "Misc/TestModule.hpp"
#include "Misc/ToggleSounds.hpp"

#include "Movement/AirJump.hpp"
#include "Movement/AirSpeed.hpp"
#include "Movement/AntiImmobile.hpp"
#include "Movement/AutoPath.hpp"
#include "Movement/AutoWalk.hpp"
#include "Movement/DamageBoost.hpp"
#include "Movement/DebugFly.hpp"
#include "Movement/FastStop.hpp"
#include "Movement/Fly.hpp"
#include "Movement/HiveFly.hpp"
#include "Movement/InventoryMove.hpp"
#include "Movement/Jesus.hpp"
#include "Movement/Jetpack.hpp"
#include "Movement/LongJump.hpp"
#include "Movement/NoJumpDelay.hpp"
#include "Movement/NoSlowDown.hpp"
#include "Movement/Phase.hpp"
#include "Movement/ReverseStep.hpp"
#include "Movement/SafeWalk.hpp"
#include "Movement/ServerSneak.hpp"
#include "Movement/Speed.hpp"
#include "Movement/Spider.hpp"
#include "Movement/Sprint.hpp"
#include "Movement/Step.hpp"
#include "Movement/TargetStrafe.hpp"
#include "Movement/Velocity.hpp"

#include "Player/AntiVoid.hpp"
#include "Player/AutoBoombox.hpp"
#include "Player/AutoEat.hpp"
#include "Player/AutoKick.hpp"
#include "Player/AutoSpellBook.hpp"
#include "Player/AutoTool.hpp"
#include "Player/ChestAura.hpp"
#include "Player/ChestStealer.hpp"
#include "Player/ClickTp.hpp"
#include "Player/Derp.hpp"
#include "Player/Extinguisher.hpp"
#include "Player/FastMine.hpp"
#include "Player/Freecam.hpp"
#include "Player/InvManager.hpp"
#include "Player/MidclickAction.hpp"
#include "Player/NoFall.hpp"
#include "Player/NoRotate.hpp"
#include "Player/Nuker.hpp"
#include "Player/OreMiner.hpp"
#include "Player/Regen.hpp"
#include "Player/RegenRecode.hpp"
#include "Player/Scaffold.hpp"
#include "Player/Surround.hpp"
#include "Player/ZipLine.hpp"
#include "Player/Teams.hpp"
#include "Player/Timer.hpp"
#include "Player/FastEat.hpp"

#include "spdlog/spdlog.h"

#include "Visual/Animations.hpp"
#include "Visual/Arraylist.hpp"
#include "Visual/AutoScale.hpp"
#include "Visual/BlockESP.hpp"
#include "Visual/BoneEsp.hpp"
#include "Visual/ChinaHat.hpp"
#include "Visual/ClickGui.hpp"
#include "Visual/CustomChat.hpp"
#include "Visual/DestroyProgress.hpp"
#include "Visual/ESP.hpp"
#include "Visual/Freelook.hpp"
#include "Visual/FullBright.hpp"
#include "Visual/Glint.hpp"
#include "Visual/HudEditor.hpp"
#include "Visual/Interface.hpp"
#include "Visual/ItemESP.hpp"
#include "Visual/ItemPhysics.hpp"
#include "Visual/JumpCircles.hpp"
#include "Visual/Keystrokes.hpp"
#include "Visual/LevelInfo.hpp"
#include "Visual/MotionBlur.hpp"
#include "Visual/NameProtect.hpp"
#include "Visual/Nametags.hpp"
#include "Visual/NoCameraClip.hpp"
#include "Visual/NoDebuff.hpp"
#include "Visual/NoHurtcam.hpp"
#include "Visual/NoRender.hpp"
#include "Visual/Notifications.hpp"
#include "Visual/PlaceHighlights.hpp"
#include "Visual/RobloxCamera.hpp"
#include "Visual/SessionInfo.hpp"
#include "Visual/TargetHUD.hpp"
#include "Visual/Tracers.hpp"
#include "Visual/UpdateForm.hpp"
#include "Visual/ViewModel.hpp"
#include "Visual/Watermark.hpp"
#include "Visual/Zoom.hpp"

void ModuleManager::init()
{
    // Visual (must be initialized first)
    mModules.emplace_back(std::make_shared<HudEditor>());

    // Combat
    mModules.emplace_back(std::make_shared<Aura>());
    mModules.emplace_back(std::make_shared<TriggerBot>());
    mModules.emplace_back(std::make_shared<AutoClicker>());
    mModules.emplace_back(std::make_shared<Reach>());
    mModules.emplace_back(std::make_shared<Criticals>());
    mModules.emplace_back(std::make_shared<InfiniteAura>());
    mModules.emplace_back(std::make_shared<HitBoxes>());


    // Movement
    mModules.emplace_back(std::make_shared<Fly>());
    mModules.emplace_back(std::make_shared<Velocity>());
    mModules.emplace_back(std::make_shared<NoSlowDown>());
    mModules.emplace_back(std::make_shared<AntiImmobile>());
    mModules.emplace_back(std::make_shared<Sprint>());
    mModules.emplace_back(std::make_shared<Speed>());
    mModules.emplace_back(std::make_shared<InventoryMove>());
    mModules.emplace_back(std::make_shared<SafeWalk>());
    mModules.emplace_back(std::make_shared<NoJumpDelay>());
    mModules.emplace_back(std::make_shared<Phase>());
    mModules.emplace_back(std::make_shared<FastStop>());
    mModules.emplace_back(std::make_shared<Step>());
    mModules.emplace_back(std::make_shared<LongJump>());
    mModules.emplace_back(std::make_shared<Spider>());
    mModules.emplace_back(std::make_shared<ServerSneak>());
    mModules.emplace_back(std::make_shared<AirJump>());
    mModules.emplace_back(std::make_shared<TargetStrafe>());
    mModules.emplace_back(std::make_shared<Jesus>());
    mModules.emplace_back(std::make_shared<AirSpeed>());
    mModules.emplace_back(std::make_shared<ReverseStep>());
    mModules.emplace_back(std::make_shared<Jetpack>());
    mModules.emplace_back(std::make_shared<DamageBoost>());
    mModules.emplace_back(std::make_shared<AutoWalk>());

    // Player
    mModules.emplace_back(std::make_shared<AutoSpellBook>());
    mModules.emplace_back(std::make_shared<Timer>());
    mModules.emplace_back(std::make_shared<ChestStealer>());
    mModules.emplace_back(std::make_shared<InvManager>());
    mModules.emplace_back(std::make_shared<Regen>());
    mModules.emplace_back(std::make_shared<RegenRecode>());
    mModules.emplace_back(std::make_shared<Scaffold>());
    mModules.emplace_back(std::make_shared<Surround>());
    mModules.emplace_back(std::make_shared<ZipLine>());
    mModules.emplace_back(std::make_shared<Nuker>());
    mModules.emplace_back(std::make_shared<OreMiner>());
    mModules.emplace_back(std::make_shared<AutoBoombox>());
    mModules.emplace_back(std::make_shared<AutoTool>());
    mModules.emplace_back(std::make_shared<MidclickAction>());
    mModules.emplace_back(std::make_shared<Derp>());
    mModules.emplace_back(std::make_shared<Freecam>());
    mModules.emplace_back(std::make_shared<NoFall>());
    mModules.emplace_back(std::make_shared<Teams>());
    mModules.emplace_back(std::make_shared<AntiVoid>());
    mModules.emplace_back(std::make_shared<Extinguisher>());
    mModules.emplace_back(std::make_shared<FastMine>());
    mModules.emplace_back(std::make_shared<ClickTp>());
    mModules.emplace_back(std::make_shared<ChestAura>());
    mModules.emplace_back(std::make_shared<NoRotate>());
    mModules.emplace_back(std::make_shared<FastEat>());
    mModules.emplace_back(std::make_shared<AutoEat>());

    // Misc
    mModules.emplace_back(std::make_shared<ToggleSounds>());
    mModules.emplace_back(std::make_shared<PacketLogger>());
    mModules.emplace_back(std::make_shared<DeviceSpoof>());
    mModules.emplace_back(std::make_shared<EditionFaker>());
    mModules.emplace_back(std::make_shared<KickSounds>());
    mModules.emplace_back(std::make_shared<AutoQueue>());
    mModules.emplace_back(std::make_shared<AntiBot>());
    mModules.emplace_back(std::make_shared<AntiCheatDetector>());
    mModules.emplace_back(std::make_shared<Friends>());
    mModules.emplace_back(std::make_shared<NoPacket>());
    mModules.emplace_back(std::make_shared<NoFilter>());
    mModules.emplace_back(std::make_shared<AutoMessage>());
    mModules.emplace_back(std::make_shared<Killsults>());
    mModules.emplace_back(std::make_shared<NetSkip>());
    mModules.emplace_back(std::make_shared<Disabler>());
    mModules.emplace_back(std::make_shared<AutoReport>());
    mModules.emplace_back(std::make_shared<StaffAlert>());
    mModules.emplace_back(std::make_shared<AutoCosmetic>());
    mModules.emplace_back(std::make_shared<AutoAccept>());
    mModules.emplace_back(std::make_shared<PartySpammer>());
    mModules.emplace_back(std::make_shared<Spammer>());
 //   mModules.emplace_back(std::make_shared<IRC>());
    mModules.emplace_back(std::make_shared<SkinStealer>());
    mModules.emplace_back(std::make_shared<AutoLootbox>());
    mModules.emplace_back(std::make_shared<AutoDodge>());
    mModules.emplace_back(std::make_shared<AutoSnipe>());
    mModules.emplace_back(std::make_shared<AutoVote>());
    mModules.emplace_back(std::make_shared<CostumeSpammer>());
    mModules.emplace_back(std::make_shared<TestModule>());


    // Visual
    mModules.emplace_back(std::make_shared<Watermark>());
    mModules.emplace_back(std::make_shared<ClickGui>());
    mModules.emplace_back(std::make_shared<AutoScale>());
    mModules.emplace_back(std::make_shared<Interface>());
    mModules.emplace_back(std::make_shared<Arraylist>());
    mModules.emplace_back(std::make_shared<LevelInfo>());
    mModules.emplace_back(std::make_shared<Notifications>());
    mModules.emplace_back(std::make_shared<DestroyProgress>());
    mModules.emplace_back(std::make_shared<PlaceHighlights>());
    mModules.emplace_back(std::make_shared<ESP>());
    mModules.emplace_back(std::make_shared<BlockESP>());
    mModules.emplace_back(std::make_shared<MotionBlur>());
    mModules.emplace_back(std::make_shared<Animations>());
    mModules.emplace_back(std::make_shared<NoCameraClip>());
    mModules.emplace_back(std::make_shared<RobloxCamera>());
    mModules.emplace_back(std::make_shared<TargetHUD>());
    mModules.emplace_back(std::make_shared<ItemESP>());
    mModules.emplace_back(std::make_shared<Nametags>());
    mModules.emplace_back(std::make_shared<NoHurtcam>());
    mModules.emplace_back(std::make_shared<FullBright>());
    mModules.emplace_back(std::make_shared<Keystrokes>());
    mModules.emplace_back(std::make_shared<ViewModel>());
    mModules.emplace_back(std::make_shared<SessionInfo>());
    mModules.emplace_back(std::make_shared<Tracers>());
    mModules.emplace_back(std::make_shared<ChinaHat>());
    mModules.emplace_back(std::make_shared<NameProtect>());
    mModules.emplace_back(std::make_shared<Zoom>());
    mModules.emplace_back(std::make_shared<Glint>());
    mModules.emplace_back(std::make_shared<NoDebuff>());
    mModules.emplace_back(std::make_shared<JumpCircles>());
    mModules.emplace_back(std::make_shared<Freelook>());
    mModules.emplace_back(std::make_shared<NoRender>());


#ifdef __PRIVATE_BUILD__
    mModules.emplace_back(std::make_shared<HiveFly>()); // Flareon V2 boombox fly
    mModules.emplace_back(std::make_shared<DebugFly>()); // Real Sigma fly for Flareon V1 and the latest one
    mModules.emplace_back(std::make_shared<Desync>()); // needs troubleshooting
    mModules.emplace_back(std::make_shared<SkinBlinker>());
    mModules.emplace_back(std::make_shared<Anticheat>()); // Private for now cuz its not really good

    // TODO: Finish these modules
#endif

    // Development only
#ifdef __DEBUG__
    mModules.emplace_back(std::make_shared<AutoPath>());

    mModules.emplace_back(std::make_shared<AutoKick>()); // LMAO

    //mModules.emplace_back(std::make_shared<ItemPhysics>());
    mModules.emplace_back(std::make_shared<BoneEsp>());
    mModules.emplace_back(std::make_shared<CustomChat>());

#endif

    // Determine if we should add UpdateForm
    std::string oldHash = OAuthUtils::getLastCommitHash();
    std::string latestHash = SOLSTICE_BUILD_VERSION;
    if (oldHash != latestHash && oldHash != "")
    {
        spdlog::info("Adding UpdateForm module, oldHash: {}, latestHash: {}", oldHash, latestHash);
        mModules.emplace_back(std::make_shared<UpdateForm>());
    } else
    {
        spdlog::info("Not adding UpdateForm module, oldHash: {}, latestHash: {}", oldHash, latestHash);
    }

    for (auto& module : mModules)
    {
        try
        {
            module->onInit();
        } catch (const std::exception& e)
        {
            spdlog::error("Failed to initialize module {}: {}", module->mName, e.what());
        } catch (const nlohmann::json::exception& e)

        {
            spdlog::error("Failed to initialize module {}: {}", module->mName, e.what());
        } catch (...)
        {
            spdlog::error("Failed to initialize module {}: unknown", module->mName);
        }
    }
}

void ModuleManager::shutdown()
{
    for (auto& module : mModules)
    {
        if (module->mEnabled)
        {
            module->mEnabled = false;
            module->onDisable();
        }
    }

    mModules.clear();
}

void ModuleManager::registerModule(const std::shared_ptr<Module>& module)
{
    mModules.push_back(module);
}

std::vector<std::shared_ptr<Module>>& ModuleManager::getModules()
{
    return mModules;
}

Module* ModuleManager::getModule(const std::string& name) const
{
    for (const auto& module : mModules)
    {
        if (StringUtils::equalsIgnoreCase(module->mName, name))
        {
            return module.get();
        }
    }
    return nullptr;
}

void ModuleManager::removeModule(const std::string& name)
{
    for (auto it = mModules.begin(); it != mModules.end(); ++it)
    {
        if (StringUtils::equalsIgnoreCase((*it)->mName, name))
        {
            mModules.erase(it);
            return;
        }
    }
}

std::vector<std::shared_ptr<Module>>& ModuleManager::getModulesInCategory(int catId)
{
    static std::unordered_map<int, std::vector<std::shared_ptr<Module>>> categoryMap = {};
    if (categoryMap.contains(catId))
    {
        return categoryMap[catId];
    }

    // Cache category
    std::vector<std::shared_ptr<Module>> modules;
    for (const auto& module : mModules)
    {
        if (static_cast<int>(module->mCategory) == catId)
        {
            modules.push_back(module);
        }
    }

    categoryMap[catId] = modules;
    return categoryMap[catId];
}

std::unordered_map<std::string, std::shared_ptr<Module>> ModuleManager::getModuleCategoryMap()
{
    static std::unordered_map<std::string, std::shared_ptr<Module>> map;

    if (!map.empty())
    {
        return map;
    }

    for (const auto& module : mModules)
    {
        map[module->getCategory()] = module;
    }

    return map;
}

void ModuleManager::onClientTick()
{
    for (auto& module : mModules)
    {
        try
        {
            if (module->mWantedState != module->mEnabled)
            {
                module->mEnabled = module->mWantedState;
                spdlog::trace("onClientTick: calling {} on module {}", module->mEnabled ? "onEnable" : "onDisable", module->mName);
                if (module->mEnabled)
                {
                    module->onEnable();
                }
                else
                {
                    module->onDisable();
                }
            }
        } catch (const std::exception& e)
        {
            spdlog::error("Failed to enable/disable module {}: {}", module->mName, e.what());
        } catch (const nlohmann::json::exception& e)
        {
            spdlog::error("Failed to enable/disable module {}: {}", module->mName, e.what());
        } catch (...)
        {
            spdlog::error("Failed to enable/disable module {}: unknown", module->mName);
        }
    }

    auto holder = nes::make_holder<ClientTickEvent>();
    gFeatureManager->mDispatcher->trigger(holder);
}

nlohmann::json ModuleManager::serialize() const
{
    nlohmann::json j;
    j["client"] = "Solstice";
    j["version"] = SOLSTICE_VERSION;
    j["modules"] = nlohmann::json::array();

    for (const auto& module : mModules)
    {
        j["modules"].push_back(module->serialize());
    }

    return j;
}

nlohmann::json ModuleManager::serializeModule(Module* module)
{
    // same as above but only for the specified module
    nlohmann::json j;
    j["client"] = "Solstice";
    j["version"] = SOLSTICE_VERSION;
    j["modules"] = nlohmann::json::array();

    j["modules"].push_back(module->serialize());

    return j;
}

void ModuleManager::deserialize(const nlohmann::json& j, bool showMessages)
{
    // Get the version of the config
    const std::string version = j["version"];
    std::string currentVersion = SOLSTICE_VERSION;

    if (version != currentVersion)
    {
        spdlog::warn("Config version mismatch. Expected: {}, Got: {}", currentVersion, version);
        ChatUtils::displayClientMessage("§eWarning: The specified config is from a different version of Solstice. §cSome settings may not be loaded§e.");
    }

    int modulesLoaded = 0;
    int settingsLoaded = 0;
    // Get names of all modules in the moduleManager
    std::vector<std::string> moduleNames;
    for (const auto& module : mModules)
    {
        moduleNames.push_back(module->mName);
    }
    for (const auto& module : j["modules"])
    {
        const std::string name = module["name"];
        std::erase(moduleNames, name);
        const bool enabled = module["enabled"];
        const int keybind = module["key"];
        //const bool hideInArraylist = module["hideInArraylist"];

        auto* mod = getModule(name);
        if (mod)
        {
            mod->mWantedState = enabled;
            mod->mKey = keybind;
            //mod->mHideInArraylist = hideInArraylist;
            // Get the settings for the module
            std::vector<std::string> settingNames;
            for (const auto& setting : mod->mSettings)
            {
                settingNames.push_back(setting->mName);
            }
            if (module.contains("settings"))
            {
                for (const auto& setting : module["settings"].items())
                {
                    try
                    {
                        const auto& settingValue = setting.value();
                        const std::string settingName = settingValue["name"];
                        std::erase(settingNames, settingName);

                        auto* set = mod->getSetting(settingName);
                        if (set)
                        {
                            if (set->mType == SettingType::Bool)
                            {
                                auto* boolSetting = static_cast<BoolSetting*>(set);
                                boolSetting->mValue = settingValue["boolValue"];

                                if (settingValue.contains("key"))
                                {
                                    boolSetting->mKey = settingValue["key"];
                                }
                                else
                                {
                                    boolSetting->mKey = -1;
                                }
                            }
                            else if (set->mType == SettingType::Number)
                            {
                                auto* numberSetting = static_cast<NumberSetting*>(set);
                                numberSetting->mValue = settingValue["numberValue"];
                            }
                            else if (set->mType == SettingType::Enum)
                            {
                                auto* enumSetting = static_cast<EnumSetting*>(set);
                                // Make sure the enum value is valid and within the bounds
                                if (settingValue["enumValue"] >= 0 && settingValue["enumValue"] < enumSetting->mValues.size())
                                    enumSetting->mValue = settingValue["enumValue"];
                                else
                                {
                                    spdlog::warn("Invalid enum value for setting {} in module {}", settingName, name);
                                    if (showMessages) ChatUtils::displayClientMessage("§cInvalid enum value for setting §6" + settingName + "§c in module §6" + name + "§c.");
                                }
                            } else if (set->mType == SettingType::Color)
                            {
                                auto* colorSetting = static_cast<ColorSetting*>(set);
                                // Get the  settingValue["colorValue"] as a float[4]
                                for (int i = 0; i < 4; i++)
                                {
                                    colorSetting->mValue[i] = settingValue["colorValue"][i];
                                }
                            }

                            settingsLoaded++;
                        } else
                        {
                            spdlog::warn("Setting {} not found for module {}", settingName, name);
                            if (showMessages) ChatUtils::displayClientMessage("§cSetting §6" + settingName + "§c not found for module §6" + name + "§c.");
                        }
                    } catch (const std::exception& e)
                    {
                        spdlog::warn("Failed to load setting {} for module {}: {}", setting.key(), name, e.what());
                        if (showMessages) ChatUtils::displayClientMessage("§cFailed to load setting §6" + setting.key() + "§c for module §6" + name + "§c.");
                    }
                }

                modulesLoaded++;
            }

            // If there are any settings left, log it
            for (const auto& settingName : settingNames)
            {
                spdlog::warn("Setting {} not found for module {}, default value will be used", settingName, name);
                if (showMessages) ChatUtils::displayClientMessage("§cSetting §6" + settingName + "§c not found for module §6" + name + "§c, default value will be used.");
            }
        } else
        {
            spdlog::warn("Module {} not found", name);
            if (showMessages) ChatUtils::displayClientMessage("§cModule §6" + name + "§c not found.");
        }
    }

    // If there are any modules left, log it
    for (const auto& moduleName : moduleNames)
    {
        spdlog::warn("Module {} not found in config, using default settings", moduleName);
        if (showMessages) ChatUtils::displayClientMessage("§cModule §6" + moduleName + "§c not found in config, using default settings.");
    }

    spdlog::info("Loaded {} modules and {} settings from config", modulesLoaded, settingsLoaded);
}