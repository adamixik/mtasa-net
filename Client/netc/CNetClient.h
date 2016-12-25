//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#include <net/CNet.h>
#include <RakNet/Source/RakPeerInterface.h>

class CNetClient : public CNet
{

public:
	CNetClient();

	bool                        StartNetwork(const char* szServerHost, unsigned short usServerPort, bool bPacketTag = false);
	void                        StopNetwork(void);

	void                        SetFakeLag(unsigned short usPacketLoss, unsigned short usMinExtraPing, unsigned short usExtraPingVariance, int iKBPSLimit);

	bool                        IsConnected(void);

	void                        DoPulse(void);
	void                        Shutdown(void);

	void                        RegisterPacketHandler(PPACKETHANDLER pfnPacketHandler);

	NetBitStreamInterface*      AllocateNetBitStream(void);
	void                        DeallocateNetBitStream(NetBitStreamInterface* bitStream);
	bool                        SendPacket(unsigned char ucPacketID, NetBitStreamInterface* bitStream, NetPacketPriority packetPriority, NetPacketReliability packetReliability, ePacketOrdering packetOrdering = PACKET_ORDERING_DEFAULT);

	void                        SetClientPort(unsigned short usClientPort);
	const char *                GetConnectedServer(bool bIncludePort = false);

	bool                        GetNetworkStatistics(NetStatistics* pDest);
	const SPacketStat*          GetPacketStats(void);

	int                         GetPing(void);
	unsigned long               GetTime(void);

	const char *                GetLocalIP(void);
	void                        GetSerial(char* szSerial, size_t maxLength);

	unsigned char               GetConnectionError(void);
	void                        SetConnectionError(unsigned char ucConnectionError);

	void                        Reset(void);

	CNetHTTPDownloadManagerInterface*   GetHTTPDownloadManager(EDownloadModeType iMode);

	void                        SetServerBitStreamVersion(unsigned short usServerBitStreamVersion);
	unsigned short              GetServerBitStreamVersion(void);

	void                        GetStatus(char* szStatus, size_t maxLength);
	unsigned short              GetNetRev(void);
	unsigned short              GetNetRel(void);

	const char*                 GetNextBuffer(void);
	const char*                 GetDiagnosticStatus(void);
	void                        UpdatePingStatus(const char* szStatus, ushort& usDataRef);

	bool                        VerifySignature(const char* pData, unsigned long ulSize);

	void                        ResetStub(DWORD dwType, ...);
	void                        ResetStub(DWORD dwType, va_list);

	const char*                 GetCurrentServerId(bool bPreviousVer);
	bool                        CheckFile(const char* szType, const char* szFilename, const void* pData = nullptr, size_t sizeData = 0);

	uint                        GetExtendedErrorCode(void);
	void                        SetTimeoutTime(uint uiTimeoutTime);

	bool                        ValidateBinaryFileName(const char* szFilename);
	CBinaryFileInterface*       AllocateBinaryFile(void);
	bool                        EncryptDumpfile(const char* szClearPathFilename, const char* szEncryptedPathFilename);
	bool                        DeobfuscateScript(const char* cpInBuffer, uint uiInSize, const char** pcpOutBuffer, uint* puiOutSize, const char* szScriptName);
	void                        PostCrash(void);
	int                         SendTo(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen);

private:
	RakNet::RakPeerInterface * client;
	PPACKETHANDLER pfnPacketHandler;
	unsigned char ucConnectionError;
	unsigned char usServerBitStreamVersion;
	bool isConnected;
	RakNet::SystemAddress serverAddress;
};