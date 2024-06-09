#pragma once

class ConsoleSystem
{
	/*
      "Address": 54573472,
      "Name": "ConsoleSystem.Index_TypeInfo",
      "Signature": "ConsoleSystem_Index_c*"
	*/
    uint64_t Class = 0x3C28130;
    //Dump.cs / DummyDLL
    uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
    uint64_t AllBackingField = 0x10; //private static ConsoleSystem.Command[] <All>k__BackingField;
    uint32_t ListSize = 0x18;

    // public class Command offsets -> ConsoleSystem.Command
    uint64_t Name = 0x10; // public string name;
    uint64_t AllowRunFromServer = 0x60; // public bool allowRunFromServer;

    std::vector<std::wstring> BlacklistedCommands = {L"noclip",L"camspeed",L"camzoomlerp",L"camzoomspeed",L"debugcamera",L"debugcamera_fov" };
public:
    ConsoleSystem();
    void DisableCommand(uint64_t command);
};
