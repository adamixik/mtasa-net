//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#ifdef NET_CLIENT
#include <net/CNet.h>
#elif defined NET_SERVER
#include <net/CNetServer.h>
#else
#error "meow"
#endif
#include <stdio.h>

class CBinaryFile : public CBinaryFileInterface 
{
public:
	bool        FOpen(const char* szFilename, const char* szMode, bool bValidate);
	void        FClose(void);
	bool        FEof(void);
	void        FFlush(void);
	int         FTell(void);
	void        FSeek(int iOffset, int iOrigin);
	int         FRead(void* pData, uint uiSize);
	int         FWrite(const void* pData, uint uiSize);

private:
	FILE * file;
};