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
#include <curl/curl.h>
#include <queue>

struct DownloadableFile
{
	SString szURL;
	SString szOutputFile;
	void * objectPtr;
	SString szPostData;
	PFN_DOWNLOAD_FINISHED_CALLBACK callback;
};

#define HANDLECOUNT 256

class CNetHTTPDownloadManager : public CNetHTTPDownloadManagerInterface
{
public:
	CNetHTTPDownloadManager();

	// Get some stats regarding the current download size now & total
	uint            GetDownloadSizeNow(void);
	void            ResetDownloadSize(void);

	// Get an error if one has been set
	const char*     GetError(void);

	// Process the queued files
	// Returns true if all of the downloads have completed, false if there are additional downloads 
	bool            ProcessQueuedFiles(void);

	// Queue a file to download
	// szPostHeaders is a new line separated list of HTTP headers. Examples at https://en.wikipedia.org/wiki/List_of_HTTP_header_fields
	bool            QueueFile(const char* szURL, const char* szOutputFile, double dSize = 0, const char* szPostData = NULL, unsigned int uiPostSize = 0, bool bPostBinary = false, void * objectPtr = NULL, PFN_DOWNLOAD_FINISHED_CALLBACK pfnDownloadFinishedCallback = NULL, bool bIsLocal = false, uint uiConnectionAttempts = 10, uint uiConnectTimeoutMs = 10000, bool bCheckContents = false, bool bResumeFile = false, const char* szPostHeaders = NULL);

	// Limit number of concurrent http client connections
	void            SetMaxConnections(int iMaxConnections);

	void            Reset(void);

private:
	uint fullsize;
};