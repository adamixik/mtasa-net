//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#include "CNetServer.h"
#include <StdInc.h>
#include <string>
//#include <net/CNet.h>
//#include "CNetClient.h"
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

CNetServerImpl::CNetServerImpl()
{
	server = RakNet::RakPeerInterface::GetInstance();
	pfnPacketHandler = NULL;
}

bool CNetServerImpl::StartNetwork(const char * szIP, unsigned short usServerPort, unsigned int uiAllowedPlayers, const char * szServerName)
{
	RakNet::SocketDescriptor socketDescriptor(usServerPort, 0);
	socketDescriptor.socketFamily = AF_INET;

	server->Startup(uiAllowedPlayers, &socketDescriptor, 1);
	server->SetMaximumIncomingConnections(uiAllowedPlayers);
	server->SetOccasionalPing(true);
	
	server->SetTimeoutTime(10000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);


	return true;
}

void CNetServerImpl::StopNetwork(void)
{
	server->Shutdown(3000);
}

void CNetServerImpl::DoPulse(void)
{
	
	for (RakNet::Packet * p = server->Receive(); p; server->DeallocatePacket(p), p = server->Receive())
	{


		const int mine = ID_USER_PACKET_ENUM;


		SNetExtraInfo * info = new SNetExtraInfo();


		info->m_bHasPing = true;
		info->m_uiPing = server->GetLastPing(p->systemAddress);

		switch (GetPacketIdentifier(p))
		{
			case ID_USER_PACKET_ENUM:
			{
			
				RakNet::BitStream * bs = new RakNet::BitStream(p->data, p->length, true);
				bs->IgnoreBytes(sizeof(unsigned char));

				unsigned char packetIdentifier = 0;
				bs->Read(packetIdentifier);


				RakNet::BitStream * bsOut = new RakNet::BitStream();
				bs->Read(bsOut);

				auto bitStream = new CNetBitStream(bsOut);

				//if(packetIdentifier == PACKET_ID_PLAYER_JOINDATA)

				if (pfnPacketHandler != NULL)
					pfnPacketHandler(packetIdentifier, GetPlayerNetId(p->systemAddress), bitStream, info);

				break;
			}
			case ID_NEW_INCOMING_CONNECTION:
			{
				//PACKET_ID_PLAYER_JOIN

				PlayerMap * map = new PlayerMap();
				map->addr = p->systemAddress;
				map->id = NetServerPlayerID(p->systemAddress.address.addr4.sin_addr.s_addr, p->systemAddress.GetPort());

				players.push_back(*map);

				RakNet::BitStream * bs = new RakNet::BitStream(p->data, p->length, true);
				bs->IgnoreBytes(sizeof(RakNet::MessageID));

				unsigned char packetIdentifier = 0;
				bs->Read(packetIdentifier);

				auto bitStream = new CNetBitStream();

				if (pfnPacketHandler != NULL)
					pfnPacketHandler(PACKET_ID_PLAYER_JOIN, GetPlayerNetId(p->systemAddress), bitStream, info);
				break;
			}

		}

//		server->DeallocatePacket(p);
	}
}

void CNetServerImpl::RegisterPacketHandler(PPACKETHANDLER pfnPacketHandler)
{
	this->pfnPacketHandler = pfnPacketHandler;
}

bool CNetServerImpl::GetNetworkStatistics(NetStatistics * pDest, const NetServerPlayerID & PlayerID)
{
	
	return true;
}

const SPacketStat * CNetServerImpl::GetPacketStats(void)
{
	return new SPacketStat();
}

bool CNetServerImpl::GetBandwidthStatistics(SBandwidthStatistics * pDest)
{
	return true;
}

bool CNetServerImpl::GetNetPerformanceStatistics(SNetPerformanceStatistics * pDest, bool bResetCounters)
{
	
	return true;
}

void CNetServerImpl::GetPingStatus(SFixedString<32>* pstrStatus)
{
	
}

bool CNetServerImpl::GetSyncThreadStatistics(SSyncThreadStatistics * pDest, bool bResetCounters)
{
	
	return true;
}

NetBitStreamInterface * CNetServerImpl::AllocateNetServerBitStream(unsigned short usBitStreamVersion, const void * pData, uint uiDataSize, bool bCopyData)
{
	return new CNetBitStream((unsigned char *)pData, uiDataSize, bCopyData);
}

void CNetServerImpl::DeallocateNetServerBitStream(NetBitStreamInterface * bitStream)
{
	delete (CNetBitStream*)bitStream;
}

