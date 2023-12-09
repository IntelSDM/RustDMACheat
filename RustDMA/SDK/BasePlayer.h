#pragma once
#include "BaseMovement.h"
#include "PlayerInventory.h"
#include "DMAHandler.h"
#include "Item.h"
enum class PlayerFlags : uint32_t
{
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Incapacitated = 524288,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
	VoiceRangeBoost = 8388608,
	ModifyClan = 16777216,
	LoadingAfterTransfer = 33554432,
	NoRespawnZone = 67108864
};

class BasePlayer
{
	uint64_t Class = 0;
	uint64_t PlayerFlag = 0x8C8; // 	public global::BasePlayer.PlayerFlags playerFlags;
	uint64_t BaseMovementOffset = 0x6A0; // public BaseMovement movement;
	uint64_t PlayerInventoryOffset = 0x8D8; // public global::PlayerInventory inventory;
	uint64_t ActiveItemIDOffset = 0x7D0; // private ItemId clActiveItem;
	BaseMovement* BaseMovementInstance;
	uint64_t Inventory = 0x8D8; // public PlayerInventory inventory;
	PlayerInventory* PlayerInventoryInstance = nullptr;
	uint64_t ActiveItemID = 0;

public:
	BasePlayer(uint64_t address);
	PlayerFlags GetPlayerFlag();
	void WritePlayerFlag(PlayerFlags flag);
	BaseMovement* GetBaseMovement();
	PlayerInventory* GetPlayerInventory();
	void UpdateActiveItemID(VMMDLL_SCATTER_HANDLE handle);
	uint64_t GetActiveItemID();
	Item* GetActiveItem();
};