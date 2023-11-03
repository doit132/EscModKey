#include "interception.h"
#include "utils.h"

#include <iostream>
#include <stdio.h>
#include <windows.h>

#define KEY_DOWN(vKey) ((GetAsyncKeyState(vKey) & 0x8000) ? 1 : 0)

enum ScanCode { SCANCODE_LWIN = 0x5B, SCANCODE_LALT = 0x38, SCANCODE_LSHIFT = 0x2A, SCANCODE_LCTRL = 0x1D };

enum VirtCode { VIRTCODE_LWIN = 91, VIRTCODE_LALT = 18, VIRTCODE_LSHIFT = 16, VIRTCODE_LCTRL = 17 };

enum PhyKeyState { PHY_KEY_DOWN = false, PHY_KEY_UP = true };

// 全局变量，用于线程同步
HANDLE g_ThreadHandle = NULL;
HANDLE g_ThreadHandle_LAlt = NULL;
HANDLE g_ThreadHandle_LShift = NULL;
HANDLE g_ThreadHandle_LCtrl = NULL;
HANDLE g_ThreadHandle_LWin = NULL;

BOOL g_ExitThread = FALSE;

bool phy_LAlt_pressed = PHY_KEY_UP;
bool phy_LCtrl_pressed = PHY_KEY_UP;
bool phy_LShift_pressed = PHY_KEY_UP;
bool phy_LWin_pressed = PHY_KEY_UP;

// 全局变量，定时器ID
UINT_PTR g_TimerId_LAlt = 0;
UINT_PTR g_TimerId_LShift = 0;
UINT_PTR g_TimerId_LCtrl = 0;
UINT_PTR g_TimerId_LWin = 0;

// ### 定时器回调函数 begin
VOID CALLBACK TimerCallback_LCtrl(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_LCtrl);
	g_TimerId_LCtrl = 0;
	phy_LCtrl_pressed = PHY_KEY_DOWN;
}

VOID CALLBACK TimerCallback_LShift(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_LShift);
	g_TimerId_LShift = 0;
	phy_LShift_pressed = PHY_KEY_DOWN;
}

VOID CALLBACK TimerCallback_LAlt(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_LAlt);
	g_TimerId_LAlt = 0;
	phy_LAlt_pressed = PHY_KEY_DOWN;
}

VOID CALLBACK TimerCallback_LWin(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_LWin);
	g_TimerId_LWin = 0;
	phy_LAlt_pressed = PHY_KEY_DOWN;
}
// ### 定时器回调函数 end

// ### 多线程函数 begin
DWORD WINAPI releaseLCtrlKeyState(LPVOID lpParam)
{
	(void)lpParam;

	short count_lctrl = 0;
	while (1) {
		if (phy_LCtrl_pressed == PHY_KEY_DOWN) {
			// printf("phy_ctrl is down\n");
			Sleep(250);
			continue;
		}
		// 这里使用 count_lctrl 做次数控制, 是因为如果使用纯软件做按键模拟, 那么物理按键一定是 PHY_KEY_UP 状态
		// 程序就要执行到这里 如果立即释放, 那么软件模拟的按键可能会失效, 比如软件模拟 ctrl+w
		// 因为立即释放了 ctrl 所以实际输出就只是 w
		while (KEY_DOWN(VK_LCONTROL)) { // lctrl
			count_lctrl++;
			Sleep(10);
			if (count_lctrl == 10) {
				break;
			}
		}
		if (count_lctrl == 10) {
			std::cout << "lctrl will releaseL" << std::endl;
			count_lctrl = 0;
			keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
		}

		Sleep(250);
	}
}

DWORD WINAPI releaseLShiftKeyState(LPVOID lpParam)
{
	(void)lpParam;

	short count_lshift = 0;
	while (1) {
		if (phy_LShift_pressed == PHY_KEY_DOWN) {
			// printf("phy_lshift is down\n");
			Sleep(250);
			continue;
		}

		// 这里使用 count_lshift 做次数控制, 是因为如果使用纯软件做按键模拟, 那么物理按键一定是 PHY_KEY_UP 状态
		// 程序就要执行到这里 如果立即释放, 那么软件模拟的按键可能会失效, 比如软件模拟 shift+f4
		// 因为立即释放了 shift 所以实际输出就只是 f4
		while (KEY_DOWN(VK_LSHIFT)) { // lshift
			count_lshift++;
			Sleep(10);
			if (count_lshift == 10) {
				break;
			}
		}
		if (count_lshift == 10) {
			std::cout << "lshift will releaseL" << std::endl;
			count_lshift = 0;
			keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
		}

		Sleep(250);
	}
}

DWORD WINAPI releaseLAltKeyState(LPVOID lpParam)
{
	(void)lpParam;

	short count_lalt = 0;
	while (1) {
		if (phy_LAlt_pressed == PHY_KEY_DOWN) {
			// printf("phy_alt is down\n");
			Sleep(250);
			continue;
		}

		// 这里使用 count_lalt 做次数控制, 是因为如果使用纯软件做按键模拟, 那么物理按键一定是 PHY_KEY_UP 状态
		// 程序就要执行到这里 如果立即释放, 那么软件模拟的按键可能会失效, 比如软件模拟 alt+f4
		// 因为立即释放了 alt 所以实际输出就只是 f4
		while (KEY_DOWN(VK_LMENU)) { // lalt
			count_lalt++;
			Sleep(10);
			if (count_lalt == 10) {
				break;
			}
		}
		if (count_lalt == 10) {
			std::cout << "alt will releaseL" << std::endl;
			count_lalt = 0;
			keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
		}

		Sleep(250);
	}
}

