#include "gtest/gtest.h"

#include "../GameHackingEngine/include/core/Address.h"
#include "../GameHackingEngine/include/core/Pointer.h"
#include "../GameHackingEngine/include/core/WinGame.h"

#include <chrono>
#include <thread>

#pragma comment(lib, "kernel32.lib") 

TEST(ProcessMonitoring, WindowsGame)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	ghe::Address<DWORD_PTR> m16Address(true, 0x005207B4);
	DWORD scoreValue = tr2.memoryManager().readValue<DWORD>(m16Address);
	printf("M16 VALUE: %ud\n", scoreValue);

	DWORD newValue = 5000;
	(tr2.memoryManager().writeValue<DWORD>(m16Address, newValue)) ? printf("WriteValue success!\n") : printf("WriteValue failure!\n");
	printf("M16 VALUE: %ud\n", tr2.memoryManager().readValue<DWORD>(m16Address));

	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, Address)
{
	ghe::Address<DWORD_PTR> m16Address(true, 0x005207B4);
	const DWORD_PTR m16AddressValue = static_cast<DWORD_PTR>(m16Address.value());
	const bool isStatic = m16Address.isStatic();

	(isStatic) ? printf("%ud\n", m16AddressValue) : printf("test has failed!\n");

	EXPECT_TRUE(isStatic);
}

TEST(ProcessMonitoring, Pointer1)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> winGame(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	ghe::Address<DWORD_PTR> m16Address(true, winGame.baseAddress()->value() + static_cast<DWORD_PTR>(0x001207BC));
	std::vector<DWORD_PTR> offsets = { 0x00000035 };

	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, Pointer2)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x0011B91C, 0x00000360 };

	ghe::Address<DWORD_PTR> tr2baseAddressCopy = tr2.baseAddressCopy();

	DWORD_PTR tmp_addr = NULL;
	DWORD_PTR dynamicAddress = tr2.baseAddress()->value();
	hProcess = tr2.hProcess();

	for (size_t i = 0; i < offsets.size() - 1; ++i)
	{
		dynamicAddress += offsets[i];
		ReadProcessMemory(hProcess, (LPCVOID)(dynamicAddress), (LPVOID)&tmp_addr, sizeof(tmp_addr), NULL);
		dynamicAddress = tmp_addr;
	}
	dynamicAddress += offsets[offsets.size() - 1];

	ghe::Address<DWORD_PTR> xCoordinateAddress(false, dynamicAddress);
	
	printf("dynamic address: 0x%08x\n", xCoordinateAddress.value());
	BYTE xCoordinateValue = tr2.memoryManager().readValue<BYTE>(xCoordinateAddress); //X address is write only
	printf("X game value: %hhx\n", xCoordinateValue);

	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, Pointer3)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressCopy()), std::move(offsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess()); 
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
	printf("X game value: %hhx\n", xCoordinateValue);
	
	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, Pointer4)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressDeref()), std::move(offsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess()); 
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
	printf("X game value: %hhx\n", xCoordinateValue);
	
	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, FlyHack)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001207BC, 0x00000039 };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> laraYPointer(std::move(tr2.baseAddressDeref()), std::move(offsets));
	ghe::Address<DWORD_PTR> laraYPointerAddressResult = laraYPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", laraYPointerAddressResult.value());
	BYTE yCoordinateValue = tr2.memoryManager().readValue<BYTE>(laraYPointerAddressResult);
	printf("X game value: %hhx\n", yCoordinateValue);

	BYTE newY = 7;
	tr2.memoryManager().writeValue<BYTE>(laraYPointerAddressResult, newY);
	printf("X game value: %hhx\n", yCoordinateValue);

	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, ReadFromLockedMemory1)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001207BC, 0x00000039 };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> laraYPointer(std::move(tr2.baseAddressDeref()), std::move(offsets));
	ghe::Address<DWORD_PTR> laraYPointerAddressResult = laraYPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", laraYPointerAddressResult.value());
	
	
	DWORD _protectionBackup;
	BYTE yCoordinateValue;
	DWORD_PTR dynamicYAddress = laraYPointerAddressResult.value();

	if (VirtualProtectEx(tr2.hProcess(), (LPVOID)dynamicYAddress, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &_protectionBackup) == 0)
	{
		unsigned int codeError = GetLastError();
		printf("unlock() exited with %ud as code error, check VirtualProtectEx call 1\n", codeError);
	}
	
	ReadProcessMemory(tr2.hProcess(), (LPCVOID)dynamicYAddress, (LPVOID)&yCoordinateValue, sizeof(yCoordinateValue), NULL);
	printf("Y game value: %hhx\n", yCoordinateValue);

	if (VirtualProtectEx(tr2.hProcess(), (LPVOID)dynamicYAddress, sizeof(yCoordinateValue), _protectionBackup, NULL) == 0)
	{
		unsigned int codeError = GetLastError();
		printf("restore() exited with %ud as code error, check VirtualProtectEx call\n", codeError);
	}

	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, WriteToLockedMemory1)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001207BC, 0x00000039 };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> laraYPointer(std::move(tr2.baseAddressDeref()), std::move(offsets));
	ghe::Address<DWORD_PTR> laraYPointerAddressResult = laraYPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", laraYPointerAddressResult.value());

	DWORD _protectionBackup;
	BYTE yCoordinateValue;

	if (VirtualProtectEx(tr2.hProcess(), (LPVOID)laraYPointerAddressResult.value(), sizeof(yCoordinateValue), PAGE_EXECUTE_READWRITE, &_protectionBackup) == 0)
	{
		unsigned int codeError = GetLastError();
		printf("unlock() exited with %ud as code error, check VirtualProtectEx call 1\n", codeError);
	}

	ReadProcessMemory(tr2.hProcess(), (LPCVOID)laraYPointerAddressResult.value(), (LPVOID)&yCoordinateValue, sizeof(yCoordinateValue), NULL);
	printf("Y game value: %hhx\n", yCoordinateValue);

	BYTE newY = 0x00;
	WriteProcessMemory(tr2.hProcess(), (LPVOID)laraYPointerAddressResult.value(), (LPCVOID)&newY, sizeof(newY), NULL);

	ReadProcessMemory(tr2.hProcess(), (LPCVOID)laraYPointerAddressResult.value(), (LPVOID)&newY, sizeof(newY), NULL);
	printf("Y game value: %hhx\n", newY);

	if (VirtualProtectEx(tr2.hProcess(), (LPVOID)laraYPointerAddressResult.value(), sizeof(newY), _protectionBackup, NULL) == 0)
	{
		unsigned int codeError = GetLastError();
		printf("restore() exited with %ud as code error, check VirtualProtectEx call\n", codeError);
	}

	EXPECT_TRUE(true);
}

