#include "ChatBot.h"
#include "../../../Memory/Hooks.h"
ChatBot::ChatBot() : IModule(0, Category::MISC, "Adds the client suffix") {
}
std::string Sentence2 = "My commands are tpaccept, locate, kill, hub, fact, health, ask. Prefix is !";
ChatBot::~ChatBot() {
}

const char* ChatBot::getModuleName() {
	return "ChatBot+";
}

void ChatBot::onTick(GameMode* gm) {
    auto player = Game.getLocalPlayer();
    if (player == nullptr) return;
    Vec3 playerPos = Game.getLocalPlayer()->getPos()->floor();
    if (cmd1 == true) {
        cmd1 = false;
        Utils::sendCommand("tpyes");
    }
    if (cmd2 == true) {
        cmd2 = false;
        std::stringstream ss;
        ss << "X: " << playerPos.x << " Y: " << playerPos.y << " Z: " << playerPos.z;
        Utils::sendChat(ss.str());
    }

    if (cmd3 == true) {
        Utils::sendChat("nice try bozo");
        cmd3 = false;
    }
    if (cmd4 == true) {
        Utils::sendChat("nice try nn");
        cmd4 = false;
    }
    if (cmd6 == true) {
        int healthey = player->Health;
        Utils::sendChat("HP: " + std::to_string(healthey));
        cmd6 = false;
    }
    std::string sentences2[2] = {
     "Yes.",
     "No.",
    };
    std::random_device rf;
    std::mt19937 gen2(rf());
    std::uniform_int_distribution<> dis1(0, 1);
    if (cmd7 == true) {
        cmd7 = false;
        int randomIndex = dis1(gen2);
        Utils::sendChat(sentences2[randomIndex]);
    }
#include <random>   
#include <iostream>
    std::string sentences[250] = {
     "Octopuses have three hearts and blue blood.",
     "Honey never spoils and has been found in ancient tombs.",
     "There are more stars in the universe than grains of sand on Earth.",
     "Bananas are berries, but strawberries are not.",
     "The Eiffel Tower can grow taller in the summer due to heat expansion.",
     "A day on Venus is longer than a year on Venus.",
     "Sharks have existed longer than trees.",
     "Sloths can hold their breath for up to 40 minutes underwater.",
     "Humans share 60 of their DNA with bananas.",
     "Cows have best friends and can get stressed when separated.",
     "An octopus can change its shape and color to blend in with its environment.",
     "The shortest war in history lasted 38 to 45 minutes.",
     "The Earths core is as hot as the surface of the sun.",
     "Cats have a specialized collarbone to help them land on their feet.",
     "There are over 1,500 active volcanoes in the world.",
     "In Antarctica, it can snow as little as two inches per year.",
     "The longest hiccuping spree lasted 68 years.",
     "Your body contains enough carbon to fill 9,000 pencils.",
     "The Great Wall of China is not visible from space without aid.",
     "Butterflies taste with their feet.",
     "The shortest commercial flight in the world is 57 seconds long.",
     "The world’s largest desert is the Antarctic desert.",
     "Bees can recognize human faces.",
     "The total weight of ants on Earth is greater than that of all humans.",
     "A blue whale’s tongue weighs as much as an elephant.",
     "Water can boil and freeze at the same time.",
     "A group of flamingos is called a flamboyance.",
     "The word trivia comes from the Latin for three paths.",
     "Venus is the hottest planet in the solar system.",
     "Koalas sleep up to 22 hours a day.",
     "An elephant’s trunk has more than 40,000 muscles.",
     "It rains diamonds on Jupiter and Saturn.",
     "The longest recorded flight of a chicken is 13 seconds.",
     "Honeybees can fly at 15 miles per hour.",
     "The shortest commercial flight is between two Scottish islands.",
     "A sneeze travels out of your nose at over 100 miles per hour.",
     "Cleopatra lived closer in time to the moon landing than to the pyramids.",
     "Wombats have cube-shaped poop.",
     "There is enough DNA in the human body to stretch from the sun to Pluto.",
     "The first alarm clock could only ring at 4 a.m.",
     "Cows have best friends and get stressed when they are separated.",
     "A duck’s quack does not echo.",
     "A snail can sleep for three years.",
     "The human nose can distinguish over 1 trillion different smells.",
     "The average person walks about 100,000 miles in their lifetime.",
     "The longest hiccuping spree lasted 68 years.",
     "A hummingbird's heart beats up to 1,200 times per minute.",
     "A giraffe’s neck contains only seven vertebrae, the same as humans.",
     "The longest recorded flight of a chicken is 13 seconds.",
     "The strongest muscle in the human body is the masseter.",
     "There is a jellyfish that is biologically immortal.",
     "Sloths can hold their breath for up to 40 minutes underwater.",
     "The largest living organism on Earth is a fungus in Oregon.",
     "A bolt of lightning can reach temperatures five times hotter than the sun.",
     "Tardigrades can survive in the vacuum of space.",
     "A crocodile cannot stick its tongue out.",
     "A cockroach can live for a week without its head.",
     "The average lifespan of a taste bud is 10 to 14 days.",
     "A starfish does not have a brain.",
     "Rats can laugh when tickled.",
     "The Eiffel Tower can grow by up to 6 inches in the heat.",
     "The moon has moonquakes.",
     "A human’s bones are about five times stronger than steel.",
     "Sharks have been around longer than trees.",
     "The longest recorded flight of a chicken is 13 seconds.",
     "Your stomach gets a new lining every three to four days.",
     "Humans are the only animals that blush.",
     "A jellyfish is 95 water.",
     "There are more possible iterations of a game of chess than there are atoms in the universe.",
     "Sea otters hold hands when they sleep to avoid drifting apart.",
     "A polar bear's skin is black.",
     "Penguins propose to their mates with a pebble.",
     "A hippopotamus can run faster than a human.",
     "Cows can walk upstairs but not downstairs.",
     "A hummingbird’s heart beats up to 1,200 times per minute.",
     "Lions can run at 35 miles per hour.",
     "The world’s oldest piece of chewing gum is over 9,000 years old.",
     "A spider’s silk is stronger than steel of the same width.",
     "The first computer virus was created in 1986.",
     "A blue whale’s heart weighs as much as an elephant.",
     "The only letter that doesn’t appear in any U.S. state name is Q.",
     "Wombats have cube-shaped poop.",
     "The largest volcano in the solar system is on Mars.",
     "The moon is moving away from Earth at about 1.5 inches per year.",
     "The longest-living mammal is the bowhead whale.",
     "A single teaspoon of honey represents the life work of 12 bees.",
     "An ostrich's eye is bigger than its brain.",
     "The Earth’s inner core is as hot as the surface of the sun.",
     "Mice are ticklish and can laugh when tickled.",
     "There are more fake flamingos in the world than real ones.",
     "Cows can sleep standing up, but they dream lying down.",
     "The average person spends six months of their lifetime waiting for red lights to change.",
     "The human brain can generate enough electricity to power a light bulb.",
     "A full moon is 14 times brighter than a half moon.",
     "Elephants can communicate with each other using infrasound.",
     "Jellyfish have been around for more than 500 million years.",
     "Sloths can swim up to 3 times faster than they can move on land.",
     "Cows can produce over 6 gallons of milk a day.",
     "The shortest commercial flight is between two islands in Scotland.",
     "A sneeze can travel over 100 miles per hour.",
     "Giraffes have the same number of neck vertebrae as humans.",
     "One teaspoon of honey represents the life work of 12 bees.",
     "A cow has four stomachs.",
     "The fastest fish in the world is the sailfish.",
     "Penguins mate for life.",
     "Whales can hold their breath for over 30 minutes.",
     "The longest mountain range in the world is underwater.",
     "There are more stars in the universe than grains of sand on Earth.",
     "Butterflies can taste with their feet.",
     "A chameleon can move its eyes in different directions at the same time.",
     "The human body has more than 650 muscles.",
     "The longest-lived bird is the albatross, with a lifespan of up to 60 years.",
     "There are more than 7,000 languages spoken in the world today.",
     "A group of owls is called a parliament.",
     "An octopus has three hearts.",
     "The longest word in the English language has 189,819 letters.",
     "The Great Wall of China is not visible from space with the naked eye.",
     "Cows can be trained to use a toilet.",
     "A starfish has no brain.",
     "There are more than 200 bones in the human body.",
     "A hippo's sweat is pink.",
     "Sharks have been around for more than 400 million years.",
     "A strawberry is not a berry, but a banana is.",
     "A panda spends most of its day eating bamboo.",
     "Sloths can hold their breath for up to 40 minutes underwater.",
     "The fastest bird is the peregrine falcon, reaching speeds of up to 240 miles per hour.",
     "The longest lifespan of a human was 122 years.",
     "A giraffe’s tongue is around 18 inches long.",
     "Koalas sleep up to 22 hours a day.",
     "A cheetah can run at 60 miles per hour.",
     "Rats can laugh when tickled.",
     "The longest living insect is the termite queen, living up to 50 years.",
     "An adult human body contains around 37.2 trillion cells.",
     "Your body produces about 1 to 1.5 quarts of saliva each day.",
     "A single elephant tooth can weigh as much as 6 pounds.",
     "Cats have a special reflective layer in their eyes that helps them see in the dark.",
     "Sharks are the only fish species that can blink with both eyes.",
     "A dolphin has its own signature whistle.",
     "The word nerd was first coined by Dr. Seuss in 1950.",
     "Elephants are the only animals that cannot jump.",
     "The world’s largest living organism is a fungus in Oregon.",
     "The Eiffel Tower can grow by up to 6 inches in the heat.",
     "Sharks can sense an electric field.",
     "An octopus can change its color and texture.",
     "A lightning bolt is five times hotter than the surface of the sun.",
     "The human body has more than 650 muscles."
    };
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dis(0, 249);
	if (cmd5 == true) {
		cmd5 = false;
		int randomIndex = dis(gen);
		Utils::sendChat(sentences[randomIndex]);  
	}
}

void ChatBot::onSendPacket(Packet* packet) {
}
void ChatBot::onEnable() {
	Utils::sendChat(Sentence2);
	bool cmd1 = false;
	bool cmd2 = false;
	bool cmd3 = false;
	bool cmd4 = false;
	bool cmd5 = false;
	bool filter = false;
}