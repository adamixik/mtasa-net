--============== MTASA Network ==============
--
-- Project: MTA:SA Network
-- Author(s): adamix
-- License: MIT
--
--===========================================

project "Client Network"
	language "C++"
	kind "SharedLib"
	targetname "netc"
	targetdir(buildpath("mta"))
	
	filter "system:windows"
		links { "ws2_32", "pthread" }
		linkoptions { "/SAFESEH\:NO" }
		buildoptions { "-Zm130" }
	
	filter {}
		includedirs { 
			".",
			"../sdk",
			"../../vendor/pthreads/include",
			"../../vendor/RakNet/Source",
			"../../vendor/curl/include",
			"../../vendor/cpr/include",
			"../../Shared/net"
		}

	libdirs {
		"../../vendor/detours/lib",
		"../../Bin/Server/",
	}
	
	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	vpaths { 
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = "premake5.lua"
	}
	
	files {
		"premake5.lua",
		"**.h",
		"**.cpp",
		"../../vendor/cpr/cpr/**.cpp",
		"../../Shared/net/**.cpp",
		"../../Shared/net/**.h"
	}

	links {
		"raknet", "libcurl"
	}

	defines {
		"_WIN32_WINNT=0x502",
		"NET_CLIENT"
	}

	filter "architecture:x64"
		flags { "ExcludeFromBuild" } 
		
	filter "system:not windows"
		flags { "ExcludeFromBuild" } 
