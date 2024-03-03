#pragma once
#include "Pch.h"
#include "Misc.h"

class ConfigInstances
{
public:
	MiscConfig Misc = MiscConfig(LIT("Player"));

	json ToJson()
	{
		json jsoned;
		jsoned.merge_patch(Misc.ToJson());
	}

	void FromJson(json jsoned)
	{
		Misc.FromJson(jsoned);
	}
};
inline ConfigInstances ConfigInstance;