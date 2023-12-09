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
	this->BasePlayerValue = TargetProcess.Read<uint64_t>(StaticField + BasePlayerBackingField); // Set BasePlayer Backing Field
	printf("[LocalPlayer] Base Player: 0x%llX\n", BasePlayerValue);

}

uint64_t LocalPlayer::GetBasePlayer()
{
	return BasePlayerValue;
}

void LocalPlayer::UpdateBasePlayer(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.QueueScatterReadEx(handle, StaticField + BasePlayerBackingField,reinterpret_cast<void*>(&BasePlayerValue),sizeof(uint64_t));
}

bool LocalPlayer::IsLocalPlayerValid()
{
	return BasePlayerValue != 0;
}

LocalPlayer::~LocalPlayer()
{
}