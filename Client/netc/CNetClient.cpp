//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#include <string>
#include <net/CNet.h>
#include "CNetClient.h"
#include <RakNet/Source/MessageIdentifiers.h>
#include "CNetBitStream.h"
#include "CBinaryFile.h"
#include "CNetHTTPDownloadManager.h"

unsigned char GetPacketIdentifier(RakNet::Packet *p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char)p->data[0];
}

CNetClient::CNetClient()
{
	client = RakNet::RakPeerInterface::GetInstance();
	pfnPacketHandler = NULL;
	usServerBitStreamVersion = 0x0;
	isConnected = false;
}

bool CNetClient::StartNetwork(const char * szServerHost, unsigned short usServerPort, bool bPacketTag)
{
	RakNet::SocketDescriptor socketDescriptor(0, 0);
	socketDescriptor.socketFamily = AF_INET;

	client->Startup(65535, &socketDescriptor, 1);
	client->SetOccasionalPing(true);

	bool b = client->Connect(szServerHost, usServerPort, NULL, 0) == RakNet::CONNECTION_ATTEMPT_STARTED;

	return b;
}

void CNetClient::StopNetwork(void)
{
	client->Shutdown(5000);
}

void CNetClient::SetFakeLag(unsigned short usPacketLoss, unsigned short usMinExtraPing, unsigned short usExtraPingVariance, int iKBPSLimit)
{
}

bool CNetClient::IsConnected(void)
{
	return isConnected;
	//return client->GetConnectionState(RakNet::UNASSIGNED_SYSTEM_ADDRESS) == RakNet::ConnectionState::IS_CONNECTED;
}

void CNetClient::DoPulse(void)
{
	for (RakNet::Packet * p = client->Receive(); p; client->DeallocatePacket(p), p = client->Receive())
	{
		switch (GetPacketIdentifier(p))
		{
			case ID_USER_PACKET_ENUM:
			{
				RakNet::BitStream * bs = new RakNet::BitStream(p->data, p->length, true);
				bs->IgnoreBytes(sizeof(unsigned char));

				unsigned char packetIdentifier = 0;
				bs->Read(packetIdentifier);

				if (packetIdentifier == PACKET_ID_SERVER_JOIN_COMPLETE)
					usServerBitStreamVersion = 0x5;

				RakNet::BitStream * bsOut = new RakNet::BitStream();
				bs->Read(bsOut);

				auto bitStream = new CNetBitStream(bsOut);


				if (pfnPacketHandler != NULL)
					pfnPacketHandler(packetIdentifier, *bitStream);

				
				break;
			}
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
			
				isConnected = true;
				serverAddress = p->systemAddress;
				if (pfnPacketHandler != NULL)
					pfnPacketHandler(PACKET_ID_SERVER_JOIN, CNetBitStream());
			}
			case ID_CONNECTION_LOST:
			{

			}

		}

		//client->DeallocatePacket(p);
	}

}

void CNetClient::Shutdown(void)
{
	client->Shutdown(5000);
}

void CNetClient::RegisterPacketHandler(PPACKETHANDLER pfnPacketHandler)
{
	this->pfnPacketHandler = pfnPacketHandler;
}

NetBitStreamInterface * CNetClient::AllocateNetBitStream(void)
{
	return new CNetBitStream();
}

void CNetClient::DeallocateNetBitStream(NetBitStreamInterface * bitStream)
{
	delete (CNetBitStream*)bitStream;
}

bool CNetClient::SendPacket(unsigned char ucPacketID, NetBitStreamInterface * bitStream, NetPacketPriority packetPriority, NetPacketReliability packetReliability, ePacketOrdering packetOrdering)
{

	RakNet::BitStream bs;
	bs.Write((unsigned char)(ID_USER_PACKET_ENUM));
	bs.Write(ucPacketID);
	bs.Write(((CNetBitStream*)bitStream)->GetBitStream());
	return client->Send(&bs, (PacketPriority)packetPriority, (PacketReliability)packetReliability, (char)packetOrdering, serverAddress, false);
}

