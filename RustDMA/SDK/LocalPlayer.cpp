#include "Pch.h"
#include "LocalPlayer.h"
#include "Globals.h"

LocalPlayer::LocalPlayer()
{
	printf("[LocalPlayer] Initialized\n");
	uint64_t localplayer = TargetProcess.Read<uint64_t>(TargetProcess.GetModuleAddress(L"GameAssembly.dll") + Class); // Get Class Start Address
	printf("[LocalPlayer] LocalPlayer: 0x%llX\n", localplayer);
	this->StaticField = TargetProcess.Read<uint64_t>(localplayer + StaticField); // Set Static Padding
	printf("[LocalPlayer] Static Fields: 0x%llX\n", StaticField);
	this->BasePlayerBackingField = TargetProcess.Read<uint64_t>(StaticField + BasePlayerBackingField); // Set BasePlayer Backing Field
	printf("[LocalPlayer] Base Player: 0x%llX\n", BasePlayerBackingField);
}
uint64_t LocalPlayer::GetBasePlayer()
{
	return BasePlayerBackingField;
}