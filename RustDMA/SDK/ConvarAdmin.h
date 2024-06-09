#pragma once
class ConvarAdmin //Convar.Admin
{
	/*
	Script.json
	   "Address": 54580088,
      "Name": "ConVar.Admin_TypeInfo",
      "Signature": "ConVar_Admin_c*"
	*/
	uint64_t Class = 0x3C1E560;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t AdminTime = 0x00;// public static float admintime;
	uint64_t AdminUnderWaterEffect = 0x11;// public static bool underwater_effect;
public:
	ConvarAdmin();
	void SetAdminTime(float value);
	void ClearVisionInWater(bool value);
};
