#include "Pch.h"
#include "LocalPlayer.h"
#include "Globals.h"

LocalPlayer::LocalPlayer()
{
	printf("[LocalPlayer] Initialized\n");
	Class = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + Class); // Get Class Start Address
	printf("[LocalPlayer] LocalPlayer: 0x%llX\n", Class);
	this->StaticField = TargetProcess.Read<uint64_t>(Class + StaticField); // Set Static Padding
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
	TargetProcess.AddScatterReadRequest(handle, StaticField + BasePlayerBackingField,reinterpret_cast<void*>(&BasePlayerValue),sizeof(uint64_t));
}

bool LocalPlayer::IsLocalPlayerValid()
{
	return BasePlayerValue != 0;
}

LocalPlayer::~LocalPlayer()
{
}