#include "gtest/gtest.h"

#include "../GameHackingEngine/include/core/WinGame.h"

TEST(TestCaseName, TestName) {
	DWORD pid;
	LPCWSTR gameName;
	HANDLE hProcess;
	HWND gameHWND;
	ghe::WinGame<DWORD, LPCWSTR, HANDLE, HWND, 2> winGame; //CONTRUCTORS SHOULD BE FIXED


  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}