bool CNetServerImpl::SendPacket(unsigned char ucPacketID, const NetServerPlayerID & playerID, NetBitStreamInterface * bitStream, bool bBroadcast, NetServerPacketPriority packetPriority, NetServerPacketReliability packetReliability, ePacketOrdering packetOrdering)
{
	RakNet::BitStream bs;
	bs.Write((unsigned char)(ID_USER_PACKET_ENUM));
	bs.Write(ucPacketID);
	bs.Write(((CNetBitStream*)bitStream)->GetBitStream());
	return server->Send(&bs, (PacketPriority)packetPriority, (PacketReliability)packetReliability, (char)packetOrdering, GetPlayerAddress(playerID), bBroadcast);
}

const char* inet_ntop(int af, const void* src, char* dst, int cnt){
 
    struct sockaddr_in srcaddr;
 
    memset(&srcaddr, 0, sizeof(struct sockaddr_in));
    memcpy(&(srcaddr.sin_addr), src, sizeof(srcaddr.sin_addr));
 
    srcaddr.sin_family = af;
    if (WSAAddressToString((struct sockaddr*) &srcaddr, sizeof(struct sockaddr_in), 0, dst, (LPDWORD) &cnt) != 0) {
        DWORD rv = WSAGetLastError();
        printf("WSAAddressToString() : %d\n",rv);
        return NULL;
    }
    return dst;
}

void CNetServerImpl::GetPlayerIP(const NetServerPlayerID & playerID, char strIP[22], unsigned short * usPort)
{
	RakNet::SystemAddress addr = GetPlayerAddress(playerID);

	addr.ToString(true, &strIP[0], ':');
	*usPort = playerID.GetPort();
}

void CNetServerImpl::Kick(const NetServerPlayerID & PlayerID)
{
	server->CloseConnection(GetPlayerAddress(PlayerID), true);
}

void CNetServerImpl::SetPassword(const char * szPassword)
{
	server->SetIncomingPassword(szPassword, strlen(szPassword));
}

void CNetServerImpl::SetMaximumIncomingConnections(unsigned short numberAllowed)
{
	server->SetMaximumIncomingConnections(numberAllowed);
}

CNetHTTPDownloadManagerInterface * CNetServerImpl::GetHTTPDownloadManager(EDownloadModeType iMode)
{
	return new CNetHTTPDownloadManager();
}

void CNetServerImpl::SetClientBitStreamVersion(const NetServerPlayerID & PlayerID, unsigned short usBitStreamVersion)
{

}

void CNetServerImpl::ClearClientBitStreamVersion(const NetServerPlayerID & PlayerID)
{
	
}

void CNetServerImpl::SetChecks(const char * szDisableComboACMap, const char * szDisableACMap, const char * szEnableSDMap, int iEnableClientChecks, bool bHideAC, const char * szImgMods)
{
}

unsigned int CNetServerImpl::GetPendingPacketCount(void)
{
	return 0;
}

void CNetServerImpl::GetNetRoute(SFixedString<32>* pstrRoute)
{
}

bool CNetServerImpl::InitServerId(const char * szPath)
{
	return true;
}

void CNetServerImpl::ResendModPackets(const NetServerPlayerID & playerID)
{
}

void CNetServerImpl::ResendACPackets(const NetServerPlayerID & playerID)
{
}

void CNetServerImpl::GetClientSerialAndVersion(const NetServerPlayerID & playerID, SFixedString<32>& strSerial, SFixedString<64>& strExtra, SFixedString<32>& strVersion)
{
	strSerial = "ABCDEF12345678900987654321FEDCBA";
	strExtra = "ABCDEF12345678900987654321FEDCBA";
	strVersion = "1.5.3-2.03772";
}

void CNetServerImpl::SetNetOptions(const SNetOptions & options)
{
}

void CNetServerImpl::GenerateRandomData(void * pOutData, uint uiLength)
{
	for (int i = 0; i < uiLength; i++)
	{
		
	}
}

NetServerPlayerID CNetServerImpl::GetPlayerNetId(RakNet::SystemAddress addr)
{
	for (auto player : players)
	{
		if (player.addr.address.addr4.sin_addr.s_addr == addr.address.addr4.sin_addr.s_addr && player.addr.GetPort() == addr.GetPort())
			return player.id;
	}

	return NetServerPlayerID();
}

RakNet::SystemAddress CNetServerImpl::GetPlayerAddress(NetServerPlayerID id)
{
	for (auto player : players)
	{
		if (player.id.GetBinaryAddress() == id.GetBinaryAddress() && player.id.GetPort() == id.GetPort())
			return player.addr;
	}

	return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
}
