//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#define ALLOC_STATS_MODULE_NAME "net"
#include "SharedUtil.hpp"
#include "SharedUtil.Thread.h"
#include "SharedUtil.IntervalCounter.h"
#include "SharedUtil.IntervalCounter.hpp"


#include <net/CNetServer.h>
#include "CNetServer.h"

extern "C" __declspec(dllexport)
unsigned long CheckCompatibility(unsigned long, unsigned long*)
{
	return 0x09E;
}

extern "C"  __declspec(dllexport) 
CNetServer * InitNetServerInterface(void)
{
	return new CNetServerImpl();
}