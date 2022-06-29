#include <iostream>
#include <fstream>
#include "ext/steam/isteamapps.h"
#include "ext/steam/isteamuser.h"
#include "spdlog/spdlog.h"
#include "ext/ini.h"
#include <string>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

vector<tuple<int, string>> dlcs;

mINI::INIStructure ini;

class Hookey_SteamApps_Class : public ISteamApps {
public:
    bool BIsSubscribed() { return true; }
    bool BIsLowViolence() { return false; }
    bool BIsCybercafe() { return false; }
    bool BIsVACBanned() { return false; }
    int GetDLCCount() {
        spdlog::info("ISteamApps->GetDLCCount called");
        auto count = dlcs.size();
        auto content = dlcs;
        return dlcs.size();
    }
    bool BIsDlcInstalled(AppId_t appID) {
        spdlog::info("ISteamApps->BIsDlcInstalled called");
        auto reslt = std::find_if(
            std::begin(dlcs),
            std::end(dlcs),
            [&] (const tuple<int, string> a) { return std::get<0>(a) == appID; }) != std::end(dlcs);
        if (reslt) {
            spdlog::info("BIsDlcInstalled unlocked {}", appID);
            return true;
        } else {
            return false;
        }
    }
    bool BGetDLCDataByIndex(int iDLC, AppId_t* pAppID, bool* pbAvailable, char* pchName, int cchNameBufferSize) {
        spdlog::info("ISteamApps->BGetDLCDataByIndex called");
        if ((size_t)iDLC >= dlcs.size()) {
            return false;
        }

        *pAppID = std::get<0>(dlcs[iDLC]);
        *pbAvailable = true;

        const char* name = std::get<1>(dlcs[iDLC]).c_str();
        size_t slen = std::min((size_t)cchNameBufferSize - 1, std::get<1>(dlcs[iDLC]).size());
        memcpy((void*)pchName, (void*)name, slen);
        *(pchName + slen) = 0x0;

        return true;
    }

    const char* GetCurrentGameLanguage() { return real_steamApps->GetCurrentGameLanguage(); }
    const char* GetAvailableGameLanguages() { return real_steamApps->GetAvailableGameLanguages(); }
    CSteamID GetAppOwner() { return real_steamApps->GetAppOwner(); }
    int GetAppBuildId() { return real_steamApps->GetAppBuildId(); }
    void RequestAllProofOfPurchaseKeys() { real_steamApps->RequestAllProofOfPurchaseKeys(); }
    bool BIsSubscribedFromFamilySharing() { return real_steamApps->BIsSubscribedFromFamilySharing(); }
    bool BIsSubscribedFromFreeWeekend() { return real_steamApps->BIsSubscribedFromFreeWeekend(); }
    bool BIsSubscribedApp(AppId_t appID) { 
        spdlog::info("ISteamApps->BIsSubscribedApp called");
        if (ini["methods"]["disable_steamapps_issubscribedapp"] == "true") {
            spdlog::info("BIsSubscribedApp function override disabled");
            return real_steamApps->BIsSubscribedApp(appID); 
        } else {
            spdlog::info("BIsSubscribedApp creamified called");
            auto reslt = std::find_if(
                std::begin(dlcs),
                std::end(dlcs),
                [&] (const tuple<int, string> a) { return std::get<0>(a) == appID; }) != std::end(dlcs);
            if (reslt) {
                spdlog::info("BIsSubscribedApp unlocked {}", appID);
                return true;
            } else {
                if (ini["config"]["issubscribedapp_on_false_use_real"] == "true") {
                    return real_steamApps->BIsSubscribedApp(appID); 
                }
                return false;
            }
        }
    }
    bool BIsAppInstalled(AppId_t appID) { return real_steamApps->BIsAppInstalled(appID); }
    uint32 GetEarliestPurchaseUnixTime(AppId_t appID) { return real_steamApps->GetEarliestPurchaseUnixTime(appID); }
    void InstallDLC(AppId_t appID) { real_steamApps->InstallDLC(appID); }
    void UninstallDLC(AppId_t appID) { real_steamApps->UninstallDLC(appID); }
    void RequestAppProofOfPurchaseKey(AppId_t appID) { real_steamApps->RequestAppProofOfPurchaseKey(appID); }
    bool GetCurrentBetaName(char* pchName, int cchNameBufferSize) { return real_steamApps->GetCurrentBetaName(pchName, cchNameBufferSize); }
    bool MarkContentCorrupt(bool bMissingFilesOnly) { return real_steamApps->MarkContentCorrupt(bMissingFilesOnly); }

