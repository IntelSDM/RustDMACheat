#pragma once
#include <string>
#include <Windows.h>
#include <vmmdll.h>

// set to FALSE if you dont want to track the total read size of the DMA
#define COUNT_TOTAL_READSIZE TRUE
class DMAHandler
{

	// Static variables, shared over all instances

	struct LibModules
	{
		HMODULE VMM = nullptr;
		HMODULE FTD3XX = nullptr;
		HMODULE LEECHCORE = nullptr;
	};

	static inline LibModules modules{};

	static inline VMM_HANDLE DMA_HANDLE = nullptr;

	// Counts the size of the reads in total. Reset every frame preferrably for memory tracking
	static inline DWORD64 readSize = 0;

	// Nonstatic variables, different for each class object on purpose, in case the user tries to access
	// multiple processes
	struct BaseProcessInfo
	{
		DWORD pid = 0;
		std::string name;
		const wchar_t* wname;
		ULONG64 base = 0;
	};

	BaseProcessInfo processInfo{};

	BOOLEAN PROCESS_INITIALIZED = FALSE;


	// Private log function used by the DMAHandler class
	static void log(const char* fmt, ...);

	// Will always throw a runtime error if PROCESS_INITIALIZED or DMA_INITIALIZED is false
	void assertNoInit() const;

	// Wow we have friends
	template<typename> friend class DMAScatter;

	static void retrieveScatter(VMMDLL_SCATTER_HANDLE handle, void* buffer, void* target, SIZE_T size);

	static bool DumpMemoryMap();

	std::string GameName;
	std::wstring WGameName;

	std::unordered_map<std::wstring, ULONG64> Modules;
public:
	/**
	 * \brief Constructor takes a wide string of the process.
	 * Expects that all the libraries are in the root dir
	 * \param wname process name
	 * \param memMap whether the memory map should get dumped to file.
	 */
	DMAHandler(const wchar_t* wname, bool memMap = true);
	bool FixDTB();
	// Whether the DMA and Process are initialized
	bool IsInitialized() const;

	// Gets the PID of the process
	DWORD GetPID() const;

	// Gets the Base address of the process
	ULONG64 GetBaseAddress();
	ULONG64 GetModuleAddress(std::wstring modulename);
	void Read(ULONG64 address, ULONG64 buffer, SIZE_T size) const;
	
	template <typename T>
	T Read(void* address)
	{
		T buffer{};
		memset(&buffer, 0, sizeof(T));
		Read(reinterpret_cast<ULONG64>(address), reinterpret_cast<ULONG64>(&buffer), sizeof(T));

		return buffer;
	}
	char* ReadString(ULONG64 address, char* buf,uint64_t length)
	{
		
		memset(&buf, 0, sizeof(buf));
		Read(address, reinterpret_cast<ULONG64>(&buf), length);

		return buf;
	}
	template <typename T>
	T Read(ULONG64 address)
	{
		return Read<T>(reinterpret_cast<void*>(address));
	}
	bool ReadBool(const ULONG64 address, const ULONG64 buffer, const SIZE_T size) const;
	bool Write(ULONG64 address, ULONG64 buffer, SIZE_T size) const;

	template <typename T>
	bool Write(ULONG64 address, T* buffer)
	{
		return Write(address, reinterpret_cast<ULONG64>(buffer), sizeof(T));
	}

	template <typename T>
	bool Write(void* address, T* buffer)
	{
		return Write(reinterpret_cast<ULONG64>(address), reinterpret_cast<ULONG64>(buffer), sizeof(T));
	}

	template <typename T>
	bool Write(ULONG64 address, T value)
	{
		return Write(address, reinterpret_cast<ULONG64>(&value), sizeof(T));
	}

	template <typename T>
	bool Write(void* address, T value)
	{
		return Write(reinterpret_cast<ULONG64>(address), reinterpret_cast<ULONG64>(&value), sizeof(T));
	}

	//Handle Scatter
	void QueueScatterReadEx(VMMDLL_SCATTER_HANDLE handle, uint64_t addr, void* bffr, size_t size) const;
	void ExecuteScatterRead(VMMDLL_SCATTER_HANDLE handle) const;

	void QueueScatterWriteEx(VMMDLL_SCATTER_HANDLE handle, uint64_t addr, void* bffr, size_t size) const;
	void ExecuteScatterWrite(VMMDLL_SCATTER_HANDLE handle) const;

	VMMDLL_SCATTER_HANDLE CreateScatterHandle() const;
	void CloseScatterHandle(VMMDLL_SCATTER_HANDLE& handle) const;


	/**
	 * \brief pattern scans the text section and returns 0 if unsuccessful
	 * \param pattern the pattern
	 * \param mask the mask
	 * \param returnCSOffset in case your pattern leads to a xxx, cs:offset, it will return the address of the global variable instead
	 * \return the address
	 */
	ULONG64 PatternScan(const char* pattern, const std::string& mask, bool returnCSOffset = true);

	/**
	 * \brief closes the DMA and sets DMA_INITIALIZED to FALSE. Do not call on every object, only at the end of your program.
	 */
	static void CloseDMA();

#if COUNT_TOTAL_READSIZE

	static DWORD64 GetTotalReadSize();

	static void ResetReadSize();

#endif
};

template <typename T>
class DMAScatter
{
	T value;
	void* address;
	DMAHandler* DMA;
	VMMDLL_SCATTER_HANDLE handle;
	bool unknown = true;

	void prepare()
	{
		DMA->queueScatterReadEx(handle, reinterpret_cast<uint64_t>(address), &value, sizeof(T));
	}
public:
	DMAScatter(DMAHandler* DMAHandler, VMMDLL_SCATTER_HANDLE handle, void* address)
		: address(address), DMA(DMAHandler), handle(handle)
	{
		if (!handle) DMAHandler::log("Invalid handle!");

		memset(&value, 0, sizeof(T));

		prepare();
	}

	DMAScatter(DMAHandler* DMAHandler, VMMDLL_SCATTER_HANDLE handle, uint64_t address)
		: address(reinterpret_cast<void*>(address)), DMA(DMAHandler), handle(handle)
	{
		if (!handle) DMAHandler::log("Invalid handle!");

		memset(&value, 0, sizeof(T));

		prepare();
	}


	T& operator*()
	{
		return value;
	}
};
