#pragma once

enum class DebugFilter
{
	Off = 0,
	Dynamic = 1,
	Static = 2,
	Grid = 4,
	All = 7
};
class OcclusionCulling
{
	/*
	 "Address": 54747536,
      "Name": "OcclusionCulling_TypeInfo",
      "Signature": "OcclusionCulling_c*"
	*/
	uint64_t Class = 0x3436190; 
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t Instance = 0x70; // 	public static OcclusionCulling Instance
	uint64_t DebugSettings = 0x1A0; // 	public OcclusionCulling.DebugSettings debugSettings
	uint64_t DebugShow = 0x8C; // 	public OcclusionCulling.DebugFilter debugFilter
	uint64_t LayerFilter = 0x20; // 	DebugSettings.LayerMask layerFilter
public:
	OcclusionCulling();
	void WriteLayerMask(int mask);
	void WriteDebugSettings(DebugFilter debugfilter);
};