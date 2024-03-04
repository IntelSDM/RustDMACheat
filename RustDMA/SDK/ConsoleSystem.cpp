#include "Pch.h"
#include "consolesystem.h"
#include "Globals.h"


void ConsoleSystem::DisableCommand(uint64_t command)
{
	uintptr_t commandname = TargetProcess.Read<uint64_t>(command + Name);
	
	wchar_t commandstring[36] = { '\0' };
	TargetProcess.Read((ULONG64)commandname + 0x14, reinterpret_cast<void*>( & commandstring), sizeof(commandstring));
	for (std::wstring blacklistedcommand : BlacklistedCommands)
	{
		if (wcscmp(commandstring, blacklistedcommand.c_str()) == 0)
		{
			printf("[ConsoleSystem] ");
			wprintf(commandstring);
			if (!TargetProcess.Write<bool>(command + AllowRunFromServer, false))
			{
				printf(" - Failed To Block From Server\n");
			}
			printf(" - Blocked Command From Server\n");
			return;
		}
	
	}
}
ConsoleSystem::ConsoleSystem()
{
	printf("[ConsoleSystem] Initialized\n");
	uint64_t server = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + Class); // Get Class Start Address
	printf("[ConsoleSystem] ConvarAdmin: 0x%llX\n", server);
	this->StaticField = TargetProcess.Read<uint64_t>(server + StaticField); // Set Static Padding
	printf("[ConsoleSystem] Static Fields: 0x%llX\n", StaticField);
	this->AllBackingField = TargetProcess.Read<uint64_t>(StaticField + AllBackingField);
	printf("[ConsoleSystem] AllBackingField: 0x%llX\n", AllBackingField);
	this->ListSize = TargetProcess.Read<uint32_t>(AllBackingField + ListSize);
	/*
	[DMA @ 22:23:07]: Didnt read all bytes requested! Only read 0/8 bytes!
[DMA @ 22:23:07]: Didnt read all bytes requested! Only read 0/72 bytes!
These are expcted :)
	*/
	for (int i = 0; i < ListSize; i++)
	{
		uint64_t command = TargetProcess.Read<uint64_t>(AllBackingField + (i*0x8));
			if (command == 0x0)
			continue; 
			DisableCommand(command);
	}
}