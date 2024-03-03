#include "Pch.h"
#include "BaseNetworkable.h"
#include "Globals.h"

BaseNetworkable::BaseNetworkable()
{
	uint64_t networkable = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + Class); // Get Class Start Address
	printf("[BaseNetworkable] BaseNetworkable: 0x%llX\n", networkable);
	this->StaticField = TargetProcess.Read<uint64_t>(networkable + StaticField); // Set Static Padding
	printf("[BaseNetworkable] Static Fields: 0x%llX\n", StaticField);
	this->ClientEntities = TargetProcess.Read<uint64_t>(StaticField + ClientEntities);
	printf("[BaseNetworkable] ClientEntities: 0x%llX\n", ClientEntities);
	this->EntityList = TargetProcess.Read<uint64_t>(ClientEntities + EntityList); // entity realm
	printf("[BaseNetworkable] EntityList: 0x%llX\n", EntityList);
	this->BufferList = TargetProcess.Read<uint64_t>(EntityList + BufferList);
	printf("[BaseNetworkable] BufferList: 0x%llX\n", BufferList);
	this->ObjectList = TargetProcess.Read<uint64_t>(BufferList + ObjectList);
	printf("[BaseNetworkable] ObjectList: 0x%llX\n", ObjectList);
	this->ObjectListSize = TargetProcess.Read<uint32_t>(BufferList + ObjectListSize);
	printf("[BaseNetworkable] ObjectListSize: 0x%lX\n", ObjectListSize);
}
void BaseNetworkable::ItterateEntities()
{
	for (int i = 0; i < ObjectListSize; i++)
	{
		uint64_t currentobject = TargetProcess.Read<uint64_t>(ObjectList + (0x20 + (i * 8)));
		uint64_t baseobject = TargetProcess.Read<uint64_t>(currentobject + 0x10);
		uint64_t object = TargetProcess.Read<uint64_t>(baseobject + 0x30);

		uint64_t prefab = TargetProcess.Read<uint64_t>(object + 0x60); 
		WORD tag = TargetProcess.Read<WORD>(object + 0x54);
		char buff[256] = { 0 };
		TargetProcess.Read(prefab,reinterpret_cast<void*>(buff),sizeof(buff));// access name from heap. 
		std::string prefabname = buff;
		std::cout << "[BaseNetworkable] PrefabName: " << prefabname << std::endl;
		std::cout << "[BaseNetworkable] Tag: " << tag << std::endl;
	}

}