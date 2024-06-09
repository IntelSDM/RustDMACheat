#pragma once
#include "BaseMovement.h"
#include "Item.h"
#include "Pch.h"
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
	NoRespawnZone = 67108864,
	IsInTutorial = 134217728
};
static enum StanceFlags : int
{
	Ducked = 1,
	Jumped = 2,
	OnGround = 4,
	Sleeper = 8,
	Sprinting = 16,
	OnLadder = 32,
	Flying = 64,
	Aim = 128,
	Prone = 256,
	Mounted = 512,
	Relax = 1024,
	OnPhone = 2048,
	Crawling = 4096,
	Loading = 8192,
	HeadLook = 16384,
	HasParachute = 32768
};
class BasePlayer
{
	/*
	  "Address": 54626928,
  "Name": "BasePlayer_TypeInfo",
  "Signature": "BasePlayer_c*"
*/
	uint64_t StaticClass = 0x3C29E10;
	uint64_t Class = 0;
	uint64_t PlayerFlag = 0x838; // 	public global::BasePlayer.PlayerFlags playerFlags;
	uint64_t BaseMovementOffset = 0x608; // public BaseMovement movement;
	uint64_t PlayerInventory = 0x848; // public global::PlayerInventory inventory;
	uint64_t ActiveItemIDOffset = 0x748; // private ItemId clActiveItem;
	uint64_t PlayerModel = 0x5E0; //public PlayerModel playerModel;
	uint64_t Position = 0x1C8;// PlayerModel -> internal Vector3 position;
	Vector3 TransformPosition = Vector3::Zero();
	uint64_t DisplayName = 0x898; // protected string _displayName;
	uint64_t VisiblePlayerList = 0x20; // 	private static ListDictionary<ulong, global::BasePlayer> visiblePlayerList;
	uint64_t DestroyedOffset = 0x40; // basenetworkable -> private bool <IsDestroyed>k__BackingField;
	uint32_t IsNPCOffset = 0x2D8; // 	private bool <IsNpc>k__BackingField;
	std::shared_ptr<BaseMovement> BaseMovementInstance;


	uint32_t ActiveItemID = 0;

	// these are offsets outside of baseplayer. I just don't want the hassle of 1000 classes. The Class is before the "->"
	uint64_t ContainerBelt = 0x30; // PlayerInventory -> public global::ItemContainer containerBelt;
	uint64_t ItemList = 0x48; // ItemContainer -> 	public List<global::Item> itemList;
	uint64_t ItemListContents = 0x10; // ItemList + 0x10 is the actual contents of the c# list
	uint64_t ItemListSize = 0x18; // ItemList + 0x18 is the size of a c# list
	std::vector<std::shared_ptr<BasePlayer>> PlayerList;
	std::vector<std::shared_ptr<Item>> BeltContainerList;
	int PlayerListSize = 0;
	wchar_t PlayerName[36] = { '\0' };

	bool Destroyed = false;
	bool NPC = false;
	int ActiveFlag = 0;

public:
	BasePlayer(uint64_t address, VMMDLL_SCATTER_HANDLE handle);
	~BasePlayer();
	uint64_t GetClass();
	PlayerFlags GetPlayerFlag();
	void WritePlayerFlag(PlayerFlags flag);
	std::shared_ptr<BaseMovement> GetBaseMovement();

	void UpdateActiveItemID(VMMDLL_SCATTER_HANDLE handle);
	uint32_t GetActiveItemID();
	std::shared_ptr<Item> GetActiveItem();
	bool IsPlayerValid();
	void SetupBeltContainerList();
	void InitializePlayerList();
	void CachePlayers();
	int GetPlayerListSize();
	void CacheStage1(VMMDLL_SCATTER_HANDLE handle);
	bool IsSleeping();
	bool IsNPC();
	std::wstring GetName();
	std::vector<std::shared_ptr<BasePlayer>> GetPlayerList();
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	void UpdateDestroyed(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetPosition();
	void UpdateActiveFlag(VMMDLL_SCATTER_HANDLE handle);
	int GetActiveFlag();
	void WriteActiveFlag(int pose);
};