void CNetClient::SetClientPort(unsigned short usClientPort)
{
}

const char * CNetClient::GetConnectedServer(bool bIncludePort)
{
	return "127.0.0.1";
}

bool CNetClient::GetNetworkStatistics(NetStatistics * pDest)
{
	return false;
}

const SPacketStat * CNetClient::GetPacketStats(void)
{
	
	static SPacketStat stats;
	return &stats;
}

int CNetClient::GetPing(void)
{
	return client->GetLastPing(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
}

unsigned long CNetClient::GetTime(void)
{
	return 0;
}

const char * CNetClient::GetLocalIP(void)
{
	return "";
}

void CNetClient::GetSerial(char * szSerial, size_t maxLength)
{
	strcpy(szSerial, "0000000000");
}

unsigned char CNetClient::GetConnectionError(void)
{
	return 0;
	return ucConnectionError;
}

void CNetClient::SetConnectionError(unsigned char ucConnectionError)
{
	this->ucConnectionError = ucConnectionError;
}

void CNetClient::Reset(void)
{
	//Shutdown();
	usServerBitStreamVersion = 0x0;
	client = RakNet::RakPeerInterface::GetInstance();
	pfnPacketHandler = NULL;
}

CNetHTTPDownloadManagerInterface * CNetClient::GetHTTPDownloadManager(EDownloadModeType iMode)
{
	return new CNetHTTPDownloadManager();
}

void CNetClient::SetServerBitStreamVersion(unsigned short usServerBitStreamVersion)
{
	//this->usServerBitStreamVersion = usServerBitStreamVersion;
}

unsigned short CNetClient::GetServerBitStreamVersion(void)
{
	return usServerBitStreamVersion;
}

void CNetClient::GetStatus(char * szStatus, size_t maxLength)
{
	strcpy_s(szStatus, maxLength, "OK");
}

unsigned short CNetClient::GetNetRev(void)
{
	return 0x5;
}

unsigned short CNetClient::GetNetRel(void)
{
	return 0x5;
}

const char * CNetClient::GetNextBuffer(void)
{
	return "";
}

const char * CNetClient::GetDiagnosticStatus(void)
{
	return "OK";
}

void CNetClient::UpdatePingStatus(const char * szStatus, ushort & usDataRef)
{

}

bool CNetClient::VerifySignature(const char * pData, unsigned long ulSize)
{
	return true;
}

void CNetClient::ResetStub(DWORD dwType, ...)
{
}

void CNetClient::ResetStub(DWORD dwType, va_list)
{
}

const char * CNetClient::GetCurrentServerId(bool bPreviousVer)
{
	return "1";
}

bool CNetClient::CheckFile(const char * szType, const char * szFilename, const void * pData, size_t sizeData)
{
	return true;
}

uint CNetClient::GetExtendedErrorCode(void)
{
	return 0x0;
}

void CNetClient::SetTimeoutTime(uint uiTimeoutTime)
{
	client->SetTimeoutTime(uiTimeoutTime, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
}

bool CNetClient::ValidateBinaryFileName(const char * szFilename)
{
	return true;
}

CBinaryFileInterface * CNetClient::AllocateBinaryFile(void)
{
	return new CBinaryFile();
}

bool CNetClient::EncryptDumpfile(const char * szClearPathFilename, const char * szEncryptedPathFilename)
{
	return true;
}

bool CNetClient::DeobfuscateScript(const char * cpInBuffer, uint uiInSize, const char ** pcpOutBuffer, uint * puiOutSize, const char * szScriptName)
{
	*pcpOutBuffer = new char[uiInSize+1];
	*puiOutSize = uiInSize;
	*pcpOutBuffer = cpInBuffer;
	return true;
}

void CNetClient::PostCrash(void)
{
}

int CNetClient::SendTo(SOCKET s, const char * buf, int len, int flags, const sockaddr * to, int tolen)
{
	sendto(s, buf, len, flags, to, tolen);
	return 0;
}
