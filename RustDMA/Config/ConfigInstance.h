#pragma once
#include "Pch.h"
#include "Misc.h"
#include "PlayerEspConfig.h"

class ConfigInstances
{
public:
	MiscConfig Misc = MiscConfig(LIT("Misc"));
	PlayerEspConfig PlayerESP = PlayerEspConfig(LIT("PlayerESP"));


	json ToJson()
	{
		json jsoned;
		jsoned.merge_patch(Misc.ToJson());
		jsoned.merge_patch(PlayerESP.ToJson());
	}

	void FromJson(json jsoned)
	{
		Misc.FromJson(jsoned);
		PlayerESP.FromJson(jsoned);
	}
};
inline ConfigInstances ConfigInstance;