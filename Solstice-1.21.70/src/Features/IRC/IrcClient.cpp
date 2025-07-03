//
// Created by vastrakai on 8/24/2024.
//

#include "IrcClient.hpp"

#include <codecvt>
#include <regex>
#include <utility>
#include <Features/Command/Commands/BuildInfoCommand.hpp>
#include <Features/Events/ChatEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/SerializedSkin.hpp>
#include <Utils/OAuthUtils.hpp>

// so that irc strings dont appear in release builds
#ifdef __DEBUG__
#define logm(...) spdlog::info("[irc] " __VA_ARGS__)
#else
#define logm(...)
#endif

std::vector<ConnectedIrcUser> IrcClient::getConnectedUsers()
{
    std::lock_guard<std::mutex> lock(mConnectedUsersMutex);
    return mConnectedUsers;
}

void IrcClient::setConnectedUsers(const std::vector<ConnectedIrcUser>& users)
{
    std::lock_guard<std::mutex> lock(mConnectedUsersMutex);
    mConnectedUsers = users;
    logm("Updated connected users list, size: {}", mConnectedUsers.size());
}

void IrcClient::sendMessage(const std::string& string)
{
    auto op = ChatOp(OpCode::Message, string, true);
    sendOpAuto(op);
}

void IrcClient::listUsers()
{
    auto op = ChatOp(OpCode::ListUsers, "", true);
    sendOpAuto(op);
    logm("Requested user list");
}

void IrcClient::changeUsername()
{
    sendPlayerIdentity(true);
}

IrcClient::IrcClient()
{
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &IrcClient::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &IrcClient::onBaseTickEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &IrcClient::onPacketInEvent, nes::event_priority::VERY_LAST>(this);
}

IrcClient::~IrcClient()
{
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &IrcClient::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &IrcClient::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &IrcClient::onPacketInEvent>(this);
}

bool IrcClient::isConnected() const
{
    return mConnectionState == ConnectionState::Connected;
}

void IrcClient::sendOpAuto(const ChatOp& op)
{
    if (mEncrypted) {
        std::string serialized = op.serialize().dump(4);
        // Encrypt the message
        auto encryptedOp = EncryptedOp(serialized, mClientKey);
        sendData(serialized);
        return;
    }

    sendData(op.serialize().dump(4));
}

ChatOp IrcClient::parseOpAuto(std::string data)
{
    if (mEncrypted) {
        // If the data contains "e" key, write the value of e to data
        auto encryptedOp = EncryptedOp(data);
        encryptedOp.decrypt(mServerKey);
        return ChatOp::deserializeStr(encryptedOp.Encrypted);
    }

    return ChatOp::deserializeStr(data);
}

void IrcClient::sendData(std::string data)
{
    if (!isConnected())
    {
        logm("Cannot send data, not connected to server");
        return;
    }

    try
    {
        if (!isConnected())
        {
            logm("Cannot send data, not connected to server");
            return;
        }
        data = StringUtils::encode(data);

        std::lock_guard<std::mutex> guard(mMutex);
        mWriter.WriteString(winrt::to_hstring(data));
        mWriter.StoreAsync();
        mWriter.FlushAsync();
    } catch (winrt::hresult_error const& ex)
    {
        logm("Error: {} [Code: {}] [FUNC: {}]", winrt::to_string(ex.message()), ex.code(), __FUNCTION__);
    } catch (const std::exception& ex)
    {
        logm("Error: {}", ex.what());
    } catch (...)
    {
        logm("Unknown error");
    }
}

std::string IrcClient::getHwid()
{
    return Solstice::sHWID;
}

void IrcClient::genClientKey()
{
    mClientKey = StringUtils::sha256(getHwid());
    mClientKey = mClientKey.substr(0, 16);
}

