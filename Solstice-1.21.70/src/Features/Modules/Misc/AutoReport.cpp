//
// Created by vastrakai on 8/13/2024.
//

#include "AutoReport.hpp"

#include <regex>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/ModalFormResponsePacket.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlaySoundPacket.hpp>

#include "AutoQueue.hpp"
/*{
    "buttons": [
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/bed-bedwars.png",
                "type": "url"
            },
            "text": "Bruno15901"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/monster-max.png",
                "type": "url"
            },
            "text": "MouldyPark85600"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/monster-drew.png",
                "type": "url"
            },
            "text": "Fake KJ"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/default.png",
                "type": "url"
            },
            "text": "lbizzzyl"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/monster-riley.png",
                "type": "url"
            },
            "text": "Kianura14"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/bed-bedwars.png",
                "type": "url"
            },
            "text": "LeadingFish6318"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/default.png",
                "type": "url"
            },
            "text": "Yo8356"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/default.png",
                "type": "url"
            },
            "text": "SrOfTheNight"
        },
        {
            "image": {
                "data": "https://cdn.playhive.com/avatars/sky-island.png",
                "type": "url"
            },
            "text": "Kingsmill3839"
        }
    ],
    "content": "If you suspect a player to be breaking our rules, you may report them below.",
    "title": "Report Player",
    "type": "form"
}
[10:16:30.473] [info] Form ID 61 was closed [button 5]
{
    "buttons": [
        {
            "text": "Hacking or Cheating\nClient modifications"
        },
        {
            "text": "Behavior or Skin\nAFK, name, and more."
        },
        {
            "text": "Chat\nIn-Chat Offenses"
        }
    ],
    "content": "What are you reporting LeadingFish6318 for?",
    "title": "Report LeadingFish6318",
    "type": "form"
}
[10:16:31.341] [info] Form ID 62 was closed [button 0]
{
    "buttons": [
        {
            "text": "AutoClicking"
        },
        {
            "text": "Flying"
        },
        {
            "text": "Combat (PvP)"
        },
        {
            "text": "No Knockback"
        },
        {
            "text": "Movement"
        }
    ],
    "content": "What are you reporting LeadingFish6318 for?",
    "title": "Report LeadingFish6318",
    "type": "form"
}
[10:16:36.367] [info] Form ID 63 was closed [button 4]
{
    "buttons": [
        {
            "text": "Confirm"
        },
        {
            "text": "Cancel"
        },
        {
            "text": "Go back"
        }
    ],
    "content": "You are about to report LeadingFish6318 for Movement: the player using a movement related cheat or modification.\n\n┬ºcAbuse of reports can lead to a punishment against your account.",
    "title": "Confirm Report",
    "type": "form"
}
[10:16:37.267] [info] Form ID 64 was closed [button 0]
[10:16:37.324] [info] Received message: ┬ºa┬ºl┬╗ ┬ºr┬º7Your report against ┬ºeLeadingFish6318 ┬º7has been submitted.*/

void AutoReport::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoReport::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &AutoReport::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoReport::onBaseTickEvent>(this);

    mFinishedReporting = false;
}

void AutoReport::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoReport::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &AutoReport::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoReport::onBaseTickEvent>(this);


    if (!mFinishedReporting)
    {
        ChatUtils::displayClientMessage("Cancelled reporting");
        mFinishedReporting = true;
    }
}

// get current ms with the NOW macro

void AutoReport::submitForm(int buttonId)
{
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = mLastFormId;
    MinecraftJson::Value json;
    json.mType = MinecraftJson::ValueType::Int;
    json.mValue.mInt = buttonId;
    packet->mJSONResponse = json;

    // Get the assoc button text
    auto jsonObj = nlohmann::json::parse(mJson);
    try
    {
        std::string buttonText = jsonObj["buttons"][buttonId]["text"];
        spdlog::info("Submitting form with button {}", buttonText);
    } catch (std::exception& e)
    {
        spdlog::error("Failed to get button text: {}", e.what());
    }

    ClientInstance::get()->getPacketSender()->send(packet.get());
}

void AutoReport::closeForm()
{
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = mLastFormId;
    packet->mFormCancelReason = ModalFormCancelReason::UserClosed;
    ClientInstance::get()->getPacketSender()->send(packet.get());
}

