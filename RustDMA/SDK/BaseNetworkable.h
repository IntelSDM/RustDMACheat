#pragma once
class BaseNetworkable
{
	// So we use the static basegamemode__C in order to then access basenetworkable through the inheritance chain as basenetworkable itself is in a guarded region
	/*
	  "Address": 54879968,
      "Name": "System.Collections.Generic.List\u003CBaseGameMode\u003E_TypeInfo",
      "Signature": "System_Collections_Generic_List_BaseGameMode__c*"
	*/
	uint64_t Class = 0x34566E0;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t ClientEntities = 0x10; // public static global::BaseNetworkable.EntityRealm clientEntities;
	uint64_t EntityList = 0x10; // public global::System.Collections.Generic.List<global::BaseNetworkable> entityList; In EntityRealm Class
	// Not sure about these ones, I copied from a chain
	uint64_t BufferList = 0x28;
	uint64_t ObjectList = 0x18; 
	uint32_t ObjectListSize = 0x10;

public:
	BaseNetworkable();
	void ItterateEntities();

};


