#pragma once
class ConvarGraphics
{
	/*
	Script.json
	  "Address": 54466328,
	  "Name": "ConVar.Graphics_TypeInfo",
	  "Signature": "ConVar_Graphics_c*"
	
	*/
	uint64_t Class = 0x33F1718;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t FOV = 0x18;// private static float _fov;
public:
	ConvarGraphics();
	void WriteFOV(float value);
};