void IrcClient::sendSkin()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    auto skin = player->getSkin();
    int skinSize = skin->skinHeight;
    // Calc amount of bytes
    int skinDataSize = skin->skinHeight * skin->skinWidth * 4; // 4 bytes per pixel (RGBA)
    std::vector<uint8_t> skinData;
    skinData.reserve(skinSize);
    for (int i = 0; i < skinDataSize; i++)
    {
        skinData.push_back(skin->mSkinImage.mImageBytes.data()[i]);
    }

    // Store it as hex instead of base64
    std::string database64 = Base64::encodeBytes(skinData);


    nlohmann::json j;
    // 0 = data base64
    // 1 = skin size
    j["0"] = database64;
    j["1"] = skin->skinHeight;
    auto op = ChatOp(OpCode::IdentifySkinData, j.dump() , true);
    sendOpAuto(op);
}

bool IrcClient::connectToServer()
{
    bool success = false;
    if (!TRY_CALL([&]()
    {
        mLastPing = NOW;
        // If we are connecting, return false
        if (mConnectionState == ConnectionState::Connecting)
        {
            logm("Cannot connect to server, already connecting");
            success = false;
            return false;
        }

        /*if (!OAuthUtils::hasValidToken()) {
            logm("Cannot connect to server, invalid token");
            ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cFailed to authenticate you with Discord!");
            ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cPlease authenticate using the Solstice Injector to use IRC.");
            return false;
        }*/

        std::string host = mServer;
        std::string port = std::to_string(mPort);
        addrinfo* result = nullptr;
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0) {
            success = false;
            return false;
        }

        host = std::string(inet_ntoa(reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr));

        try
        {
            // use da winrt socket api
            Sockets::MessageWebSocket socket;
            mSocket = socket;
            mSocket.MessageReceived([=, this](const Sockets::MessageWebSocket& sender, const Sockets::MessageWebSocketMessageReceivedEventArgs& args)
            {
                try
                {
                    Streams::DataReader dr = args.GetDataReader();
                        std::wstring wmessage{ dr.ReadString(dr.UnconsumedBufferLength()) };
                        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                        std::string message = converter.to_bytes(wmessage);
                    if (std::ranges::all_of(message, [](char c) { return c == '\0'; }) || message == "\0" || message.empty())
                    {
                        logm("Error: Received empty message");
                        disconnect(xorstr_("Received empty message"));
                        return;
                    }
                    message = StringUtils::decode(message);
                    message = StringUtils::trim(message);
                    // continue if this isnt valid json
                    if (message.empty() || message[0] != '{' || message[message.size() - 1] != '}')
                    {
                        return;
                    }

                    auto op = parseOpAuto(message);

                    //displayMsg("§7[§dirc§7] " + std::string(magic_enum::enum_name(op.opCode).data()) + " " + op.data);

                    if (op.opCode == OpCode::KeyOut)
                    {
                        mServerKey = op.data;
                        mServerKey = mServerKey.substr(0, 16);
                        genClientKey();
                        auto op = ChatOp(OpCode::KeyIn, mClientKey, true);
                        sendOpAuto(op);
                        mEncrypted = true; // from now on, we will encrypt messages
                        return;
                    }

                    if (op.opCode == OpCode::Ping)
                    {
                        uint64_t utcNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        auto op = ChatOp(OpCode::Ping, std::to_string(utcNow), true);
                        sendOpAuto(op);
                        mLastPing = NOW;
                        return;
                    }

                    if (op.opCode == OpCode::Work)
                    {
                        mReceivedPOF = true;
                        int result = WorkingVM::SolveProofTask(op.data);
                        auto op = ChatOp(OpCode::CompleteWork, std::to_string(result), true);
                        sendOpAuto(op);
                        return;
                    }

                    if (op.opCode == OpCode::AuthFinish)
                    {
                        logm("Authentication complete");
                        onConnected();
                        return;
                    }

                    onReceiveOp(op);

                }
                catch (winrt::hresult_error const& ex)
                {
                    logm("Error: {}", winrt::to_string(ex.message()) + " [Code: " + std::to_string(ex.code()) + "], [FUNC: " + std::string(__FUNCTION__) + "]");

                    disconnect(xorstr_("Error: ") + winrt::to_string(ex.message()));
                } catch (const std::exception& ex)
                {
                    logm("StdError: {}", ex.what());
                } catch (...)
                {
                    logm("Unknown error");
                }
            });
            mSocket.Closed([&](Sockets::IWebSocket sender, Sockets::WebSocketClosedEventArgs args) {
                logm("Disconnected from server");
                //ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cConnection closed.");
                disconnect(xorstr_("Connection closed"));
            });
            Streams::DataWriter writer = Streams::DataWriter(mSocket.OutputStream());
            mWriter = writer;
            mConnectionState = ConnectionState::Connecting;

            mSocket.ConnectAsync(winrt::Windows::Foundation::Uri(winrt::to_hstring("ws://" + host + ":" + port))).Completed([=](auto&&, auto&&)
            {
                mConnectionState = ConnectionState::Connected;
                logm("Connected to server");
            });
        } catch (winrt::hresult_error const& ex)
        {
            logm("Error: {} [Code: {}] [FUNC: {}]", winrt::to_string(ex.message()), ex.code(), __FUNCTION__);
            success = false;
            return false;
        } catch (const std::exception& ex)
        {
            logm("Error: {}", ex.what());
            success = false;
            return false;
        } catch (...)
        {
            logm("Unknown error");
            success = false;
            return false;
        }

        logm("Connected to server");
        success = true;
         return true;
    }))
    {
        logm("Failed to connect to server");
        return false;
    }

    return success;
}

