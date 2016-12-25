//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>

#include <net/CNetServer.h>

struct PlayerMap
{
	NetServerPlayerID id;
	RakNet::SystemAddress addr;
};

class CNetServerImpl : public CNetServer
{
public:
	CNetServerImpl();

	enum ENetworkUsageDirection
	{
		STATS_INCOMING_TRAFFIC = 0,
		STATS_OUTGOING_TRAFFIC = 1
	};

	// szIP can be NULL if autochoosing is wanted.
	bool                            StartNetwork(const char* szIP, unsigned short usServerPort, unsigned int uiAllowedPlayers, const char* szServerName);
	void                            StopNetwork(void);

	void                            DoPulse(void);

	void                            RegisterPacketHandler(PPACKETHANDLER pfnPacketHandler);

	bool                            GetNetworkStatistics(NetStatistics* pDest, const NetServerPlayerID& PlayerID);
	const SPacketStat*              GetPacketStats(void);
	bool                            GetBandwidthStatistics(SBandwidthStatistics* pDest);
	bool                            GetNetPerformanceStatistics(SNetPerformanceStatistics* pDest, bool bResetCounters);
	void                            GetPingStatus(SFixedString < 32 >* pstrStatus);
	bool                            GetSyncThreadStatistics(SSyncThreadStatistics* pDest, bool bResetCounters);

	NetBitStreamInterface*          AllocateNetServerBitStream(unsigned short usBitStreamVersion, const void* pData = nullptr, uint uiDataSize = 0, bool bCopyData = false);
	void                            DeallocateNetServerBitStream(NetBitStreamInterface* bitStream);
	bool                            SendPacket(unsigned char ucPacketID, const NetServerPlayerID& playerID, NetBitStreamInterface* bitStream, bool bBroadcast, NetServerPacketPriority packetPriority, NetServerPacketReliability packetReliability, ePacketOrdering packetOrdering = PACKET_ORDERING_DEFAULT);

	void                            GetPlayerIP(const NetServerPlayerID& playerID, char strIP[22], unsigned short* usPort);

	void                            Kick(const NetServerPlayerID &PlayerID);

	void                            SetPassword(const char* szPassword);

	void                            SetMaximumIncomingConnections(unsigned short numberAllowed);

	CNetHTTPDownloadManagerInterface*   GetHTTPDownloadManager(EDownloadModeType iMode);

	void                            SetClientBitStreamVersion(const NetServerPlayerID &PlayerID, unsigned short usBitStreamVersion);
	void                            ClearClientBitStreamVersion(const NetServerPlayerID &PlayerID);

	void                            SetChecks(const char* szDisableComboACMap, const char* szDisableACMap, const char* szEnableSDMap, int iEnableClientChecks, bool bHideAC, const char* szImgMods);

	unsigned int                    GetPendingPacketCount(void);
	void                            GetNetRoute(SFixedString < 32 >* pstrRoute);

	bool                            InitServerId(const char* szPath);
	void                            ResendModPackets(const NetServerPlayerID& playerID);
	void                            ResendACPackets(const NetServerPlayerID& playerID);

	void                            GetClientSerialAndVersion(const NetServerPlayerID& playerID, SFixedString < 32 >& strSerial, SFixedString < 64 >& strExtra, SFixedString < 32 >& strVersion);
	void                            SetNetOptions(const SNetOptions& options);
	void                            GenerateRandomData(void* pOutData, uint uiLength);
	bool                            EncryptDumpfile(const char* szClearPathFilename, const char* szEncryptedPathFilename) { return false; }
	bool                            ValidateHttpCacheFileName(const char* szFilename) { return true; }
	bool                            GetScriptInfo(const char* cpInBuffer, uint uiInSize, SScriptInfo* pOutInfo) { auto info = new SScriptInfo(); info->szMinClientRunVer = new char[256]; info->szMinServerHostVer = new char[256]; info->szMinServerRunVer = new char[256]; strcpy((char*)info->szMinClientRunVer, "1.3.0-9.03772"); strcpy((char*)info->szMinServerHostVer, "1.3.0"); strcpy((char*)info->szMinServerRunVer, "1.3.0");  *pOutInfo = *info;  return true; }
	bool                            DeobfuscateScript(const char* cpInBuffer, uint uiInSize, const char** pcpOutBuffer, uint* puiOutSize, const char* szScriptName) { *pcpOutBuffer = cpInBuffer; *puiOutSize = uiInSize; return true; }
	bool                            GetPlayerPacketUsageStats(uchar* packetIdList, uint uiNumPacketIds, SPlayerPacketUsage* pOutStats, uint uiTopCount) { return true; }
	const char*                     GetLogOutput(void) { return NULL; }
	bool                            IsValidSocket(const NetServerPlayerID& playerID) { return false; }

private:
	RakNet::RakPeerInterface * server;
	PPACKETHANDLER pfnPacketHandler;
	unsigned char ucConnectionError;
	unsigned char usServerBitStreamVersion;
	std::vector<PlayerMap> players;

	NetServerPlayerID GetPlayerNetId(RakNet::SystemAddress addr);
	RakNet::SystemAddress GetPlayerAddress(NetServerPlayerID id);


};

