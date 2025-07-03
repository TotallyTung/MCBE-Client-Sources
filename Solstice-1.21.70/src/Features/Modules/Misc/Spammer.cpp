//
// 8/30/2024.
//

#include "Spammer.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <SDK/Minecraft/World/Level.hpp>

Spammer::Spammer(): ModuleBase("Spammer", "Automatically sends a chat message in specified delay", ModuleCategory::Misc, 0, false)
{
    addSetting(&mMode);
    addSetting(&mDelayMs);
    addSetting(&mAvoidKicks);
    addSetting(&mAddedDelay);

    mNames = {
        {Lowercase, "spammer"},
        {LowercaseSpaced, "spammer"},
        {Normal, "Spammer"},
        {NormalSpaced, "Spammer"}
    };
}

void Spammer::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Spammer::onBaseTickEvent>(this);
    mLastMessageSent = NOW;
}

void Spammer::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Spammer::onBaseTickEvent>(this);
}

void Spammer::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    if (NOW <= mLastMessageSent + mDelayMs.mValue) return;

    std::string entry = mSpammerMessageTemplate.getEntry();

    std::vector<std::string> mentions;

    for (auto& [uuid, p] : *player->getLevel()->getPlayerList())
    {
        if (p.mId == player->getActorUniqueIDComponent()->mUniqueID) continue;
        if (p.mName.contains(player->getRawName())) continue;

        // format mention
        std::string name = p.mName;
        if (mName.contains(" ")) name = "\"" + name + "\"";
        name = "@" + name;

        mentions.push_back(name);
    }

    static int mentionIndex = 0;
    int maxMentionIndex = mentions.size() - 1;


    // For each !randMention! in the message, replace it with a random player's name
    while (StringUtils::contains(entry, "!randMention!"))
    {
        if (maxMentionIndex == -1) break;

        if (mentionIndex > maxMentionIndex) mentionIndex = 0;

        entry = StringUtils::replace(entry, "!randMention!", mentions[mentionIndex]);
        mentionIndex++;
    }

    // replace leftover mentions with empty string
    entry = StringUtils::replaceAll(entry, "!randMention!", "");

    PacketUtils::sendChatMessage(entry);
    mLastMessageSent = NOW;
}

void Spammer::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::Text)
    {
        std::string localName = player->getRawName();

        auto tp = event.getPacket<TextPacket>();
        if (mAvoidKicks.mValue && StringUtils::containsIgnoreCase(tp->mMessage, localName))
            mLastMessageSent = NOW + mAddedDelay.mValue;
    }
}