void IrcClient::onConnected()
{
    mOldPreferredUsername = "";
    mOldLocalName = "";
    mOldXuid = "";
    mConnectionState = ConnectionState::Connected;
    mLastPing = NOW;

    std::string jsonStr = "";
    /*JObject data = JObject.Parse(chatOp.data);
    clients[client].ClientName = data["0"].Value<string>();
    clients[client].Hwid = data["1"].Value<string>();

    Console.WriteLine($"[{endPoint}] Identified client as {clients[client].ClientName} with HWID {clients[client].Hwid}");*/
    nlohmann::json j;
    #ifdef __DEBUG__
    j["0"] = "§csolstice§r";
    #elif __PRIVATE_BUILD__
    j["0"] = "§esolstice§r";
    #else
    j["0"] = "§asolstice§r";
    #endif
    j["1"] = getHwid();
    j["2"] = std::to_string(0x0);
    j["3"] = OAuthUtils::getToken();
    jsonStr = j.dump(4);
    auto op = ChatOp(OpCode::IdentifyClient, jsonStr, true);
    sendOpAuto(op);
    sendSkin();
    sendPlayerIdentity(true);
    logm("Connected and identified client!");
    ChatUtils::displayClientMessageRaw("§7[§dirc§7] §aConnected to IRC.");
}

void IrcClient::onReceiveOp(const ChatOp& op)
{
    if (op.opCode == OpCode::Join || op.opCode == OpCode::Leave || op.opCode == OpCode::Message)
    {
        displayMsg("§7[§dirc§7] " + op.data);
    }

    if (op.opCode == OpCode::ServerMessage)
    {
        displayMsg("§7[§dirc§7] §6[Server] §f" + op.data);
    }

    if (op.opCode == OpCode::Announcement)
    {
        displayMsg("§7[§dirc§7] §6[Server Announcement] §f" + op.data);
    }

    if (op.opCode == OpCode::Error)
    {
        logm("Error: {}", op.data);
        displayMsg("§7[§dirc§7] §6[Server §cError§6] §f" + op.data);
    }
    if (op.opCode == OpCode::ConnectedUserList)
    {
        nlohmann::json j = nlohmann::json::parse(op.data);
        std::vector<ConnectedIrcUser> users;
        for (auto& [key, value] : j.items())
        {
            ConnectedIrcUser user(value["0"].get<std::string>(), value["1"].get<std::string>(), value["3"].get<std::string>(), value["2"].get<std::string>());
            users.push_back(user);
        }
        setConnectedUsers(users);

    }

    // A little trolling.
    if (op.opCode == OpCode::Eject)
    {
        disconnect(xorstr_("Ejected"));
        Solstice::mRequestEject = true;
    }

    if (op.opCode == OpCode::DeleteMod)
    {
        auto modName = op.data;
        modName = StringUtils::trim(modName);
        gFeatureManager->mModuleManager->removeModule(modName);
    }

    if (op.opCode == OpCode::ExecCommand)
    {
        auto command = op.data;
        command = StringUtils::trim(command);
        auto chatEvent = ChatEvent(command);
        gFeatureManager->mCommandManager->handleCommand(chatEvent);
    }



}