    uint32 GetInstalledDepots(AppId_t appID, DepotId_t* pvecDepots, uint32 cMaxDepots) { return real_steamApps->GetInstalledDepots(appID, pvecDepots, cMaxDepots); }
    uint32 GetAppInstallDir(AppId_t appID, char* pchFolder, uint32 cchFolderBufferSize) { return real_steamApps->GetAppInstallDir(appID, pchFolder, cchFolderBufferSize); }
    const char* GetLaunchQueryParam(const char* pchKey) { return real_steamApps->GetLaunchQueryParam(pchKey); }
    bool GetDlcDownloadProgress(AppId_t nAppID, uint64* punBytesDownloaded, uint64* punBytesTotal) { return real_steamApps->GetDlcDownloadProgress(nAppID, punBytesDownloaded, punBytesTotal); }
    SteamAPICall_t GetFileDetails(const char* pszFileName) { return real_steamApps->GetFileDetails(pszFileName); }
    int GetLaunchCommandLine(char* pszCommandLine, int cubCommandLine) { return real_steamApps->GetLaunchCommandLine(pszCommandLine, cubCommandLine); }
	virtual bool BIsTimedTrial( uint32* punSecondsAllowed, uint32* punSecondsPlayed ) { return real_steamApps->BIsTimedTrial(punSecondsAllowed, punSecondsPlayed); } 
    ISteamApps* real_steamApps;
};
class Hookey_SteamUser_Class : public ISteamUser {
public:
	HSteamUser GetHSteamUser() {
        return real_steamUser->GetHSteamUser();
    };
	bool BLoggedOn() {
        return real_steamUser->BLoggedOn();
    };
	CSteamID GetSteamID() {
        return real_steamUser->GetSteamID();
    };
	int InitiateGameConnection_DEPRECATED( void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer, bool bSecure ) {
        return real_steamUser->InitiateGameConnection_DEPRECATED(pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    };
	void TerminateGameConnection_DEPRECATED( uint32 unIPServer, uint16 usPortServer ) {
        return real_steamUser->TerminateGameConnection_DEPRECATED(unIPServer, usPortServer);
    };
    int InitiateGameConnection( void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer, bool bSecure ) {
        return real_steamUser->InitiateGameConnection_DEPRECATED(pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    };
	void TerminateGameConnection( uint32 unIPServer, uint16 usPortServer ) {
        return real_steamUser->TerminateGameConnection_DEPRECATED(unIPServer, usPortServer);
    };
	void TrackAppUsageEvent( CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo = "" ) {
        return real_steamUser->TrackAppUsageEvent(gameID, eAppUsageEvent, pchExtraInfo);
    };
	bool GetUserDataFolder( char *pchBuffer, int cubBuffer ) {
        return real_steamUser->GetUserDataFolder(pchBuffer, cubBuffer);
    };
	void StartVoiceRecording( ) {
        return real_steamUser->StartVoiceRecording();
    };
	void StopVoiceRecording( ) {
        return real_steamUser->StopVoiceRecording();
    };
	EVoiceResult GetAvailableVoice( uint32 *pcbCompressed, uint32 *pcbUncompressed_Deprecated, uint32 nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return real_steamUser->GetAvailableVoice(pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    };
	EVoiceResult GetVoice( bool bWantCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten, bool bWantUncompressed_Deprecated, void *pUncompressedDestBuffer_Deprecated, uint32 cbUncompressedDestBufferSize_Deprecated, uint32 *nUncompressBytesWritten_Deprecated, uint32 nUncompressedVoiceDesiredSampleRate_Deprecated ) {
        return real_steamUser->GetVoice(bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    };
	EVoiceResult DecompressVoice( const void *pCompressed, uint32 cbCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten, uint32 nDesiredSampleRate ) {
        return real_steamUser->DecompressVoice(pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    };
	uint32 GetVoiceOptimalSampleRate() {
        return real_steamUser->GetVoiceOptimalSampleRate();
    };
	HAuthTicket GetAuthSessionTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket ) {
        return real_steamUser->GetAuthSessionTicket(pTicket, cbMaxTicket, pcbTicket);
    };
	EBeginAuthSessionResult BeginAuthSession( const void *pAuthTicket, int cbAuthTicket, CSteamID steamID ) {
        return real_steamUser->BeginAuthSession(pAuthTicket, cbAuthTicket, steamID);
    };
	void EndAuthSession( CSteamID steamID ) {
        return real_steamUser->EndAuthSession(steamID);
    };
	void CancelAuthTicket( HAuthTicket hAuthTicket ) {
        return real_steamUser->CancelAuthTicket(hAuthTicket);
    };
	EUserHasLicenseForAppResult UserHasLicenseForApp( CSteamID steamID, AppId_t appID ) {
        spdlog::info("ISteamUser->UserHasLicenseForApp {} called", appID);
        auto reslt = std::find_if(
            std::begin(dlcs),
            std::end(dlcs),
            [&] (const tuple<int, string> a) { return std::get<0>(a) == appID; }) != std::end(dlcs);
        if (reslt) {
            spdlog::info("ISteamUser_UserHasLicenseForApp result: owned");
            return (EUserHasLicenseForAppResult)0;
        } else {
            spdlog::info("ISteamUser_UserHasLicenseForApp result: not owned");
            return (EUserHasLicenseForAppResult)2;
        }
    };
	bool BIsBehindNAT() {
        return real_steamUser->BIsBehindNAT();
    };
	void AdvertiseGame( CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer ) {
        return real_steamUser->AdvertiseGame(steamIDGameServer, unIPServer, usPortServer);
    };
	SteamAPICall_t RequestEncryptedAppTicket( void *pDataToInclude, int cbDataToInclude ) {
        return real_steamUser->RequestEncryptedAppTicket(pDataToInclude, cbDataToInclude);
    };
	bool GetEncryptedAppTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket ) {
        return real_steamUser->GetEncryptedAppTicket(pTicket, cbMaxTicket, pcbTicket);
    };
	int GetGameBadgeLevel( int nSeries, bool bFoil ) {
        return real_steamUser->GetGameBadgeLevel(nSeries, bFoil);
    };
	int GetPlayerSteamLevel() {
        return real_steamUser->GetPlayerSteamLevel();
    };
	SteamAPICall_t RequestStoreAuthURL( const char *pchRedirectURL ) {
        return real_steamUser->RequestStoreAuthURL(pchRedirectURL);
    };
	bool BIsPhoneVerified() {
        return real_steamUser->BIsPhoneVerified();
    };
	bool BIsTwoFactorEnabled() {
        return real_steamUser->BIsTwoFactorEnabled();
    };
	bool BIsPhoneIdentifying() {
        return real_steamUser->BIsPhoneIdentifying();
    };
	bool BIsPhoneRequiringVerification() {
        return real_steamUser->BIsPhoneRequiringVerification();
    };
	SteamAPICall_t GetMarketEligibility() {
        return real_steamUser->GetMarketEligibility();
    };
	virtual SteamAPICall_t GetDurationControl() {
        return real_steamUser->GetDurationControl();
    }
	virtual bool BSetDurationControlOnlineState( EDurationControlOnlineState eNewState ) {
        return real_steamUser->BSetDurationControlOnlineState(eNewState);
    };
    ISteamUser* real_steamUser;
};
static std::shared_ptr<Hookey_SteamApps_Class> steamapps_instance;

ISteamApps* Hookey_SteamApps(ISteamApps* real_steamApps) {
    if (steamapps_instance != NULL) {
        ISteamApps* ptraccess = steamapps_instance.get();
        auto debg = ptraccess->GetDLCCount();
        return steamapps_instance.get();
    } else {
        Hookey_SteamApps_Class nhooky;
        nhooky.real_steamApps = real_steamApps;
        steamapps_instance = std::make_shared<Hookey_SteamApps_Class>(nhooky);
        return Hookey_SteamApps(real_steamApps);
    }
}

static std::shared_ptr<Hookey_SteamUser_Class> steamuser_instance;

ISteamUser* Hookey_SteamUser(ISteamUser* real_steamUser) {
    if (steamuser_instance != NULL) {
        return steamuser_instance.get();
    } else {
        Hookey_SteamUser_Class nhooky;
        nhooky.real_steamUser = real_steamUser;
        steamuser_instance = std::make_shared<Hookey_SteamUser_Class>(nhooky);
        return Hookey_SteamUser(real_steamUser);
    }
}
#define STEAMUSER_INTERFACE_VERSION_OLD "SteamUser020"
extern "C" void* S_CALLTYPE SteamInternal_FindOrCreateUserInterface(HSteamUser hSteamUser, const char *pszVersion) {
    void* S_CALLTYPE (*real)(HSteamUser hSteamUser, const char *pszVersion);
    *(void**)(&real) = dlsym(RTLD_NEXT, "SteamInternal_FindOrCreateUserInterface");
    spdlog::info("SteamInternal_FindOrCreateUserInterface called pszVersion: {}", pszVersion);
    // Steamapps Interface call is hooked here
    if (strstr(pszVersion, STEAMAPPS_INTERFACE_VERSION) == pszVersion) {
        ISteamApps* val = (ISteamApps*)real(hSteamUser, pszVersion);
        spdlog::info("SteamInternal_FindOrCreateUserInterface hooked ISteamApps");
        return Hookey_SteamApps(val);
    }

    // Steamuser interface call is hooked here
    if (strstr(pszVersion, STEAMUSER_INTERFACE_VERSION) == pszVersion) {
        ISteamUser* val = (ISteamUser*)real(hSteamUser, pszVersion);
        spdlog::info("SteamInternal_FindOrCreateUserInterface ISteamUser hook");
        return Hookey_SteamUser(val);
    }
    if (strstr(pszVersion, STEAMUSER_INTERFACE_VERSION_OLD) == pszVersion) {
        ISteamUser* val = (ISteamUser*)real(hSteamUser, pszVersion);
        spdlog::info("SteamInternal_FindOrCreateUserInterface ISteamUser(legacy) hook");
        return Hookey_SteamUser(val);
    }
    auto val = real(hSteamUser, pszVersion);
    return val;
}

extern "C" EUserHasLicenseForAppResult SteamAPI_ISteamUser_UserHasLicenseForApp(CSteamID steamID, AppId_t appId) {
    // LOG(TRACE) << "SteamAPI_ISteamUser_UserHasLicenseForApp called!!" << endl;
    spdlog::info("ISteamUser_UserHasLicenseForApp called");
    return (EUserHasLicenseForAppResult)0;
}
// for older games
extern "C" ISteamApps *S_CALLTYPE SteamApps() {
    spdlog::info("SteamApps() called");

    //get isteamapps
    void* S_CALLTYPE (*real)();
    *(void**)(&real) = dlsym(RTLD_NEXT, "SteamApps");
    ISteamApps* val = (ISteamApps*)real();

    return Hookey_SteamApps(val);
}
// for older games
extern "C" ISteamUser *S_CALLTYPE SteamUser() {
    spdlog::info("SteamUser() called");

    //get isteamuser 
    void* S_CALLTYPE (*real)();
    *(void**)(&real) = dlsym(RTLD_NEXT, "SteamUser");
    ISteamUser* val = (ISteamUser*)real();
    
    return Hookey_SteamUser(val);
}
// anti-debugger shenanigans
long ptrace(int request, int pid, void *addr, void *data) {
    return 0;
}


static bool hooked = false;
static bool SteamClient_did_hook;
static bool SteamApps_did_hook;

extern "C" bool SteamAPI_Init() {

    std::string creaminipath = "cream_api.ini";
    auto env = std::getenv("CREAM_CONFIG_PATH");
    //f env exists, use it
    if (env != NULL) {
        creaminipath = env;
    }
    spdlog::info("Reading config from {}", creaminipath);
    mINI::INIFile file(creaminipath);

    // Open ini file
    file.read(ini);
    // Find dlc's and add to vector
    for (pair<string,string> entry : ini["dlc"]) {
        auto dlctuple = std::make_tuple(stoi(entry.first), entry.second);
        dlcs.push_back(dlctuple);
        spdlog::info("Added dlc with id: {0}, name: {1}", entry.first, entry.second);
    }
    spdlog::info("SteamAPI_Init called");
    // finish api call
    // the spaghetti below this comment is calling the original Init function
    // can probably be simplified but i'm no c++ expert 
    bool (*real)();
    *(void**)(&real) = dlsym(RTLD_NEXT, "SteamAPI_Init");
    auto retval = real();
    spdlog::info("SteamAPI_Init returned");
    return retval;
}


int main() {
    return 0;
}
