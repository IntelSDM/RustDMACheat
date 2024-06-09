#pragma once
class BaseNetworkable
{
	// So we use the static basegamemode__C in order to then access basenetworkable through the inheritance chain as basenetworkable itself is in a guarded region
	/*
	  "Address": 54385064,
      "Name": "System.Collections.Generic.List\u003CBaseGameMode\u003E_TypeInfo",
      "Signature": "System_Collections_Generic_List_BaseGameMode__c*"
	*/
	uint64_t Class = 0x3C58568;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t ClientEntities = 0x0; // public static global::BaseNetworkable.EntityRealm clientEntities;
	uint64_t EntityList = 0x10; // private ListDictionary<NetworkableId, global::BaseNetworkable> entityList;

	//These next two are a little tricky and you will require dump.cs
	//il2cpp dumper cant retrieve these offsets since they depend on what they are instantiated with
	//this explanation is for BufferList, which is in: public class ListDictionary<TKey, TVal>
	//the fields start at 0x10, this is because System.Object takes up 8 bytes and we need to allign to native word size
	//we are trying to calculate the offset of private BufferList<TVal> vals;
	//each var is a class reference, we can consider them 0x8 apart
	//so private BufferList<TVal> vals; is (0x10 + 0x8 + 0x8 + 0x8) with 0x10 being System.Object and three class refernces being added
	// this was to calculate BufferList, but the same Logic can be applied for the other, i have put the classes and their vars in comments

	uint64_t BufferList = 0x28; // public class ListDictionary<TKey, TVal> -> private BufferList<TVal> vals;
	uint64_t ObjectList = 0x18; // public class BufferList<T> -> private T[] buffer             (the int is a pointer, so 8 bytes rather than 4)
	uint32_t ObjectListSize = 0x10;

public:
	BaseNetworkable();
	void ItterateEntities();

};
