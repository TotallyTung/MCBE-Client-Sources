#pragma once

class Insults : public Module
{
private:
	bool notification = true;
	bool sound = true;
	int mode = 0;

public:
    Insults(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Insults", "Misc", "Send kill messages", keybind, enabled)
    {
		registerEnumSetting("Mode", "The mode of message", { "Normal", "Custom" }, &mode);
		registerBoolSetting("Notification", "Show kill notification", &notification);
		registerBoolSetting("Sound", "Play kill sound", &sound);
	}

	void toLower(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	std::string clientname = Game::Skiddle::ClientName;

	string normalMessages[11] = {
	"I found you in task manager and I ended your process",
	"Take a shower, you smell like your grandpa's toes",
	"I'm not flying, I'm just defying gravity!",
	"Stop running, you weren't going to win",
	"Your client has stopped working",
	"I don't hack, I just " + clientname,
	"You should end svchost.exe!",
	"Abstractional Aidz",
	"Delete System32",
	"I Alt-F4'ed you",
	"Touch grass",
	};

	std::vector<std::string> readLinesFromFile(const std::string& filename) {
		std::ifstream file(filename);
		std::vector<std::string> lines;

		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				// Trim leading and trailing whitespace
				line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");

				// Check if the line is empty or starts with "//" after trimming whitespace
				if (line.empty() || line.find("//") == 0) {
					continue;
				}
				lines.push_back(line);
			}
			file.close();
		}
		else {
			ChatUtil::sendMessage(combine("Failed to open file!"));
		}

		return lines;
	}

	std::string selectRandomLine(const std::vector<std::string>& lines) {
		std::vector<std::string> validLines;
		for (const auto& line : lines) {
			if (!line.empty() && line.find("//") != 0) {
				validLines.push_back(line);
			}
		}

		if (validLines.empty()) {
			ChatUtil::sendMessage(combine("You have a Insults text? check this file! ('", clientname, "/Assets/Custom/Insults.txt')"));
		}

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<size_t> dist(0, validLines.size() - 1);

		return validLines[dist(rng)];
	}

	bool isOnlyWhitespace(const std::string& line) {
		return line.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
	}



    void onEvent(UpdateEvent* event) {
		auto player = Game::GetLocalPlayer();
		if (player == nullptr) return;
		toLower(clientname);

		int randomVal = 0;
		srand(time(NULL));

        if (Game::killed) {


			std::ifstream file(Util::getClientPath() + "Assets\\Custom\\Insults.txt");

			if (!file.is_open()) {
				ChatUtil::sendMessage(combine("Failed to open file!"));
			}

			std::stringstream buffer;
			buffer << file.rdbuf();

			std::string fileContent = buffer.str();

			if (fileContent.empty()) {
				ChatUtil::sendMessage(combine("You dont have a custom Insults message! ('", clientname, "/Assets/Custom/Insults.txt')"));
			}
			else if (isOnlyWhitespace(fileContent)) {
				ChatUtil::sendMessage(combine("You dont have a custom Insults message! ('", clientname,  "/Assets/Custom/Insults.txt')"));
			}
			else {
				file.close();

				if (sound) {
					Util::systemPlay("Sounds\\Insults\\Killed.wav");
				}
				if (notification) {
					Game::Core::noti.addNotification("You Killed:", "Player", 5.f);
				}

				TextPacket pkt;
				pkt.messageType = 1;
				pkt.translationNeeded = false;

				TextHolder msg((std::string)"Not selected Insults");

				switch (mode) {
				case 0: // Normal
					randomVal = rand() % 11;
					msg = normalMessages[randomVal];
					break;
				case 1:
					std::string filename = Util::getClientPath() + "Assets\\Custom\\Insults.txt";
					std::vector<std::string> lines = readLinesFromFile(filename);

					std::string selectedLine = selectRandomLine(lines);
					msg = selectedLine + " | .gg/JQunbC47nc";
					break;
				}

				pkt.message = msg;
				Game::GetInstance()->getPacketSender()->sendToServer(&pkt);

				Game::killed = false;
			}
        }
    }
};