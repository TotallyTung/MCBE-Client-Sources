//
// Created by alteik on 30/10/2024.
//

#define EXPIRE_YEAR 2024
#define EXPIRE_MONTH 12
#define EXPIRE_DAY 15 // we should update it sometimes

struct SysTime {
    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
};

class Auth
{
public:

    std::string UniqueID = "";
    std::string uniqueIdFile = FileUtils::getSolsticeDir() + xorstr_("uniqueId.txt");
    static inline std::string url = xorstr_("https://dllserver.solstice.works/hasPrivateAccess?uniqueId=");
    static inline std::string url2 = xorstr_("https://www.timeapi.io/api/Time/current/zone?timeZone=UTC");

    void init();
    void exit();
    bool isPrivateUser();
    static SysTime getOnlineTime();
};

#define isTimeSyncedCheck() \
{ \
auto onlineTime = Auth::getOnlineTime(); \
SYSTEMTIME time; \
GetSystemTime(&time); \
if (time.wYear != onlineTime.wYear || time.wMonth != onlineTime.wMonth || time.wDay != onlineTime.wDay || time.wHour != onlineTime.wHour || time.wMinute != onlineTime.wMinute) { \
__fastfail(1); \
} \
}

#define killSwitchIfNeeded() \
{ \
SYSTEMTIME st; \
GetSystemTime(&st); \
if (st.wYear > EXPIRE_YEAR) { \
__fastfail(1); \
} \
if (st.wYear == EXPIRE_YEAR && st.wMonth > EXPIRE_MONTH) { \
__fastfail(1); \
} \
if (st.wYear == EXPIRE_YEAR && st.wMonth == EXPIRE_MONTH && st.wDay >= EXPIRE_DAY) \
{ \
__fastfail(1); \
} \
}