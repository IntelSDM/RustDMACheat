# Install Guide
[Simple Video Guide](https://youtu.be/1Ug1PUPSozg)<br>

## Installing Dependencies

Download The Latest [MemProcFS Release](https://github.com/ufrisk/MemProcFS/releases). Install The win_x64 File. <br>
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/3588044d-05ec-4f6e-a1e7-e1387f3a9214"
    style="width: 85%;" />
</p>
Drag And Drop The Contents Into The Build Directory.<br>


[Download the FTD3xx Dlls](https://ftdichip.com/wp-content/uploads/2024/01/FTD3XXLibrary_v1.3.0.9.zip)
In the .rar file navigate to FTD3XXLibrary->x64->DLL

<p align="Left">
  <img src="https://github.com/IntelSDM/RustDMACheat/assets/86087830/3a26c1d1-689f-48a5-897a-ca873e74b89e"
    style="width: 75%;" />
</p>
Drop them into the build directory.<br>


Before:
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/b08eeae5-c537-47f9-bf6f-f4a82a376d07"
    style="width: 75%;" />
</p>
After:
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/823bf9f1-a346-4672-891c-fdaa3d572eed"
    style="width: 75%;" />
</p>

## Environment Variables

Now Python Must Be Set Up.<br>
[Install Python 3.8](https://www.python.org/ftp/python/3.8.0/python-3.8.0-amd64.exe)<br>
Find your install directory, This should be:
```
AppData\Local\Programs\Python\Python38
```
Open Environment Variables:
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/a44e7eaa-f09d-4cf3-9c38-05a8658bb16c"
    style="width: 50%;" />
</p>
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/1ec41330-8f64-4305-9214-d397f289d343"
    style="width: 50%;" />
</p>
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/21d25bf3-e4bf-4188-979e-7677a05435b7"
    style="width: 50%;" />
</p>
<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/cb8fd603-866e-4d96-8def-3ddcdfc3dd05"
    style="width: 50%;" />
</p>
In The Variable Value Add The Python Path And Python Script Path. Like This(the \dev\ should be your windows username):

```
C:\Users\dev\AppData\Local\Programs\Python\Python38
C:\Users\dev\AppData\Local\Programs\Python\Python38\Scripts
```
Then Add A Semi Colon Inbetween Them As Such:

```
C:\Users\dev\AppData\Local\Programs\Python\Python38;C:\Users\dev\AppData\Local\Programs\Python\Python38\Scripts
```

<p align="Left">
  <img src="https://github.com/IntelSDM/RustDmaCheat/assets/86087830/36dd33b7-632a-4518-b7f5-a7c2ca3fcacb"
    style="width: 80%;" />
</p>
Apply These Changes And You Will Be Ready To Go.
