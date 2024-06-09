#pragma once
class LocalPlayer
{
	/*
	"Address": 54566200,
      "Name": "LocalPlayer_TypeInfo",
      "Signature": "LocalPlayer_c*"
	*/
	uint64_t Class = 0x3C12348;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t BasePlayerBackingField = 0x0; // private static BasePlayer <Entity>k__BackingField;
	int64_t BasePlayerValue; // the baseplayer value
public:
	LocalPlayer();
	~LocalPlayer();
	uint64_t GetBasePlayer();
	void UpdateBasePlayer(VMMDLL_SCATTER_HANDLE);
	bool IsLocalPlayerValid();
};