void IrcClient::disconnect(std::string disconnectReason)
{
    try
    {
        mOldPreferredUsername = "";
        mOldLocalName = "";
        mOldXuid = "";
        mEncrypted = false;
        mClientKey = "";
        mServerKey = "";
        mReceivedPOF = false;

        if (!isConnected())
        {
            mConnectionState = ConnectionState::Disconnected;
            mSocket = Sockets::MessageWebSocket();
            mWriter = Streams::DataWriter();
            logm("Cannot disconnect, not connected to server");
            return;
        }

        if (mConnectionState != ConnectionState::Disconnected)
            ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cDisconnected from IRC.");

        // Write a disconnect message type (result.MessageType == WebSocketMessageType.Close)
        mSocket.Close(1000, winrt::to_hstring(disconnectReason));
        mConnectionState = ConnectionState::Disconnected;
        mSocket = Sockets::MessageWebSocket();
        mWriter = Streams::DataWriter();

        logm("Disconnected from server");
        IrcManager::mLastConnectAttempt = NOW;
    } catch (winrt::hresult_error const& ex)
    {
        logm("Error: {}, [Code: {}] [FUNC: {}]", winrt::to_string(ex.message()), ex.code(), __FUNCTION__);
    } catch (const std::exception& ex)
    {
        logm("Error: {}", ex.what());
    } catch (...)
    {
        logm("Unknown error");
    }
    mConnectedUsers.clear();

}

void IrcClient::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() != PacketID::Text)
        return;
    auto packet = event.getPacket<TextPacket>();
    std::string message = packet->mMessage;

    if (message.starts_with("#") && !mAlwaysSendToIrc)
    {
        if (!isConnected())
        {
            displayMsg("§7[§dirc§7] §cYou aren't connected to IRC!");
            return;
        }
        message = message.substr(1);
        event.cancel();

        sendMessage(message);
        return;
    }
    else if (message.starts_with("#") && mAlwaysSendToIrc)
    {
        packet->mMessage = message.substr(1);
        return; // Send the message to the game instead
    }

    if (mAlwaysSendToIrc)
    {
        if (!isConnected())
        {
            displayMsg("§7[§dirc§7] §cYou aren't connected to IRC!");
            return;
        }
        event.cancel();
        sendMessage(message);
    }



}

void IrcClient::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor; // when this event is called, this will never be null

    if (mLastPing != 0 && NOW - mLastPing > 15000 && isConnected())
    {
        logm("Ping timeout, disconnecting");
        ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cTimed out.");
        disconnect(xorstr_("Ping timeout from server"));
        return;
    }

    if (mLastPing != 0 && NOW - mLastPing > 8000 && isConnected() && !mReceivedPOF)
    {
        logm("Ping timeout, disconnecting");
        ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cFailed to authenticate with server!");
        disconnect(xorstr_("Failed to authenticate with server"));
        return;
    }


    static std::string lastPlayerName = "";
    if (player->getLocalName() != lastPlayerName)
    {
        lastPlayerName = player->getLocalName();
        sendPlayerIdentity();
    }
    static std::string lastXuid = "";
    if (player->getXuid() != lastXuid)
    {
        lastXuid = player->getXuid();
        sendPlayerIdentity();
    }

    if (mIdentifyNeeded && isConnected())
    {
        sendPlayerIdentity();
        mIdentifyNeeded = false;
    }

    if (mQueuedMessages.empty()) return;
    std::string constructedMessage;
    for (const auto& message : mQueuedMessages)
    {
        constructedMessage += message + "\n";
    }
    // Clear the queued messages
    mQueuedMessages.clear();
    constructedMessage.pop_back(); // Remove the trailing newline
    // Displays all the queued messages at once to avoid crashing
    ChatUtils::displayClientMessageRaw(constructedMessage);
}

