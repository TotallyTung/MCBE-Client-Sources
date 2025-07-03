#pragma once
#include <iomanip>
#include <filesystem>

class Utils {
public:
	static inline void toastNotification(std::string title, std::string message) {
		ToastNotification toast = ToastNotification(ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastImageAndText02));
		winrt::Windows::Data::Xml::Dom::IXmlNodeList elem = toast.Content().GetElementsByTagName(L"text");
		elem.Item(0).InnerText(winrt::to_hstring(title));
		elem.Item(1).InnerText(winrt::to_hstring(message));
		ToastNotificationManager::CreateToastNotifier().Show(toast);
	}

	static std::string format(const char* fmt, ...) {
		va_list arg;
		va_start(arg, fmt);
		char message[300];
		vsprintf_s(message, 300, fmt, arg);
		va_end(arg);
		return (std::string)message;
	};

	static std::string getRoamPath(void) {
		char* path = NULL;
		size_t length;

		_dupenv_s(&path, &length, "appdata");

		return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
	};

	static inline std::string debugPath = getRoamPath();

	static std::string getDebugPath(void) {

		auto f = std::filesystem::path(debugPath);

		if (!std::filesystem::exists(f))
			std::filesystem::create_directories(f);

		return Utils::debugPath;

	};

	static std::string getResourcePath(void) {
		auto f = std::filesystem::path(Utils::getDebugPath() + "\\Resource");

		if (!std::filesystem::exists(f))
			std::filesystem::create_directories(f);

		return Utils::getDebugPath() + "\\Resource";
	};

	static void setDebugPath(std::string path) {

		Utils::debugPath = std::string(Utils::getRoamPath() + "\\" + path);

	};

	static void debugOutput(std::string output, bool consoleMode = false) {
		if (!consoleMode)
		{
			static bool init = false;
			static auto path = Utils::getDebugPath();

			auto file = std::string(path + "\\Output.txt");

			if (!init) {
				auto f = std::filesystem::path(file);

				if (std::filesystem::exists(f))
					std::filesystem::remove(f);

				init = true;
			};

			CloseHandle(CreateFileA(file.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

			std::ofstream fStream;
			fStream.open(file.c_str(), std::ios::app);

			if (fStream.is_open())
				fStream << output << "\n" << std::endl;

			return fStream.close();
		}
		else
		{
			std::cout << output << std::endl;
		}
	};
};

template <typename T>
std::string ssstr(T t) {
	std::stringstream ss;
	ss << t;
	return ss.str();
}

namespace Game {
	extern bool Initializing;
	extern long long startMs;
	namespace RenderInfo {
		extern std::string bootLogLOL;
	}
}

void logF(const char* fmt, ...) {
	va_list arg;
	va_start(arg, fmt);

	char message[300];
	vsprintf_s(message, 300, fmt, arg);

	printf("%s", message);
	if (Game::Initializing) {
		for (auto c : (std::string)message) {
			Game::RenderInfo::bootLogLOL += c;
		}
	}
	va_end(arg);
};

inline static long long getCurrentMs()
{
	auto now = std::chrono::system_clock::now();
	auto nanos = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	return nanos.time_since_epoch().count() / 1000000;
}

void karnelLogF(const char* fmt, ...) {
	va_list arg;
	va_start(arg, fmt);

	char message[300];
	vsprintf_s(message, 300, fmt, arg);

	float second = (getCurrentMs() - Game::startMs) / 1000.f;
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(3) << second;
	if (Game::Initializing) {
		//Game::RenderInfo::bootLogLOL += Utils::format("%s[    %s] %s", GRAY, oss.str().c_str(), message);
		Game::RenderInfo::bootLogLOL += Utils::format("         %s", message);
	}
	va_end(arg);
};