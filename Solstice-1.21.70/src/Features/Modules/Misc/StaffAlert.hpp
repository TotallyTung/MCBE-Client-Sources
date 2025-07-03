#pragma once
//
// Created by vastrakai on 8/16/2024.
//


class StaffAlert : public ModuleBase<StaffAlert>
{
public:
    BoolSetting mStaffOnly = BoolSetting("Staff Only", "Only show events from staff members or nicked players", true);
    BoolSetting mShowNotifications = BoolSetting("Show Notifications", "Show notifications when a staff member/nicked player joins or leaves", true);
    BoolSetting mPlaySound = BoolSetting("Play Sound", "Plays a sound when a staff member/nicked player joins", true);
    BoolSetting mShowRecentJoins = BoolSetting("Show Recent Joins", "Show when a player joined for the first time within the last 24 hours", false);
    BoolSetting mSaveToDatabase = BoolSetting("Save To Database", "Save all found members to a database", false);

    StaffAlert() : ModuleBase("StaffAlert", "Automatically detects staff members", ModuleCategory::Misc, 0, false)
    {
        addSettings(
            &mStaffOnly,
            &mShowNotifications,
            &mPlaySound,
            &mSaveToDatabase,
            &mShowRecentJoins
        );

        mNames = {
            {Lowercase, "staffalert"},
            {LowercaseSpaced, "staff alert"},
            {Normal, "StaffAlert"},
            {NormalSpaced, "Staff Alert"}
        };
    }

    class PlayerInfo : public DataObject
    {
    public:
        std::string name = "";
        std::string rank = "";
        int64_t first_played;
        uint64_t storedAt = 0;

        PlayerInfo(const std::string& name, const std::string& rank, int64_t firstPlayed)
        : name(name), rank(rank), first_played(firstPlayed) {}

        int64_t getFirstJoined() const { return first_played; }

        nlohmann::json toJson() override
        {
            nlohmann::json json;
            json["name"] = name;
            json["rank"] = rank;
            json["storedAt"] = storedAt;
            return json;
        }

        void fromJson(nlohmann::json json) override
        {
            name = json["name"];
            rank = json["rank"];
            storedAt = json["storedAt"];
        }

        PlayerInfo() = default;
    };

    struct PlayerEvent
    {
        enum class Type {
            JOIN,
            LEAVE
        };

        Type type;
        std::string name{};

        PlayerEvent(const Type type, const std::string& name) : type(type), name(name) {}
    };

    std::vector<PlayerEvent> mPlayerEvents;
    std::vector<std::pair<uint64_t, std::unique_ptr<HttpRequest>>> mRequests;
    std::vector<std::string> mRequestedPlayers;
    uint64_t mLastRateLimit = 0;
    DataStore<PlayerInfo> mPlayerStore = DataStore<PlayerInfo>("staff");

    static void onHttpResponse(HttpResponseEvent event);
    [[nodiscard]] bool isPlayerCached(const std::string& name) const;
    const std::string& getRank(const std::string& name);
    int64_t getFirstJoined(const std::string& name) const;
    void makeRequest(const std::string& name);

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};