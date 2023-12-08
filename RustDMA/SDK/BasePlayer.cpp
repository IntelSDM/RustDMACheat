#include "Pch.h"
#include "Globals.h"
#include "BasePlayer.h"

BasePlayer::BasePlayer(uint64_t address)
{
	this->Class = address;
	printf("[BasePlayer] Initialized\n");
	this->BaseMovementOffset = TargetProcess.Read<uint64_t>(Class + BaseMovementOffset);
	printf("[BasePlayer] BaseMovement: 0x%llX\n", BaseMovementOffset);
	this->BaseMovementInstance = new BaseMovement(BaseMovementOffset);

}
PlayerFlags BasePlayer::GetPlayerFlag()
{
	PlayerFlags flag = TargetProcess.Read<PlayerFlags>(Class + PlayerFlag);
	printf("[BasePlayer] PlayerFlag: %d\n", flag);
	return flag;
}
void BasePlayer::WritePlayerFlag(PlayerFlags flag)
{
	if(!TargetProcess.Write<PlayerFlags>(Class + PlayerFlag,flag))
			printf("[BasePlayer] Failed to write PlayerFlag\n");
}
BaseMovement* BasePlayer::GetBaseMovement()
{
	return BaseMovementInstance;
}