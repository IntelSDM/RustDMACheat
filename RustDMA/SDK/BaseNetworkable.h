#pragma once
class BaseNetworkable
{
	/*
	 "Address": 54625544,
      "Name": "BaseNetworkable_TypeInfo",
      "Signature": "BaseNetworkable_c*"
	*/
	uint64_t Class = 0x3418508;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t ClientEntities = 0x0; // public static global::BaseNetworkable.EntityRealm clientEntities;
	uint64_t EntityList = 0x10; // public global::System.Collections.Generic.List<global::BaseNetworkable> entityList; In EntityRealm Class
	// Not sure about these ones, I copied from a chain
	uint64_t BufferList = 0x28;
	uint64_t ObjectList = 0x18; 
	uint32_t ObjectListSize = 0x10;

public:
	BaseNetworkable();
	void ItterateEntities();

};