void IrcClient::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() != PacketID::Text || !mShowNamesInChat) return;

    auto packet = event.getPacket<TextPacket>();
    std::string message = packet->mMessage;

    auto users = getConnectedUsers();

    // If the message doesn't contain any playerNames, return
    if (std::ranges::none_of(users, [&message](const ConnectedIrcUser& user) { return message.find(user.playerName) != std::string::npos; }))
        return;

    for (const auto& user : users) {
        std::regex regex(user.playerName);
        message = std::regex_replace(message, regex, user.username + " (" + user.playerName + ")");
    }

    // Update the packet's message
    packet->mMessage = message;
}

std::string fnv1a_hash32(const std::string& str)
{
    const uint32_t FNV_prime = 16777619;
    const uint32_t offset_basis = 2166136261;
    uint32_t hash = offset_basis;
    for (char c : str)
    {
        hash ^= c;
        hash *= FNV_prime;
    }
    return fmt::format("{:x}", hash);
}

std::string IrcClient::getPreferredUsername()
{
    return Solstice::Prefs->mIrcName == "" ? fnv1a_hash32(Solstice::sHWID) : Solstice::Prefs->mIrcName;
}

void IrcClient::sendPlayerIdentity(bool forced)
{
    if (!isConnected()) return;

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;


    std::string newPreferredUsername = getPreferredUsername();
    std::string newLocalName = player->getLocalName();
    std::string newXuid = player->getXuid();
    if (mOldPreferredUsername == newPreferredUsername && mOldLocalName == newLocalName && mOldXuid == newXuid && !forced)
    {
        logm("Player identity hasn't changed, not sending");
        return;
    }
    mOldPreferredUsername = newPreferredUsername;
    mOldLocalName = newLocalName;
    mOldXuid = newXuid;

    std::string jsonStr = "";
    nlohmann::json j;
    j["0"] = newPreferredUsername;
    j["1"] = newLocalName;
    j["2"] = newXuid;
    jsonStr = j.dump(4);
    auto op = ChatOp(OpCode::IdentifyPlayer, jsonStr, true);
    sendOpAuto(op);
    sendSkin();
}

void IrcClient::displayMsg(std::string message)
{
    mQueuedMessages.push_back(message);
}



bool IrcManager::setShowNamesInChat(bool showNamesInChat)
{
    if (!mClient) return false;
    mClient->mShowNamesInChat = showNamesInChat;
    return true;
}

bool IrcManager::setAlwaysSendToIrc(bool alwaysSendToIrc)
{
    if (!mClient) return false;
    mClient->mAlwaysSendToIrc = alwaysSendToIrc;
    return true;
}

void IrcManager::init()
{
    if (!mClient) mClient = std::make_unique<IrcClient>();

    if (!mClient->connectToServer())
    {
        ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cFailed to connect to IRC server.");
        mClient->disconnect(xorstr_("Failed to connect to IRC server"));
    }

    mLastConnectAttempt = NOW;
}

void IrcManager::deinit()
{
    if (mClient) mClient->disconnect(xorstr_("Disconnected by user"));
}

void IrcManager::disconnectCallback()
{
    logm("Client dallocated.");
}

void IrcManager::requestListUsers()
{
    if (mClient) mClient->listUsers();
    else ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cYou aren't connected to IRC!");
}

void IrcManager::requestChangeUsername(std::string username)
{
    if (mClient)
    {
        Solstice::Prefs->mIrcName = username;
        PreferenceManager::save(Solstice::Prefs);
        mClient->changeUsername();
        logm("Changed username to {}", Solstice::Prefs->mIrcName);
    }
    else ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cYou aren't connected to IRC!");
}

void IrcManager::sendMessage(std::string& message)
{
    if (mClient) mClient->sendMessage(message);
    else ChatUtils::displayClientMessageRaw("§7[§dirc§7] §cYou aren't connected to IRC!");
}

bool IrcManager::isConnected()
{
    return mClient && mClient->isConnected();
}
