//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#define ALLOC_STATS_MODULE_NAME "netc"
#include "SharedUtil.hpp"
#include "SharedUtil.Thread.h"
#include "SharedUtil.IntervalCounter.h"
#include "SharedUtil.IntervalCounter.hpp"


#include <net/CNet.h>
#include "CNetClient.h"
#include <core/CCoreInterface.h>

CCoreInterface * g_pCore = NULL;

extern "C" __declspec(dllexport)
unsigned long CheckCompatibility(unsigned long, unsigned long*)
{
	return 1;
}

extern "C"  __declspec(dllexport) 
CNet * InitNetInterface(CCoreInterface * core)
{
	g_pCore = core;
	return new CNetClient();
}