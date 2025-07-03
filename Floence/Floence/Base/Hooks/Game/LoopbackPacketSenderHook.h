#pragma once

void* onLoopbackPacketSender;
void* onTextPacketDispatcher;
void* onChangeDimensionPacketDispatcher;
void* onDisconnectPacketDispatcher;
void* onLevelEventPacketDispatcher;
void* onSetActorMotionPacketDispatcher;

void LoopbackPacketSenderDetour(LoopbackPacketSender* _this, Packet* packet) {
    bool cancelled = false;
	PacketID id = (PacketID)packet->getId();

	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* pkt = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		if (pkt) {
			Global::bodyYaw = pkt->rotation.y;
			Global::headYaw = pkt->headYaw;
			Global::pitch = pkt->rotation.x;

			Global::testPosition = pkt->position;
		}
	}

	if (strcmp(packet->getTypeName().getText(), "TextPacket") == 0) {
		std::string* message = (std::string*)(__int64)((char*)packet + 0x58);
		if (message->c_str()[0] == *"." || message->c_str()[0] == *",")
		{
			CommandMgr.sendCommand(message->c_str());
			return;
		}
	}

    PacketEvent event{ _this, packet }; // PacketEvent
    event.cancelled = &cancelled;
    CallBackEvent(&event); // Call Packet event for modules to be writen on this hook.

    if (!cancelled) {
        // Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
        Memory::CallFunc<void*, LoopbackPacketSender*, Packet*>( // CallFunc to call the original.
            onLoopbackPacketSender, _this, packet
        );
    }
}

bool ContainsIgnoreCase(std::string str, std::string find) {
	std::string mainStrLower = str;
	std::string subStrLower = find;

	// Convert both mainStr and subStr to lowercase
	std::transform(mainStrLower.begin(), mainStrLower.end(), mainStrLower.begin(), ::tolower);
	std::transform(subStrLower.begin(), subStrLower.end(), subStrLower.begin(), ::tolower);

	// Perform the containment check
	return mainStrLower.find(subStrLower) != std::string::npos;
}

bool StartsWith(std::string str, std::string start) {
	return str.rfind(start, 0) == 0;
}

std::string ToLower(const std::string& string)
{
	std::string str = string;
	std::ranges::transform(str, str.begin(), ::tolower);
	return str;
}

std::string ToUpper(const std::string& string)
{
	std::string str = string;
	std::ranges::transform(str, str.begin(), ::toupper);
	return str;
}

void TextPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
	PacketID id = packet.get()->getId();

	if (id == PacketID::Text) {
		auto* pkt = reinterpret_cast<TextPacket*>(packet.get());

		std::string message = pkt->message;

		message = Utils::sanitize(message);

		std::string playerNameTag = Address::getLocalPlayer()->getNametag()->c_str();

		if (playerNameTag.find("\n") != std::string::npos)
			playerNameTag = playerNameTag.substr(0, playerNameTag.find("\n"));

		playerNameTag = Utils::sanitize(playerNameTag);

		std::string playerNameSecound = Address::getLocalPlayer()->getNametag()->c_str();
		if (playerNameSecound.find("§r") != std::string::npos) playerNameSecound.erase(playerNameSecound.find("§r"), 2);
		if (playerNameSecound.find("§l") != std::string::npos) playerNameSecound.erase(playerNameSecound.find("§l"), 2);

		std::string playerTeam = "§" + playerNameSecound.substr(playerNameSecound.find("§") + 2, 1);

		if (ContainsIgnoreCase(message, playerNameTag + " killed")) {
			std::string target = message.substr(message.find("killed") + 7, message.find(playerNameTag) - message.find("killed") - 8);
			target = Utils::sanitize(target);

			if (ContainsIgnoreCase(target, " ")) 
				target = "\"" + target + "\"";

			Global::KilledTarget = target;
			Global::Killed = true;
			//ChatUtils::sendMessage("Just swagged out a g");
		}

		if (getModuleByName("autoqueue")->isEnabled()) {
			if (ContainsIgnoreCase(pkt->message, "§c§l» §r§c§lGame OVER!") && Global::shouldQueueOnGameEnd) {
				Global::shouldQueue = true;
				ChatUtils::sendMessage("Queueing you into a game of \"" + Global::LastGame + "\"...");
			}

			if (ContainsIgnoreCase(pkt->message, "§7has been §cELIMINATED§7!") && StartsWith(pkt->message, playerTeam + "§l»") && Global::shouldQueueOnDeath) {
				Global::shouldQueue = true;
				ChatUtils::sendMessage("Queueing you into a game of \"" + Global::LastGame + "\"...");
			}

			if (ContainsIgnoreCase(pkt->message, "You are connected to server name ")) {
				std::string game = pkt->message.substr(pkt->message.find("server name ") + 12, pkt->message.length() - 1);
				game.erase(std::remove_if(game.begin(), game.end(), ::isdigit), game.end());

				Global::LastGame = ToUpper(game);

				return;
			}
			else if (ContainsIgnoreCase(pkt->message, "You are connected to server ")) {
				std::string game = pkt->message.substr(pkt->message.find("server ") + 7, pkt->message.length() - 1);

				if (std::count_if(game.begin(), game.end(), ::isdigit) > 3)
				{
					return;
				}
				game.erase(std::remove_if(game.begin(), game.end(), ::isdigit), game.end());

				Global::LastGame = ToUpper(game);

				return;
			}

			if (ContainsIgnoreCase(pkt->message, "You are connected to public IP ") || ContainsIgnoreCase(pkt->message, "You are connected to internal IP ") || ContainsIgnoreCase(pkt->message, "You are connected to proxy ")) {
				return;
			}
		}
	}

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>( // CallFunc to call the original.
		onTextPacketDispatcher, a1, networkIdentifier, netEventCallback, packet
	);
}

void ChangeDimensionPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
	PacketID id = packet.get()->getId();

	if (id == PacketID::ChangeDimension) {
		if (getModuleByName("autoqueue")->isEnabled()) {
			std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(77);
			auto* command_packet = reinterpret_cast<CommandRequestPacket*>(packet.get());
			command_packet->Command = "/connection";

			command_packet->Origin.mType = CommandOriginType::Player;

			command_packet->InternalSource = true;
			Address::getClientInstance()->getLoopbackPacketSender()->sendToServer(command_packet);
		}
	}


	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>( // CallFunc to call the original.
		onChangeDimensionPacketDispatcher, a1, networkIdentifier, netEventCallback, packet
	);
}

void DisconnectPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
	PacketID id = packet.get()->getId();

	if (id == PacketID::Disconnect) {
		if (getModuleByName("kickdecryptor")->isEnabled()) {
			auto player = Address::getLocalPlayer();

			if (!player) return;

			auto disconnectPacket = std::reinterpret_pointer_cast<DisconnectPacket>(packet);
			std::string reason = disconnectPacket->mMessage;

			// Make sure the reason starts with "Error: " (The rest of the msg is a base64 code)
		    // Remove the "Error: " part

			if (!ContainsIgnoreCase(reason, "Error: ")) return;

			reason = reason.substr(10);

			std::string playerNameTag = Address::getLocalPlayer()->getNametag()->c_str();

			if (playerNameTag.find("\n") != std::string::npos)
				playerNameTag = playerNameTag.substr(0, playerNameTag.find("\n"));

			playerNameTag = Utils::sanitize(playerNameTag);

			disconnectPacket->mMessage =
				"§c[Flareon] §7Kicked \n" + playerNameTag + "\n" + reason;

		}
	}

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>( // CallFunc to call the original.
		onDisconnectPacketDispatcher, a1, networkIdentifier, netEventCallback, packet
	);
}

void LevelEventPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
	PacketID id = packet.get()->getId();

	if (id == PacketID::LevelEvent) {
		auto lep = std::reinterpret_pointer_cast<LevelEventPacket>(packet);

		if (lep->eventId == 3600 || lep->eventId == 2001) {
			auto pos = lep->pos;
			if (getModuleByName("regen")->isEnabled()) {
				if (Global::miningPosition != pos.ToInt()) {
					if (pos.distance(Address::getLocalPlayer()->getStateVector()->Position) < 10) {
						if (Global::StealOres) {
							BlockSource* source = Address::getBlockSource();
							if (source) {
								static std::vector<Vector3<int>> checklist = {
									Vector3<int>(0, 1, 0), Vector3<int>(0, -1, 0),
									Vector3<int>(0, 0, 1), Vector3<int>(0, 0, -1),
									Vector3<int>(1, 0, 0), Vector3<int>(-1, 0, 0),
								};
								Global::stealingBlockPos = NULL;
								for (int i = 0; i < checklist.size(); i++) {
									Vector3<int> blockPos = pos.ToInt().add(checklist[i]);
									int blockId = source->getBlock(blockPos)->GetBlockLegacy()->getBlockID();
									if ((blockId == 73 || blockId == 74) && blockPos != Global::targettingBlockPos) {
										Global::stealingBlockPos = blockPos;
										break;
									}
								}
							}
						}
						if (Global::targettingBlockPos == pos.ToInt()) {
							Global::blacklistedBlockPos = pos.ToInt();
						}
					}
				}
			}
		}
		else if (lep->eventId == 3601) {
			Vector3<int> pos = lep->pos.ToInt();
			int dist = 0;
			dist += abs(Global::targettingBlockPos.x - pos.x); // X
			dist += abs(Global::targettingBlockPos.y - pos.y); // Y
			dist += abs(Global::targettingBlockPos.z - pos.z); // Z
			if (dist == 1 && Address::getBlockSource()->getBlock(pos)->GetBlockLegacy()->getBlockID() != 0) {
				if (Global::miningPosition == Global::stealingBlockPos) Global::StopStealing = true;
				Global::stealingBlockPos = NULL;
			}
			if (pos == Global::blacklistedBlockPos) Global::blacklistedBlockPos = NULL;
		}
	}

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>( // CallFunc to call the original.
		onLevelEventPacketDispatcher, a1, networkIdentifier, netEventCallback, packet
	);
}


void SetActorMotionPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
	PacketID id = packet.get()->getId();

	if (id == PacketID::SetActorMotion) {
		auto samp = std::reinterpret_pointer_cast<SetActorMotionPacket>(packet);
		if (samp->mRuntimeID == Address::getLocalPlayer()->getRuntimeID()) {
			Vector3<float> motion = samp->mMotion;

			FileUtils::debugOutput("unko");
			Global::lastLerpVelocity = motion;

			if (getModuleByName("testmodule")->isEnabled()) {
				motion = Address::getLocalPlayer()->getStateVector()->Velocity.lerp(motion, Global::testModuleFly, 0, Global::testModuleFly);
			}

			Player* player = Address::getLocalPlayer();
			if (samp->mRuntimeID == player->getRuntimeID()) {
				if (getModuleByName("KBTests")->isDisabled()) {
				if (Global::VelocityMode == 0 && getModuleByName("velocity")->isEnabled()) {
					motion = Address::getLocalPlayer()->getStateVector()->Velocity.lerp(motion, 0, 0, 0);
				}

				if (Global::VelocityMode == 1 && getModuleByName("velocity")->isEnabled()) {
					bool jumping = false;
					motion = jumping = true;
					if (jumping) {
						StateVectorComponent* state = player->getStateVector();
						player->jumpFromGround();
						state->Velocity.y = 0.f;
						state->Velocity.y += 1.f;
					}
				}
				}
			}

			if (getModuleByName("Airvelo")->isEnabled()) {

				auto player = Address::getLocalPlayer();
				if (player == nullptr) return;
				auto onGround = player->isOnGround();

				if (onGround) {
					motion = Address::getLocalPlayer()->getStateVector()->Velocity.lerp(motion, 0, 1, 0);
				}
				else {
					motion = Address::getLocalPlayer()->getStateVector()->Velocity.lerp(motion, 0, 0, 0);
				}
			}
			if (getModuleByName("KBTests")->isEnabled())
			{
				auto player = Address::getLocalPlayer();
				if (player == nullptr) return;

				//player->getStateVector()->Velocity.y = 0.f;
				motion = Address::getLocalPlayer()->getStateVector()->Velocity.lerp(motion, 6.25, 0.25, 6.25);
			}

			samp->mMotion = motion;
		}
	}

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>( // CallFunc to call the original.
		onLevelEventPacketDispatcher, a1, networkIdentifier, netEventCallback, packet
	);
}

