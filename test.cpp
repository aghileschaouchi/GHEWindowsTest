#include "gtest/gtest.h"

#include "../GameHackingEngine/include/core/Address.h"
#include "../GameHackingEngine/include/core/Pointer.h"
#include "../GameHackingEngine/include/core/WinGame.h"

#pragma comment(lib, "kernel32.lib") 

TEST(ProcessMonitoring, WindowsGame)
{
	DWORD pid;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess;
	HWND gameHWND;
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
	const DWORD_PTR m16AddressValue = static_cast<DWORD_PTR>(m16Address.getAddress());
	const bool isStatic = m16Address.isStatic();

	(isStatic) ? printf("%ud\n", m16AddressValue) : printf("test has failed!\n");

	EXPECT_TRUE(isStatic);
}

TEST(ProcessMonitoring, Pointer)
{
	DWORD pid;
	std::string gameName("Tomb Raider II");
	std::string baseModuleName("Tomb2.exe");
	HANDLE hProcess;
	HWND gameHWND;
	ghe::Address<DWORD_PTR> _baseAddress;
	ghe::WinGame<DWORD, HANDLE, HWND, DWORD_PTR, DWORD, 2> winGame(std::move(_baseAddress), std::move(pid),
		std::move(gameName), std::move(hProcess), std::move(gameHWND), std::move(baseModuleName));

	ghe::Address<DWORD_PTR> m16Address(true, winGame.baseAddress()->getAddress() + static_cast<DWORD_PTR>(0x001207BC));
	std::vector<DWORD_PTR> offsets = { 0x00000035 };
	ghe::Pointer<DWORD_PTR, DWORD_PTR> pointer1(std::move(m16Address), std::move(offsets));

	DWORD_PTR m16StaticAddress1 = pointer1.pointedAddressValue();
	printf("is pointing to:%ud\n", m16StaticAddress1);

	ghe::Address<DWORD_PTR> testAddress(m16StaticAddress1, true);
	DWORD_PTR m16StaticAddress1Value = winGame.readValue(testAddress);
	printf("value:%ud\n", m16StaticAddress1Value);
	
	EXPECT_TRUE(0x0ED9BE09 == m16StaticAddress1Value);
}