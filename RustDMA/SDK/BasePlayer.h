#pragma once
#include "BaseMovement.h"
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
	uint64_t PlayerFlag = 0x8D8; // 	public global::BasePlayer.PlayerFlags playerFlags;
	uint64_t BaseMovementOffset = 0x6A8; // public BaseMovement movement;
	uint64_t PlayerInventory = 0x8E8; // public global::PlayerInventory inventory;
	uint64_t ActiveItemIDOffset = 0x7E0; // private ItemId clActiveItem;
	std::shared_ptr<BaseMovement> BaseMovementInstance;


	uint32_t ActiveItemID = 0;

	// these are offsets outside of baseplayer. I just don't want the hassle of 1000 classes. The Class is before the "->"
	uint64_t ContainerBelt = 0x28; // PlayerInventory -> public global::ItemContainer containerBelt;
	uint64_t ItemList = 0x40; // ItemContainer -> 	public List<global::Item> itemList;
	uint64_t ItemListContents = 0x10; // ItemList + 0x10 is the actual contents of the c# list
	uint64_t ItemListSize = 0x18; // ItemList + 0x18 is the size of a c# list

	std::vector<std::shared_ptr<Item>> BeltContainerList;
	

public:
	BasePlayer(uint64_t address);
	~BasePlayer();
	PlayerFlags GetPlayerFlag();
	void WritePlayerFlag(PlayerFlags flag);
	std::shared_ptr<BaseMovement> GetBaseMovement();

	void UpdateActiveItemID(VMMDLL_SCATTER_HANDLE handle);
	uint32_t GetActiveItemID();
	std::shared_ptr<Item> GetActiveItem();
	bool IsPlayerValid();
	void SetupBeltContainerList();
};