class LoopbackPacketSenderHook : public FuncHook {
public:
	bool Initialize() override
	{
		std::shared_ptr<Packet> textPacket = MinecraftPackets::createPacket((int)PacketID::Text);

		std::shared_ptr<Packet> changeDimensionPacket = MinecraftPackets::createPacket((int)PacketID::ChangeDimension);

		std::shared_ptr<Packet> disconnectPacket = MinecraftPackets::createPacket((int)PacketID::Disconnect);

		std::shared_ptr<Packet> levelEventPacket = MinecraftPackets::createPacket((int)PacketID::LevelEvent);

		std::shared_ptr<Packet> setActorMotionPacket = MinecraftPackets::createPacket((int)PacketID::SetActorMotion);

		if (!Memory::HookFunction((void*)textPacket->packetHandlerDispatcher->vTable[1], (void*)&TextPacketDispatcherDetour, &onTextPacketDispatcher, xorstr_("TextPacketDispatcher"))) { return false; };

		if (!Memory::HookFunction((void*)changeDimensionPacket->packetHandlerDispatcher->vTable[1], (void*)&ChangeDimensionPacketDispatcherDetour, &onChangeDimensionPacketDispatcher, xorstr_("ChangeDimensionPacketDispatcher"))) { return false; };

		if (!Memory::HookFunction((void*)disconnectPacket->packetHandlerDispatcher->vTable[1], (void*)&DisconnectPacketDispatcherDetour, &onDisconnectPacketDispatcher, xorstr_("DisconnectPacketDispatcher"))) { return false; };

		if (!Memory::HookFunction((void*)levelEventPacket->packetHandlerDispatcher->vTable[1], (void*)&LevelEventPacketDispatcherDetour, &onLevelEventPacketDispatcher, xorstr_("LevelEventPacketDispatcher"))) { return false; };

		if (!Memory::HookFunction((void*)setActorMotionPacket->packetHandlerDispatcher->vTable[1], (void*)&SetActorMotionPacketDispatcherDetour, &onSetActorMotionPacketDispatcher, xorstr_("SetActorMotionPacketDispatcher"))) { return false; };

		std::thread([&] {
			while (!Address::getClientInstance() || !Address::getClientInstance()->getLoopbackPacketSender()) {
				if (!isRunning) break;

				Sleep(0);
			}
			if (!Address::getClientInstance() || !Address::getClientInstance()->getLoopbackPacketSender()) return;

			auto VTable = *(uintptr_t**)(Address::getClientInstance()->getLoopbackPacketSender());
			if (!Memory::HookFunction((void*)VTable[2], (void*)&LoopbackPacketSenderDetour, &onLoopbackPacketSender, xorstr_("LoopbackPacketSender"))) { return; };
			}).detach();

		return true;
	}

	static LoopbackPacketSenderHook& Instance() { // a class setup function called Instance.
		static LoopbackPacketSenderHook instance;
		return instance;
	}
};
