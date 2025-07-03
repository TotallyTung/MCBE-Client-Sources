#include "PlayerJoin.h"

PlayerJoin::PlayerJoin() : Module("PlayerJoin", "Show players join/leave messages in the chat okay bro?", Category::MISC) {

}

static std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastAlertTime;

void PlayerJoin::onNormalTick(LocalPlayer* localPlayer) {

    if (localPlayer == nullptr) return;

    std::vector<std::string> currentPlayerList = getCurrentPlayerList();

    static const std::unordered_set<std::string> specialMods = {
      "ItzRubyRose2",
      "xyunami",
      "Modulo 7",
      "iSSMYT",
      "MobFox",
      "snow flow7010",
      "My1Legocraft",
      "xPandaLadyx",
      "HolyDeesus",
      "itsjustclar",
      "Mootburrito9652",
      "slilvy",
      "LiquidRhyx",
      "Oreoaxolotl1",
      "SloopyKatie",
      "DuckyMuffins",
      "GeeWhiizz",
      "Eeviumm",
      "Dozaiiv",
      "Mylo20032868",
      "ClubAmericas",
      "Lazinesses",
      "zBlqde1654",
      "KingK6316",
      "RedHeadGamer314",
      "MightyPieAJR",
      "Shgru",
      "ineptsky3255696",
      "mapajama",
      "itzOreoX",
      "unova1111",
      "LCdestroysU",
      "a1frostbite0100",
      "AverageLotus457",
      "puggy231",
      "itzmay3173",
      "Critly6084",
      "NotRanean",
      "Watermelon5418",
      "BassBump8",
      "allied forc3",
      "SilverStream688",
      "Ashara FoS",
      "IzukuDekuMHA",
      "XxDRXTEXxX",
      "Cookie Chaos323",
      "Wizard101",
      "Jolie Morenita",
      "AL4LA",
      "Mr Blue II",
      "HHeavennLeaa",
      "Misqkii",
      "x0Cyn",
      "GPS82017",
      "Sasuqkee",
      "QqLucaspQ",
      "kittykristyy",
      "simouxgboss",
      "delusionaltoed",
      "MyDadCraft",
      "MrDestrot",
      "RapidHitz",
      "iKzSlayerxPvP",
      "Shiny Skelly",
      "HydreonGamer",
      "LumiThePotat",
      "kuba7747",
      "SWFT Hydra",
      "PandemiA UwU",
      "Necryii",
      "Chrissel2208",
      "EchoShardss",
      "Spirilly",
    "Lazinesses",
    "xyunami",
    "NotKeru",
    "Piolin177515",
    "Modulo 7",
    "Watermelon54187",
    "Modulo_7",
    "kingK6316",
    "SoftwareSlicer",
    "Holydeesus",
    "IMP deadly",
    "IMP_deadly",
    "x Unii x",
    "x_Unii_x",
    "PlayerYouHate",
    "eua123go TCD",
    "eua123go_TCD",
    "gobbler",
    "Wintry Garli446",
    "Wintry_Garli446",
    "iSSMYT",
    "Misqkii",
    "SupposedJoker40",
    "yTzBlackkoutz",
    "StrayKids4Life",
    "Jolie_Morenita",
    "Im a Uniform",
    "Im_a_Uniform",
    "Jolie Morenita",
    "TaticSoulz",
    "chillarchy",
    "ZacknewAlt",
    "noddae",
    "AL4LA",
    "Mr Blue II",
    "Mr_Blue_II",
    "Mootburrito9652",
    "HHeavennLeaa",
    "ItzMay3173",
    "LiquidRhyx",
    "queenewfie",
    "nz9z",
    "toxica nika",
    "toxica_nika",
    "SloopyKatie",
    "cainesio",
    "SubtotalMeat1",
    "Exposed MRK",
    "Exposed_MRK",
    "omoria",
    "Snow flow7010",
    "Snow_flow7010",
    "x0Cyn",
      "cxitty",
    "mprettygirlnc23","Lazinesses", "iTzOreoX", "xyunami", "NotKeru", "Piolin177515",
        "Modulo 7", "Modulo_7", "Watermelon54187", "kingK6316", "SoftwareSlicer",
        "Holydeesus", "IMP deadly", "IMP_deadly", "x Unii x", "x_Unii_x",
        "PlayerYouHate", "eua123go TCD", "eua123go_TCD", "gobbler", "Wintry Garli446",
        "Wintry_Garli446", "iSSMYT", "Misqkii", "SupposedJoker40", "yTzBlackkoutz",
        "StrayKids4Life", "Jolie_Morenita", "Im a Uniform", "Im_a_Uniform", "Jolie Morenita",
        "TaticSoulz", "chillarchy", "ZacknewAlt", "noddae", "AL4LA", "Mr Blue II",
        "Mr_Blue_II", "Mootburrito9652", "HHeavennLeaa", "ItzMay3173", "queenewfie",
        "nz9z", "toxica nika", "toxica_nika", "SloopyKatie", "cainesio", "SubtotalMeat1",
        "Exposed MRK", "Exposed_MRK", "omoria", "Snow flow7010", "Snow_flow7010",
        "mprettygirlnc23", "mapajama", "ItzRuby Rose2", "ItzRuby_Rose2", "ISSMYT",
        "MobFox", "My1Legocraft", "xPandaLadyx", "itsjustclar", "slilvy", "Liquid Rhyx",
        "Liquid_Rhyx", "Oreoaxolot!1", "DuckyMuffins", "GeeWhiizz", "Eeviumm", "Dozaiiv",
        "Mylo20032868", "ClubAmericas", "zBlqde1654", "RedHeadGamer314", "MightyPieAJR",
        "Shgru", "ineptsky3255696", "unova1111", "LCdestroysU", "alfrostbite0100",
        "puggy231", "Critly6084", "NotRanean", "Watermelon5418", "Tanmanstyle",
        "POqsicle", "BassBump8", "allied forc3", "allied_forc3", "SilverStream688",
        "Ashara FoS", "Ashara_FoS"
    }; // current 14.02.2025 mods

    static std::unordered_set<std::string> seenPlayers;

    auto now = std::chrono::steady_clock::now();

    for (const auto& player : currentPlayerList) {
        if (seenPlayers.find(player) == seenPlayers.end()) {
            Game.DisplayClientMessage("%s[+]%s %s %sJoined!", MCTF::GREEN, MCTF::WHITE, player.c_str(), MCTF::GREEN);

            if (specialMods.find(player) != specialMods.end()) {
                Game.DisplayClientMessage("%s%s%s %s%sWARNING%s: %sMod %s Joined!", MCTF::GREEN, MCTF::BOLD, MCTF::GOLD, MCTF::RED, MCTF::BOLD, MCTF::GREEN, MCTF::GOLD, player.c_str());
                lastAlertTime[player] = now;
            }

            seenPlayers.insert(player);
        }
    }

    for (auto it = seenPlayers.begin(); it != seenPlayers.end();) {
        if (std::find(currentPlayerList.begin(), currentPlayerList.end(), *it) == currentPlayerList.end()) {
            Game.DisplayClientMessage("%s[-]%s %s %sLeft.", MCTF::RED, MCTF::WHITE, it->c_str(), MCTF::RED);

            if (specialMods.find(*it) != specialMods.end()) {
                Game.DisplayClientMessage("%s%s%s %s%sWARNING%s: %sMod %s Left!", MCTF::GREEN, MCTF::BOLD, MCTF::GOLD, MCTF::RED, MCTF::BOLD, MCTF::GREEN, MCTF::GOLD, it->c_str());
                lastAlertTime.erase(*it);
            }

            it = seenPlayers.erase(it);
        }
        else {
            ++it;
        }
    }

    for (const auto& player : seenPlayers) {
        if (specialMods.find(player) != specialMods.end()) {
            auto lastTime = lastAlertTime[player];
            if (std::chrono::duration_cast<std::chrono::minutes>(now - lastTime).count() >= 2) {
                Game.DisplayClientMessage("%s%s%s %s%sWARNING%s: %sMod %s  online!", MCTF::GREEN, MCTF::BOLD, MCTF::GOLD, MCTF::RED, MCTF::BOLD, MCTF::GREEN, MCTF::GOLD, player.c_str());
                lastAlertTime[player] = now;
            }
        }
    }
}

std::vector<std::string> PlayerJoin::getCurrentPlayerList() {
    std::vector<std::string> playerList;
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    Level* level = localPlayer->level;
    if (level) {
        const auto& playerMap = level->getPlayerMap();
        for (const auto& pair : playerMap) {
            playerList.push_back(pair.second.mName);
        }
    }
    return playerList;
}