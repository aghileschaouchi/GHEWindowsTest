#include "gtest/gtest.h"

#include "../GameHackingEngine/include/core/Address.h"
#include "../GameHackingEngine/include/core/Pointer.h"
#include "../GameHackingEngine/include/core/WinGame.h"

#pragma comment(lib, "kernel32.lib") 

TEST(ProcessMonitoring, WindowsGame)
{
	DWORD pid = NULL;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess = NULL;
	HWND gameHWND = NULL;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD, 2> winGame(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	ghe::Address<DWORD_PTR> m16Address(true, 0x005207B4);
	DWORD scoreValue = winGame.readValue(m16Address);
	printf("M16 VALUE: %ud\n", scoreValue);

	DWORD newValue = 5000;
	(winGame.writeValue(m16Address, newValue)) ? printf("WriteValue success!\n") : printf("WriteValue failure!\n");
	printf("M16 VALUE: %ud\n", winGame.readValue(m16Address));

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
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD, 2> winGame(std::move(_baseAddress), std::move(pid),
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
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD, 2> tr2(std::move(_baseAddress), std::move(pid),
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
	BYTE xCoordinateValue = tr2.readValue(xCoordinateAddress); //X address is write only
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
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD, 2> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressCopy()), std::move(offsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess()); 
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.readValue(tigerXPointerAddressResult);
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
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD, 2> tr2(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	std::vector<DWORD_PTR> offsets = { 0x001262F0, 0x0000034D };

	ghe::Pointer<DWORD_PTR, DWORD_PTR> tigerXPointer(std::move(tr2.baseAddressDeref()), std::move(offsets));
	ghe::Address<DWORD_PTR> tigerXPointerAddressResult = tigerXPointer.dynamicAddress(tr2.hProcess()); 
	printf("dynamic address: 0x%08x\n", tigerXPointerAddressResult.value());
	BYTE xCoordinateValue = tr2.readValue(tigerXPointerAddressResult);
	printf("X game value: %hhx\n", xCoordinateValue);
	
	EXPECT_TRUE(true);
}