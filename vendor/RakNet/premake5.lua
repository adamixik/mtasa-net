--============== MTASA Network ==============
--
-- Project: RakNet
-- Author(s): RakNet authors
-- License: http://github.com/OculusVR/RakNet/
--
--===========================================

project "RakNet"
	language "C++"
	kind "StaticLib"
	targetname "raknet"
	
	defines { "_RAKNET_LIB", "_CRT_NONSTDC_NO_DEPRECATE", "_CRT_SECURE_NO_DEPRECATE" }
	
	vpaths { 
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = "premake5.lua"
	}
	
	files {
		"premake5.lua",
		"Source/*.h",
		"Source/*.cpp"
	}
	