void AutoReport::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    // TODO: Replace this with an event trigger instead of a module dependency
    auto autoQueue = gFeatureManager->mModuleManager->getModule<AutoQueue>();

    if (NOW - mLastTeleport <= 1000 && NOW - mLastDimensionChange <= 1000)
    {
        if (player->getStatusFlag(ActorFlags::Noai))
        {
            mFinishedReporting = false;
            mLastTeleport = 0;
            mLastDimensionChange = 0;
            ChatUtils::displayClientMessage("Started reporting");
        } else {
            mFinishedReporting = true;
            mLastTeleport = 0;
            mLastDimensionChange = 0;
            ChatUtils::displayClientMessage("Cancelled reporting");

        }
    }

    if (mFinishedReporting)
    {
        if (mHasFormOpen && mIsReportMenu) {
            closeForm();
        }
        return;
    }

    if (NOW - mLastReportTime < mDelay.as<int>() * 1000)
    {
        mQueuedCommands.clear();
        return;
    }

    for (auto& [time, cmd] : mQueuedCommands) {
        if (NOW - mLastCommandTime < 1000) {
            continue;
        }

        mLastCommandTime = NOW;
        mLastExecTime = NOW;
        CommandUtils::executeCommand(cmd);
        mQueuedCommands.erase(mQueuedCommands.begin());
        break;
    }



    if (mQueuedCommands.empty() && mLastExecTime - NOW > 1000 && !mHasFormOpen)
        mQueuedCommands.emplace_back(NOW, "/report");

    if (mHasFormOpen && mIsReportMenu)
    {
        auto jsonObj = nlohmann::json::parse(mJson);
        std::string content = jsonObj.contains("content") ? jsonObj["content"] : "";

        if (content == "If you suspect a player to be breaking our rules, you may report them below.") {
            mReportStage = 0;
            // Pick and choose a random player to report
            int buttons = jsonObj["buttons"].size();
            std::vector<int> validButtons;
            for (int i = 0; i < buttons; i++)
            {
                // Make sure the player hasn't been reported already
                std::string playerName = jsonObj["buttons"][i]["text"];
                if (std::ranges::find(mReportedPlayers, playerName) == mReportedPlayers.end())
                {
                    validButtons.push_back(i);
                }
            }
            if (validButtons.empty())
            {
                if (mQueueWhenFinished && autoQueue)
                {
                    autoQueue->mQueueForGame = true;
                    autoQueue->mLastQueueTime = NOW - autoQueue->mQueueDelay.mValue * 1000; // Instantly queue
                }
                ChatUtils::displayClientMessage("Finished reporting");
                mFinishedReporting = true;
                closeForm();
                return;
            }
            int random = validButtons[MathUtils::random(0, validButtons.size() - 1)];
            submitForm(random);
        }

        if (StringUtils::containsIgnoreCase(content, "what are you reporting ") && StringUtils::containsIgnoreCase(content, " for?"))
        {
            // If the first button text contains "Hacking or Cheating" then submit that button
            if (jsonObj["buttons"][0].contains("text") && StringUtils::containsIgnoreCase(jsonObj["buttons"][0]["text"], "Hacking or Cheating"))
            {
                mReportStage = 1;
                submitForm(0);
            } else
            {
                mReportStage = 2;
                // You are about to report LeadingFish6318 for Movement: the player using a movement related cheat or modification.\n\n§cAbuse of reports can lead to a punishment against your account.
                // Get the acc name
                std::regex re("You are about to report (.*?) for");
                std::smatch match;
                std::string accName = StringUtils::replaceAll(mLastFormTitle, "Report ", "");

                size_t buttons = jsonObj["buttons"].size();
                int random = MathUtils::random(0, buttons - 1);
                std::string txt = "";
                // Get the text for this button
                if (jsonObj["buttons"][random].contains("text"))
                {
                    txt = jsonObj["buttons"][random]["text"];
                }
                ChatUtils::displayClientMessage("Reporting " + accName + " for " + txt);
                mLastPlayer = accName;
                submitForm(random);
            }
        }

        if (mLastFormTitle == "Confirm Reason")
        {
            mReportStage = 3;
            // Pick a random button (excluding the Go back button)
            size_t buttons = jsonObj["buttons"].size();
            int random = MathUtils::random(0, buttons - 2);
            submitForm(random);
        }

        // da last stage of the report menu
        if (mLastFormTitle == "Confirm Report") {
            mReportStage = 0;
            submitForm(0);
        }

    }
}

