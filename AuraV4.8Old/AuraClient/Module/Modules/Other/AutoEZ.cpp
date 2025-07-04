#include "AutoEZ.h"

AutoEz::AutoEz() : IModule(0x0, Category::OTHER, "Sends a message after you kill someone") {
}

AutoEz::~AutoEz() {
}

const char* AutoEz::getModuleName() {
    return ("AutoEz");
}

static std::vector<C_Entity*> targetList;

void findEntity995(C_Entity* currentEntity, bool isRegularEntity) {
    if (currentEntity == nullptr)
        return;

    if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
        return;

    if (currentEntity->getEntityTypeId() != 63)  // player
        return;

    float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

    if (dist < 7) {
        targetList.push_back(currentEntity);
    }
}

void AutoEz::onTick(C_GameMode* gm) {
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr) return;

    // Select the insult message based on the counter
  
    
	if (counter == 0)
		RandMessage = "Why so weak?";
	else if (counter == 1)
		RandMessage = "Aura Owning";
	else if (counter == 2)
		RandMessage = "Aura On top!!!";
	else if (counter == 3)
		RandMessage = "Get better";
	else if (counter == 4)
		RandMessage = "Nice try nn";
	else if (counter == 5)
		RandMessage = "I own you ez";
	else if (counter == 6)
		RandMessage = "iqless";
	else if (counter == 7)
		RandMessage = "your face makes onions cry";
	else if (counter == 8)
		RandMessage = "AuraV4 owns you";
	else if (counter == 9)
		RandMessage = "you bring everyone so much joy! you know, when you leave the room. but, still";
	else if (counter == 10)
		RandMessage = "you are missing a brain";
	else if (counter == 11)
		RandMessage = "You have been crushed by my big balls";
	else if (counter == 12)
		RandMessage = "Want some free skills? check out discord.gg/mRGZpPST4m";
	else if (counter == 13)
		RandMessage = "You're so ugly, your portraits hang themselves";
	else if (counter == 14)
		RandMessage = "Ever wondered why your iq is so low? It is because you are inbred";
	else if (counter == 15)
		RandMessage = "Retarded nn nice try kid";
	else if (counter == 16)
		RandMessage = "you do be lookin' kinda bad at the game";
	else if (counter == 17)
		RandMessage = "Aura power!!!!";
	else if (counter == 18)
		RandMessage = "You are like a lightswitch, even a child can turn you on and off";
	else if (counter == 19)
		RandMessage = "you really like taking L's";
	else if (counter == 20)
		RandMessage = "Team Aura Owns All";
	else if (counter == 21)
		RandMessage = "You were never going to win :(";
	else if (counter == 22)
		RandMessage = "you are the type of person to think FOV increases reach";
	else if (counter == 23)
		RandMessage = "i'd insult you after that death but it's funnier to let you imagine what i'm calling you";
	else if (counter == 24)
		RandMessage = "Your mother is so fat wen she jumped in the ocean the whales started singing We Are Family.";
	else if (counter == 25)
		RandMessage = "Why are you so bad?";
	else if (counter == 26)
		RandMessage = "Too simple for Aura :))";
	else if (counter == 27)
		RandMessage = "Osami wants to tell you: kys";
	else if (counter == 28)
		RandMessage = "Damn you are so bad";
	else if (counter == 29)
		RandMessage = "so bAd";
	else if (counter == 30)
		RandMessage = "here's your tickets to the juice world concert";
	else if (counter == 31)
		RandMessage = "Here's 5 cents, call all your friends and give me back the change.";
	else if (counter == 32)
		RandMessage = "Black monkey so EZ";
	else if (counter == 33)
		RandMessage = "Go back to Africa";
	else if (counter == 34)
		RandMessage = "You are the most sold item in the 18th century";
	else if (counter == 35)
		RandMessage = "Even your mom is better than you in this game";
	else if (counter == 36)
		RandMessage = "It looks like your face caught on fire and someone tried to put it out with a fork.";
	else if (counter == 37)
		RandMessage = "Why can't I see you in the dark?";
	else if (counter == 38)
		RandMessage = "You are so black that you become invisible in the dark";
	else if (counter == 39)
		RandMessage = "you deserved to be bhopped on";
	else if (counter == 40)
		RandMessage = "I'd smack you, but that would be animal abuse";
	else if (counter == 41)
		RandMessage = "respond in 3 seconds or gay";
	else if (counter == 42)
		RandMessage = "Ah yes all team on me i dare you";
	else if (counter == 43)
		RandMessage = "So ez";
	else if (counter == 44)
		RandMessage = "Imagine dying LLL";
	else if (counter == 45)
		RandMessage = "EZZZZZZZZZZ";
	else if (counter == 46)
		RandMessage = "discord.gg/mKjH8gvx";
	else if (counter == 47)
		RandMessage = "Stay in the server if gay";
	else if (counter == 48)
		RandMessage = "Your ability to make everyone around you throw up is truly something to behold";
	else if (counter == 49)
		RandMessage = "When you rely on a shitty client";
	else if (counter == 50)
		RandMessage = "Is this why you only have 363 days on your calender?";
	else if (counter == 51)
		RandMessage = "You fucking suck";
	else if (counter == 52)
		RandMessage = "Learn your alphabet with AuraV4 Devs: Afar Arkaris, Nakeanl, Zh1n1anQwQ, Man Lay214, xIhyna, Iammtrashhh, DualNebul090112";
	else if (counter == 53)
		RandMessage = "Download AuraV4 to kick asses while listening to some aura boosting music!";
	else if (counter == 54)
		RandMessage = "Kid, did you finish your homework?";
	else if (counter == 55)
		RandMessage = "Download AuraV4 today to kill everyone";
	else if (counter == 56)
		RandMessage = "Free loot tpa @here";
	else if (counter == 57)
		RandMessage = "LMFAO SO EZ";
	else if (counter == 58)
		RandMessage = "Personally I think even a vanilla can beat you";
	else if (counter == 59)
		RandMessage = "Ur mom sucking my d!ck";
	else if (counter == 60)
		RandMessage = "Neigaaaa";
	else if (counter == 61)
		RandMessage = "Cope more NN";
	else if (counter == 62)
		RandMessage = "Shut it retarded nn";
	else if (counter == 63)
		RandMessage = "AuraV4 is now your new client";
	else if (counter == 63)
		RandMessage = "In need of a cute present for Christmas? AuraV4 is all you need!";
	else if (counter == 64)
		RandMessage = "I have a good AuraV4 config, don't blame me";
	else if (counter == 65)
		RandMessage = "Don't piss me off or you will discover the true power of AuraV4";
	else if (counter == 66)
		RandMessage = "AuraV4 user never dies";
	else if (counter == 67)
		RandMessage = "You just helped an AuraV4 user increase their aura, thanks";
	else if (counter == 68)
		RandMessage = "Obliterated by Aura";
	else if (counter == 69)
		RandMessage = "Dropped";
	else if (counter == 70)
		RandMessage = "Buy AuraV4 Private today to get good";
	else if (counter == 71)
		RandMessage = "Click on this to get free skills: discord.gg/mRGZpPST4m";
	else if (counter == 72)
		RandMessage = "Order free baguettes with Aura client";
	else if (counter == 73)
		RandMessage = "Want to beat Aura user? Awww man too bad you will never have the chance to";
	else if (counter == 74)
		RandMessage = "Aura PvP Client boosts your skill 100%";
	else if (counter == 75)
		RandMessage = "Mojang wants to know AuraV4 Owner's location [Accept] [Deny]";
	else if (counter == 76)
		RandMessage = "Awa Power!!!!!";
	else if (counter == 77)
		RandMessage = "Get good. Get Aura.";
	else if (counter == 78)
		RandMessage = "AuraV4: Sponsored by HoshinoLoveee!!!";
	else if (counter == 79)
		RandMessage = "I am just better than you, accept this fact";
	else if (counter == 80)
		RandMessage = "Momoi owns you black guy";
	else if (counter == 81)
		RandMessage = "Learn my client's name first before thinking of beating me: A. U. R. A. V. 4.";
	else if (counter == 82)
		RandMessage = "Imagine using anything but AuraV4";
	else if (counter == 83)
		RandMessage = "Skill given by AuraV4 deved by Afar Arkaris";
	else if (counter == 84)
		RandMessage = "Don't forget to beg me for Aura in discord dms!";
	else if (counter == 85)
		RandMessage = "Don't use Aura? Ok weak";
	else if (counter == 86)
		RandMessage = "Just use Aura and get good like me";
	else if (counter == 87)
		RandMessage = "You know you gotta get Aura when you constantly get beat up by AuraV4";
	else if (counter == 88)
		RandMessage = "Fly faster than light, Killaura stronger than all client, only available in Aura?";
	else if (counter == 89)
		RandMessage = "Behind every Aura user, is an incredibly cool human being. Trust me, cooler than you :))";
	else if (counter == 90)
		RandMessage = "Your client sucks, just get Aura";
	else if (counter == 91)
		RandMessage = "AuraV4 made this world a better place, killing you with it even more";
	else if (counter == 92)
		RandMessage = "Stop being a disapointment to your parents and download AuraV4!";
	else if (counter == 93)
		RandMessage = "you've been killed by a AuraV4 user, rejoice!";
	else if (counter == 94)
		RandMessage = "My AuraV4 special fridge door screen edition is just too good";
	else if (counter == 95)
		RandMessage = "Man you suck LOL";
	else if (counter == 96)
		RandMessage = "SO EZZZZZZZZZZ";
	else if (counter == 97)
		RandMessage = "Cry";
	else if (counter == 98)
		RandMessage = "Your weak client can never compare to Aura!";
	else if (counter == 99)
		RandMessage = "IGN Owned By Aura";
	else if (counter == 100)
		RandMessage = "#AuraOwnsAll";
	else if (counter == 101)
		RandMessage = "AuraV4 is the best client for IGN!";
	else if (counter == 102)
		RandMessage = "Be like the guy who just downloaded Aura!";
	else if (counter == 103)
		RandMessage = "You're so weak, even a monkey can beat you";
	else if (counter == 104)
		RandMessage = "Get AuraV4 noob!";
	else if (counter == 105)
		RandMessage = "AL-1s wants to tell you that you are very bad";
	else if (counter == 106)
		RandMessage = "What do your clothes have in common with your skills? they're both straight out of a dumpster";
	else if (counter == 107)
		RandMessage = "Cry all you want, your father monkey George Floyd died of a fentanyl overdose";
	else if (counter == 108)
		RandMessage = "You should quit right now";
	if (counter == 109)
		RandMessage = "trash";
	else if (counter == 110)
		RandMessage = "Stop dying and get AuraV4";
	else if (counter == 111)
		RandMessage = "Probably even a Toolbox user can kill you, weak";
	else if (counter == 112)
		RandMessage = "NN get Ez'd by Aura";
	else if (counter == 113)
		RandMessage = "Your difficulty settings must be stuck on easy";
	else if (counter == 114)
		RandMessage = "Just stop breathing and do the George Floyd please";
	else if (counter == 115)
		RandMessage = "Try again after you downloaded AuraV4";
	else if (counter == 116)
		RandMessage = "Too easy for me";
	else if (counter == 117)
		RandMessage = "Your skill make me vomit";
	else if (counter == 118)
		RandMessage = "You are probably on Horion";
	else if (counter == 119)
		RandMessage = "Weak f a g g o t";
	else if (counter == 120)
		RandMessage = "Some kids were dropped at birth, but you got thrown at the wall";
	else if (counter == 121)
		RandMessage = "you're the type of guy to get quickdropped irl";
	else if (counter == 122)
		RandMessage = "Why so trash?";
	else if (counter == 123)
		RandMessage = "You're the type to get 3rd place in a 1v1";
	else if (counter == 124)
		RandMessage = "you have an IQ lower than that of a bathtub";
	else if (counter == 125)
		RandMessage = "Your parents abandoned you, then the orphanage did the same";
	else if (counter == 126)
		RandMessage = "you go to the doctors and they say you are hopeless";
	else if (counter == 127)
		RandMessage = "AuraV4, drop kicking weaks and nns since 2023";
	else if (counter == 128)
		RandMessage = "Momoi Neigaaa gif";
	else if (counter == 129)
		RandMessage = "Your mother so fat, she sat on an iphone and it became an ipad";
	else if (counter == 130)
		RandMessage = "Search up blue waffle on google, it's so cute";
	else if (counter == 131)
		RandMessage = "You smell like a moldy ball sack";
	else if (counter == 132)
		RandMessage = "Some people are abandoned by their parents due to various reasons, you are the type of guy to get abandoned because of the way you look";
	else if (counter == 133)
		RandMessage = "Welcome to my RapeDungeon! Population: Only you";
	else if (counter == 134)
		RandMessage = "Gay nn get ez by Aura";
	else if (counter == 135)
		RandMessage = "Stupid kid just tried to challenge the power of Aura. Result: he failed";
	else if (counter == 136)
		RandMessage = "Ezzzzzzzzzzzz #TeamAura";
	else if (counter == 137)
		RandMessage = "Get owned by Meow Meow Aura Squad";
	else if (counter == 138)
		RandMessage = "Get out of the basement please";
	else if (counter == 139)
		RandMessage = "AuraV4. Killing retarded nns since 2023.";
	else if (counter == 140)
		RandMessage = "Owned by Team NyAura";
	else if (counter == 141)
		RandMessage = "AuraV4 on TOP";
	else if (counter == 142)
		RandMessage = "Borion moment";
	else if (counter == 143)
		RandMessage = ":rofl: Nice try buddy";
	else if (counter == 144)
		RandMessage = "you should get Aura";
	else if (counter == 145)
		RandMessage = "You are the reason why birth control was a thing";
	else if (counter == 146)
		RandMessage = "Clown";
	else if (counter == 147)
		RandMessage = "Go breath under water";
	else if (counter == 148)
		RandMessage = "Iq?";
	else if (counter == 149)
		RandMessage = "Monkey, go back to Africa";
	else if (counter == 150)
		RandMessage = "Why are you crying because of a block game?";
	else if (counter == 151)
		RandMessage = "NNs can't talk and you are one of them";
	else if (counter == 152)
		RandMessage = "Owned. #TeamAura";
	else if (counter == 153)
		RandMessage = "Get rid of your client right now and use AuraV4";
	else if (counter == 154)
		RandMessage = "You just got killed by an AuraV4 user!! #TeamAuraOnTop";
	else if (counter == 155)
		RandMessage = "Spell my name right, A-F-A-R, A-R-K-A-R-I-S";
	else if (counter == 156)
		RandMessage = "AuraV4. Best client for PvP. Your number one choice.";// Add more messages as needed

    // Reset the counter if it exceeds the maximum limit (100 in this case)
    if (counter >= 157) {
        counter = 0;
    }

    // Increment counter for each tick
    counter++;

    // Clear previous targets
    targetList.clear();

    // Find nearby players
    g_Data.forEachEntity(findEntity995);

    // Check if a target is found and alive
    if (!targetList.empty() && g_Data.isInGame()) {
        C_Entity* targetEntity = targetList[0];

        // Check if the target is alive and has been hit (damage time > 1)
        if (targetEntity->isAlive() && targetEntity->height > 1.5f && targetEntity->damageTime > 1) {
            foundPlayer = true;
        }

        // Once the player is confirmed dead (and other conditions are met), send the message
        if (foundPlayer && !targetEntity->isAlive() && targetEntity->getDeathTime() >= 0) {
            // Send kill message
            C_TextPacket textPacket;
            textPacket.message.setText(RandMessage);
            textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
            textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
            g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);

            // Reset flags
            foundPlayer = false;
        }
    }

    // If no targets are present, reset the flags
    if (targetList.empty()) {
        foundPlayer = false;
    }
}
