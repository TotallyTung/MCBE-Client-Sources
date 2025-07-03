#pragma once

class TestModule : public Module
{
public:
    TestModule() :
        Module("TestModule", "Misc", "Testing")
    {
        
    }
private:
    float mCurrentHealth = 20.f;

public:
    void onEnabled() override
    {
        if (!InstanceManager::getLocalPlayer())
            return;

        Player* mPlayer = InstanceManager::getLocalPlayer();

        mCurrentHealth = mPlayer->getAttribute(AttributeId::Health)->mCurrentValue;
    }

    void onEvent(RenderContextEvent* event) override
    {
        if (!InstanceManager::getLocalPlayer())
            return;

        RenderUtils::draw2D(InstanceManager::getLocalPlayer(), 2);
    }

    void onEvent(BaseTickEvent* event) override
    {
        if (!InstanceManager::getLocalPlayer())
            return;

        Player* mPlayer = InstanceManager::getLocalPlayer();
        StateVectorComponent* mState = mPlayer->getStateVectorComponent();
        AABBShapeComponent* mAABB = mPlayer->getAABBShapeComponent();



        /*
        mAABB->mHitbox = Vector2<float>(0, 0);
        mPlayer->getAttribute(AttributeId::Health)->mCurrentMinValue = mCurrentHealth;
        mPlayer->getAttribute(AttributeId::Health)->mCurrentMaxValue = mCurrentHealth;
        mPlayer->getAttribute(AttributeId::Health)->mCurrentValue = mCurrentHealth;

        auto mep = MinecraftPackets::createPacket(PacketID::SetHealth);
        auto* pkt = reinterpret_cast<SetHealthPacket*>(mep.get());

        pkt->mHealth = mCurrentHealth;

        InstanceManager::get<ClientInstance>()->getLoopbackPacketSender()->sendToServer(pkt);

        Console::debug("Min Health: " + std::to_string(mPlayer->getAttribute(AttributeId::Health)->mCurrentMinValue));
        Console::debug("Max Health: " + std::to_string(mPlayer->getAttribute(AttributeId::Health)->mCurrentMaxValue));
        Console::debug("Current Health: " + std::to_string(mPlayer->getAttribute(AttributeId::Health)->mCurrentValue));
        */
    }

    void onDisabled() override
    {
        
    }
};