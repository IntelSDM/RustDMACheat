#pragma once
class MainCamera
{
	/*
	Script.json
	"Address": 54578032,
	  "Name": "MainCamera_TypeInfo",
	  "Signature": "MainCamera_c*"
	*/
	uint64_t Class = 0x3C15500;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t Camera = 0x0;// public static Camera mainCamera

	uint64_t CameraGameObject = 0x10; //You can't find these in il2cpp dumps, instead use IDA or CE
	uint64_t ViewMatrixOffset = 0x30C; //You can't find these in il2cpp dumps, instead use IDA or CE
public:
	MainCamera();
	ViewMatrix GetViewMatrix();
};