DWORD WINAPI releaseLWinKeyState(LPVOID lpParam)
{
	(void)lpParam;

	short count_lwin = 0;
	while (1) {
		if (phy_LWin_pressed == PHY_KEY_DOWN) {
			Sleep(250);
			continue;
		}

		// 这里使用 count_lwin 做次数控制, 是因为如果使用纯软件做按键模拟, 那么物理按键一定是 PHY_KEY_UP 状态
		// 程序就要执行到这里 如果立即释放, 那么软件模拟的按键可能会失效, 比如软件模拟 win+d
		// 因为立即释放了 win 所以实际输出就只是 d
		while (KEY_DOWN(VK_LWIN)) { // lwin
			count_lwin++;
			Sleep(10);
			if (count_lwin == 10) {
				break;
			}
		}
		if (count_lwin == 10) {
			std::cout << "lwin will releaseL" << std::endl;
			count_lwin = 0;
			keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
		}

		Sleep(250);
	}
}
// ### 多线程函数 end

DWORD WINAPI CheckPhyKeyState(LPVOID lpParam)
{
	(void)lpParam;
	using namespace std;

	InterceptionContext context;
	InterceptionDevice device;
	InterceptionStroke stroke;

	// wchar_t hardware_id[500];

	raise_process_priority();

	context = interception_create_context();

	// interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN |
	// INTERCEPTION_FILTER_KEY_UP);
	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);
	// interception_set_filter(context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_LEFT_BUTTON_DOWN);

	while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0) {
		if (interception_is_keyboard(device)) {
			InterceptionKeyStroke &keystroke = *(InterceptionKeyStroke *)&stroke;
			if (keystroke.code == SCANCODE_LALT) {
				phy_LAlt_pressed = keystroke.state;
				// std::cout << "lalt state: " << keystroke.state << std::endl;
				// keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
				// 启动定时器
				if (g_TimerId_LAlt != 0) {
					KillTimer(NULL, g_TimerId_LAlt);
				}
				g_TimerId_LAlt = SetTimer(NULL, 0, 500, TimerCallback_LAlt);
			}
			if (keystroke.code == SCANCODE_LSHIFT) {
				phy_LShift_pressed = keystroke.state;
				if (g_TimerId_LShift != 0) {
					KillTimer(NULL, g_TimerId_LShift);
				}
				g_TimerId_LShift = SetTimer(NULL, 0, 500, TimerCallback_LShift);
			}
			if (keystroke.code == SCANCODE_LCTRL) {
				phy_LCtrl_pressed = keystroke.state;
				if (g_TimerId_LCtrl != 0) {
					KillTimer(NULL, g_TimerId_LCtrl);
				}
				g_TimerId_LCtrl = SetTimer(NULL, 0, 500, TimerCallback_LCtrl);
			}
			if (keystroke.code == SCANCODE_LWIN) {
				phy_LWin_pressed = keystroke.state;
				if (g_TimerId_LWin != 0) {
					KillTimer(NULL, g_TimerId_LWin);
				}
				g_TimerId_LWin = SetTimer(NULL, 0, 500, TimerCallback_LWin);
			}
		}

		interception_send(context, device, &stroke, 1);
	}

	interception_destroy_context(context);
	return 0;
}

int main()
{
	// 隐藏命令窗口
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	// short count_alt = 0, count_shift = 0, count_ctrl = 0;

	// 创建线程
	g_ThreadHandle = CreateThread(NULL, 0, CheckPhyKeyState, NULL, 0, NULL);
	if (g_ThreadHandle == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	g_ThreadHandle_LAlt = CreateThread(NULL, 0, releaseLAltKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_LAlt == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	g_ThreadHandle_LCtrl = CreateThread(NULL, 0, releaseLCtrlKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_LCtrl == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	g_ThreadHandle_LShift = CreateThread(NULL, 0, releaseLShiftKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_LShift == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	g_ThreadHandle_LWin = CreateThread(NULL, 0, releaseLWinKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_LWin == NULL) {
		printf("线程创建失败\n");
		return 1;
	}

	// 设置退出标志
	g_ExitThread = TRUE;

	// 等待线程退出
	WaitForSingleObject(g_ThreadHandle, INFINITE);
	WaitForSingleObject(g_ThreadHandle_LAlt, INFINITE);
	WaitForSingleObject(g_ThreadHandle_LCtrl, INFINITE);
	WaitForSingleObject(g_ThreadHandle_LShift, INFINITE);
	WaitForSingleObject(g_ThreadHandle_LWin, INFINITE);

	// 关闭线程句柄
	CloseHandle(g_ThreadHandle);
	CloseHandle(g_ThreadHandle_LAlt);
	CloseHandle(g_ThreadHandle_LCtrl);
	CloseHandle(g_ThreadHandle_LShift);
	CloseHandle(g_ThreadHandle_LWin);

	return 0;
}