void AutoReport::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlaySound)
    {
        auto packet = event.getPacket<PlaySoundPacket>();
        if (packet->mName == "random.pop2")
        {
            // cancel if a form was open in the last second
            if (NOW - mLastFormTime < 1000)
            {
                event.cancel();
            }
        }
    };

    if (event.mPacket->getId() == PacketID::Text) {
        auto packet = event.getPacket<TextPacket>();
        // » Your report against USERNAME has been submitted.
        auto txt = packet->mMessage;
        txt = ColorUtils::removeColorCodes(txt);
        if (StringUtils::containsIgnoreCase(txt, "Your report against")) {
            // get the index between against and has
            size_t start = txt.find("against") + 8;
            size_t end = txt.find("has");
            std::string reportedPlayer = txt.substr(start, end - start);
            reportedPlayer = StringUtils::trim(reportedPlayer);
            spdlog::info("'{}' has been reported", reportedPlayer);
            mLastReportTime = NOW;
            mReportedPlayers.push_back(reportedPlayer);
        }

        if (StringUtils::containsIgnoreCase(txt, "You have already reported this user!")) {
            std::string reportedPlayer = mLastPlayer;
            spdlog::info("'{}' has been reported", reportedPlayer);
            mLastReportTime = NOW;
            mReportedPlayers.push_back(reportedPlayer);
        }

        if (StringUtils::containsIgnoreCase(txt, "List of players, or replay ID, not yet loaded.")) {
            mFinishedReporting = true;
            mLastReportTime = NOW;
            ChatUtils::displayClientMessage("Cancelled reporting");
        }
    }

    if (event.mPacket->getId() == PacketID::ModalFormRequest && !mFinishedReporting) {
        auto packet = event.getPacket<ModalFormRequestPacket>();
        nlohmann::json json = nlohmann::json::parse(packet->mJSON);
        std::string jsonStr = json.dump(4);
        mJson = packet->mJSON;
        mLastFormTime = NOW;
        mLastFormId = packet->mFormId;
        mHasFormOpen = true;

        std::string type = json.contains("type") ? json["type"] : "";
        std::string title = json.contains("title") ? json["title"] : "";

        if (type == "form" && StringUtils::containsIgnoreCase(title, "report") || type == "form" && StringUtils::containsIgnoreCase(title, "confirm reason")) {
            mIsReportMenu = true;
            event.cancel(); // Hide the report menu from the player
        }

        if (json.contains("title")) {
            mLastFormTitle = json["title"];
        } else {
            mLastFormTitle = "Unknown";
        }

        spdlog::info("Form ID {} was opened", mLastFormId);
    }

    if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto player = ClientInstance::get()->getLocalPlayer();

        auto mp = event.getPacket<MovePlayerPacket>();
        if (mp->mPlayerID == player->getRuntimeID())
            mLastTeleport = NOW;
    }

    if (event.mPacket->getId() == PacketID::ChangeDimension)
    {
        mLastDimensionChange = NOW;
    }
}

void AutoReport::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::CommandRequest)
    {
        mLastCommandTime = NOW;
    }
    // For this to work, you need to send packets using send() and not sendToServer()
    if (event.mPacket->getId() == PacketID::ModalFormResponse) {
        auto packet = event.getPacket<ModalFormResponsePacket>();
        if (mFinishedReporting)
        {
            mHasFormOpen = false;
            mLastFormTime = NOW;
            return;
        }
        mLastFormTime = NOW;
        if (packet->mFormId == mLastFormId) {
            mHasFormOpen = false;
            int buttonId = -1;
            if (packet->mJSONResponse.has_value() && packet->mJSONResponse.value().mType == MinecraftJson::ValueType::Int) {
                buttonId = packet->mJSONResponse.value().mValue.mInt;
            }
            spdlog::info("Form ID {} was closed [button {}]", mLastFormId, buttonId);
        }
    }
}
