//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#include "CNetHTTPDownloadManager.h"
#include <stdio.h>
#include <curl/curl.h>
#include <cpr/cpr.h>

#ifdef NET_CLIENT

#include <core/CCoreInterface.h>
extern CCoreInterface * g_pCore;

#endif

uint __fullsize = 0;
static std::queue<DownloadableFile*> downloads;

CNetHTTPDownloadManager::CNetHTTPDownloadManager()
{
	
}

uint CNetHTTPDownloadManager::GetDownloadSizeNow(void)
{
	return __fullsize;
}

void CNetHTTPDownloadManager::ResetDownloadSize(void)
{
	__fullsize = 0;
}

const char * CNetHTTPDownloadManager::GetError(void)
{
	return "";
}

bool CNetHTTPDownloadManager::ProcessQueuedFiles(void)
{
	if (downloads.size() > 0)
	{
#ifdef NET_CLIENT
		g_pCore->GetConsole()->Printf("File downloads %d\n", downloads.size());
#endif
			//g_pCore->GetConsole()->Printf("Download %d\n", i);
			DownloadableFile * file = downloads.front();
			if (file == NULL)
				return false;



			if (file->szPostData.length() != 0)
			{
				auto future_text = cpr::PostCallback([file](cpr::Response r) {
					if (r.text.length() == 0)
						return false;


					__fullsize += r.text.length();
#ifdef NET_CLIENT
					g_pCore->GetConsole()->Printf("File %s %s size: %d %s\n", *file->szOutputFile, *file->szURL, r.text.length(), r.text.c_str());
#endif
					if (file->callback)
						file->callback((char*)*SString(r.text.data(), NULL), r.text.length(), file->objectPtr, !(bool)r.error, (int)r.error.code);

					if (file->szOutputFile.length() != 0)
					{
						FILE * f = fopen(*file->szOutputFile, "wb");
						fwrite(r.text.data(), 1, r.text.length(), f);
						fclose(f);
					}
				}, cpr::Url{ file->szURL }, cpr::Body{ file->szPostData });
				//future_text.wait();
			}
			else
			{
				auto future_text = cpr::GetCallback([file](cpr::Response r) {
					if (r.text.length() == 0)
						return false;

					__fullsize += r.text.length();
#ifdef NET_CLIENT
					g_pCore->GetConsole()->Printf("File %s %s size: %d %s\n", *file->szOutputFile, *file->szURL, r.text.length(), r.text.c_str());
#endif
					if (file->callback)
						file->callback((char*)*SString(r.text.data(), NULL), r.text.length(), file->objectPtr, !(bool)r.error, (int)r.error.code);

					if (file->szOutputFile.length() != 0)
					{
						FILE * f = fopen(*file->szOutputFile, "wb");
						fwrite(r.text.data(), 1, r.text.length(), f);
						fclose(f);
					}
				}, cpr::Url{ file->szURL });
				//future_text.wait();

				


			}

			downloads.pop();
		//}
	}

	return downloads.size() == 0;
}


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool CNetHTTPDownloadManager::QueueFile(const char * szURL, const char * szOutputFile, double dSize, const char * szPostData, unsigned int uiPostSize, bool bPostBinary, void * objectPtr, PFN_DOWNLOAD_FINISHED_CALLBACK pfnDownloadFinishedCallback, bool bIsLocal, uint uiConnectionAttempts, uint uiConnectTimeoutMs, bool bCheckContents, bool bResumeFile, const char * szPostHeaders)
{
	curl_global_init(CURL_GLOBAL_ALL);

	SString url = szURL;
	if (url.Contains("multitheftauto") || url.Contains("mtasa.com"))
	{
		return true;
	}

	DownloadableFile * file = new DownloadableFile();
	file->callback = pfnDownloadFinishedCallback;
	file->objectPtr = objectPtr;
	file->szOutputFile = SString("%s", szOutputFile);
	file->szURL = SString("%s", szURL);
	file->szPostData = SString("%s", szPostData);

	downloads.push(file);

	return true;
}

void CNetHTTPDownloadManager::SetMaxConnections(int iMaxConnections)
{
}

void CNetHTTPDownloadManager::Reset(void)
{
}