TEST(ProcessMonitoring, WinMemoryManagerReadWriteXTiger)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressCopy()), std::move(offsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess()); 
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
	printf("X tiger value: %hhx\n", xCoordinateValue);

	BYTE newXValue = 0x75;
	ASSERT_TRUE(tr2.memoryManager().writeValue<BYTE>(tigerXPointerAddressResult, newXValue));
	BYTE checkNewValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
	printf("X tiger new value: %hhx\n", checkNewValue);
}

TEST(ProcessMonitoring, WinMemoryManagerReadWriteZTiger)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001262F0, 0x00000354 };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerZPointer(std::move(tr2.baseAddressCopy()), std::move(offsets));
	ghe::Address<DWORD_PTR> tigerZPointerAddressResult = tigerZPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", tigerZPointerAddressResult.value());
	BYTE zCoordinateValue = tr2.memoryManager().readValue<BYTE>(tigerZPointerAddressResult);
	printf("Z tiger value: %hhx\n", zCoordinateValue);

	BYTE newZValue = 0x5A;
	ASSERT_TRUE(tr2.memoryManager().writeValue<BYTE>(tigerZPointerAddressResult, newZValue));
	BYTE checkNewValue = tr2.memoryManager().readValue<BYTE>(tigerZPointerAddressResult);
	printf("Z tiger new value: %hhx\n", checkNewValue);
}

TEST(ProcessMonitoring, WinMemoryManagerReadWriteXZTigerMemoryUnlock)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> zOffsets = { 0x001262F0, 0x00000354 };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerZPointer(std::move(tr2.baseAddressCopy()), std::move(zOffsets));
	ghe::Address<DWORD_PTR> tigerZPointerAddressResult = tigerZPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", tigerZPointerAddressResult.value());
	BYTE zCoordinateValue = tr2.memoryManager().readValue<BYTE, false>(tigerZPointerAddressResult);
	printf("Z tiger value: %hhx\n", zCoordinateValue);

	std::vector<DWORD_PTR> xOffsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressCopy()), std::move(xOffsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
	printf("X tiger value: %hhx\n", xCoordinateValue);



	BYTE newZValue = 0x5A;
	BYTE newXValue = 0x75;
	bool newZValueIsWritten = tr2.memoryManager().writeValue<BYTE, false>(tigerZPointerAddressResult, newZValue);
	ASSERT_TRUE(newZValueIsWritten);
	BYTE checkZNewValue = tr2.memoryManager().readValue<BYTE, false>(tigerZPointerAddressResult);
	printf("Z tiger new value: %hhx\n", checkZNewValue);

	bool newXValueIsWritten = tr2.memoryManager().writeValue<BYTE, false>(tigerXPointerAddressResult, newXValue);
	ASSERT_TRUE(newXValueIsWritten);
	BYTE checkXNewValue = tr2.memoryManager().readValue<BYTE, false>(tigerXPointerAddressResult);
	printf("X tiger new value: %hhx\n", checkXNewValue);
}

TEST(ProcessMonitoring, TigerLooper)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> zOffsets = { 0x001262F0, 0x00000354 };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerZPointer(std::move(tr2.baseAddressCopy()), std::move(zOffsets));
	ghe::Address<DWORD_PTR> tigerZPointerAddressResult = tigerZPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", tigerZPointerAddressResult.value());
	BYTE zCoordinateValue = tr2.memoryManager().readValue<BYTE, true>(tigerZPointerAddressResult);
	printf("Z tiger value: %hhx\n", zCoordinateValue);

	std::vector<DWORD_PTR> xOffsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressCopy()), std::move(xOffsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess());
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
	printf("X tiger value: %hhx\n", xCoordinateValue);



	BYTE newZValue = 0x5A;
	BYTE newXValue = 0x75;
	while (true)
	{
		ASSERT_TRUE(tr2.memoryManager().writeValue<BYTE>(tigerZPointerAddressResult, newZValue));
		BYTE checkZNewValue = tr2.memoryManager().readValue<BYTE>(tigerZPointerAddressResult);
		printf("Z tiger new value: %hhx\n", checkZNewValue);

		ASSERT_TRUE(tr2.memoryManager().writeValue<BYTE>(tigerXPointerAddressResult, newXValue));
		BYTE checkXNewValue = tr2.memoryManager().readValue<BYTE>(tigerXPointerAddressResult);
		printf("X tiger new value: %hhx\n", checkXNewValue);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}