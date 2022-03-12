#include <iostream>
#include <fstream>
#include "steam/steam_api.h"
#include <string>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

inline const char* const BoolToString(bool b)
{
  return b ? "true" : "false";
}

int main() {
    SteamAPI_Init();
    auto testdlc = 445630;
    std::cout << "We have " << SteamApps()->GetDLCCount() << " DLCs. " << std::endl;
    std::cout << BoolToString(SteamApps()->BIsDlcInstalled(testdlc)) << std::endl;
    std::cout << SteamUser()->UserHasLicenseForApp(SteamUser()->GetSteamID(), testdlc) << std::endl;
    return 0;
}