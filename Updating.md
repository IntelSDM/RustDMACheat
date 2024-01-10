# Updating Guide
[Simple Video Guide](https://youtu.be/kCqW_dBe5qo)

## Tools
[IL2CPP Dumper](https://github.com/Perfare/Il2CppDumper/releases/download/v6.7.40/Il2CppDumper-net7-win-v6.7.40.zip)<br>
[dnSpy](https://github.com/dnSpy/dnSpy/releases/download/v6.1.8/dnSpy-net-win64.zip)

## IL2CPP Dumping
Select `GameAssembly.dll`: 
```
Rust/GameAssembly.dll
```
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/570c31b6-988a-4d8e-8f45-f258297d95d7"
    style="width: 75%;" />
</p>

Select `global-metadata.dat`

```
Rust\RustClient_Data\il2cpp_data\Metadata\global-metadata.dat
```

<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/ae59da4a-4b93-4dd6-86d0-cf57bb3663a9"
    style="width: 75%;" />
</p>



## Updating Static Classes
*This Needs To Be Updated Each Time The Game Updates* <br>
Open `script.json` Generated from Il2CPP Dumper. <br>
Look into the RustDMA/SDK Folder For A Header That Has An Address, Name, Signature.<br>
I Will Be Using TODSky.h As An Example. <br>

```
	/*
	"Address": 54329896,
      "Name": "TOD_Sky_TypeInfo",
      "Signature": "TOD_Sky_c*"
	*/
```

Now Search For The Signature In `Script.json`(open in notepad and control + f). This Signature Will Be `TOD_Sky_c*` <br>

<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/6c3ab642-7117-488a-b2d9-12534454bf4f"
    style="width: 50%;" />
</p>

Copy The Address From `Script.json`. For Me It Will Be `"Address": 54842648,`<br>
Open Calculator And Set It To Programmer Mode <br>
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/b6d17bfe-b0e9-4828-88fd-b5d8f3bc5483"
    style="width: 50%;" />
</p>
Make Sure You Are Selected In Decimal
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/42684cfc-0312-462f-a317-0e522e900729"
    style="width: 50%;" />
</p>
Input The Address And Then Select Hex
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/02ed1634-b461-4f13-80ac-99d6c66adab1"
    style="width: 50%;" />
</p>

The New Address Is `0x344D518`

## Updating Offsets
*These Only Need To Be Updated Every When The Game Adds Something To The Class* <br>
Drag The `Assembly-CSharp.dll` From `il2cpp dumper\DummyDll`
I Will Use `TODSky.h` As An Example. <br>
The Game Class Is `TOD_Sky` <br>
To Update The NightParamaters You Search For `public TOD_NightParameters Night`<br>
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/4ad91c33-a117-4922-b6cc-2648ee48ffe5"
    style="width: 50%;" />
</p>
Revealing The Offset Is 0x58 


