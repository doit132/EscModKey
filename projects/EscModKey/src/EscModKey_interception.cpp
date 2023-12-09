#include "interception.h"
#include "utils.h"

#include <iostream>
#include <stdio.h>
#include <windows.h>

#define KEY_DOWN(vKey) ((GetAsyncKeyState(vKey) & 0x8000) ? 1 : 0)

enum ScanCode {
	SCANCODE_LWIN = 0x5B,
	SCANCODE_LALT = 0x38,
	SCANCODE_RALT = 0x138,
	SCANCODE_LSHIFT = 0x2A,
	SCANCODE_RSHIFT = 0x136,
	SCANCODE_LCTRL = 0x1D,
	SCANCODE_RCTRL = 0x11D
};

enum VirtCode {
	VIRTCODE_LWIN = 91,
	VIRTCODE_LALT = 18,
	VIRTCODE_RALT = 165,
	VIRTCODE_LSHIFT = 16,
	VIRTCODE_RSHIFT = 161,
	VIRTCODE_LCTRL = 17,
	VIRTCODE_RCTRL = 163
};

enum PhyKeyState { PHY_KEY_DOWN = false, PHY_KEY_UP = true };

// 全局变量，用于线程同步
HANDLE g_ThreadHandle = NULL;
HANDLE g_ThreadHandle_Alt = NULL;
HANDLE g_ThreadHandle_Shift = NULL;
HANDLE g_ThreadHandle_Ctrl = NULL;
HANDLE g_ThreadHandle_LWin = NULL;

BOOL g_ExitThread = FALSE;

bool phy_Alt_pressed = PHY_KEY_UP;
bool phy_Ctrl_pressed = PHY_KEY_UP;
bool phy_Shift_pressed = PHY_KEY_UP;
bool phy_LWin_pressed = PHY_KEY_UP;

// 全局变量，定时器ID
UINT_PTR g_TimerId_Alt = 0;
UINT_PTR g_TimerId_Shift = 0;
UINT_PTR g_TimerId_Ctrl = 0;
UINT_PTR g_TimerId_LWin = 0;

// ### 定时器回调函数 begin
VOID CALLBACK TimerCallback_Ctrl(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_Ctrl);
	g_TimerId_Ctrl = 0;
	phy_Ctrl_pressed = PHY_KEY_DOWN;
}

VOID CALLBACK TimerCallback_Shift(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_Shift);
	g_TimerId_Shift = 0;
	phy_Shift_pressed = PHY_KEY_DOWN;
}

VOID CALLBACK TimerCallback_Alt(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	(void)dwTime;
	(void)idEvent;
	(void)hwnd;
	(void)uMsg;
	// 停止定时器
	KillTimer(NULL, g_TimerId_Alt);
	g_TimerId_Alt = 0;
	phy_Alt_pressed = PHY_KEY_DOWN;
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
	phy_Alt_pressed = PHY_KEY_DOWN;
}
// ### 定时器回调函数 end

// ### 多线程函数 begin
DWORD WINAPI releaseCtrlKeyState(LPVOID lpParam)
{
	(void)lpParam;

	volatile short count_ctrl = 0;
	while (1) {
		if (phy_Ctrl_pressed == PHY_KEY_DOWN) {
			// printf("phy_ctrl is down\n");
			Sleep(50);
			continue;
		}
		// 这里使用 count_ctrl 做次数控制, 是因为如果使用纯软件做按键模拟,
		// 那么物理按键一定是 PHY_KEY_UP 状态 程序就要执行到这里 如果立即释放,
		// 那么软件模拟的按键可能会失效, 比如软件模拟 ctrl+w 因为立即释放了 ctrl
		// 所以实际输出就只是 w
		while (KEY_DOWN(VK_LCONTROL) || KEY_DOWN(VK_RCONTROL)) { // ctrl
			count_ctrl++;
			Sleep(5);
			if (count_ctrl == 10) {
				break;
			}
		}
		if (count_ctrl == 10) {
			// std::cout << "lctrl will releaseL" << std::endl;
			keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RCONTROL, 0, KEYEVENTF_KEYUP, 0);
		}
		count_ctrl = 0;
		Sleep(50);
	}
}

DWORD WINAPI releaseLShiftKeyState(LPVOID lpParam)
{
	(void)lpParam;

	volatile short count_shift = 0;
	while (1) {
		if (phy_Shift_pressed == PHY_KEY_DOWN) {
			// printf("phy_lshift is down\n");
			Sleep(50);
			continue;
		}

		// 这里使用 count_shift 做次数控制, 是因为如果使用纯软件做按键模拟,
		// 那么物理按键一定是 PHY_KEY_UP 状态 程序就要执行到这里 如果立即释放,
		// 那么软件模拟的按键可能会失效, 比如软件模拟 shift+f4 因为立即释放了 shift
		// 所以实际输出就只是 f4
		while (KEY_DOWN(VK_LSHIFT) || KEY_DOWN(VK_RSHIFT)) { // shift
			count_shift++;
			Sleep(5);
			if (count_shift == 10) {
				break;
			}
		}
		if (count_shift == 10) {
			// std::cout << "lshift will releaseL" << std::endl;
			keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RSHIFT, 0, KEYEVENTF_KEYUP, 0);
		}
		count_shift = 0;
		Sleep(50);
	}
}

DWORD WINAPI releaseAltKeyState(LPVOID lpParam)
{
	(void)lpParam;

	volatile short count_alt = 0;
	while (1) {
		if (phy_Alt_pressed == PHY_KEY_DOWN) {
			// printf("phy_alt is down\n");
			Sleep(50);
			continue;
		}

		// 这里使用 count_alt 做次数控制, 是因为如果使用纯软件做按键模拟,
		// 那么物理按键一定是 PHY_KEY_UP 状态 程序就要执行到这里 如果立即释放,
		// 那么软件模拟的按键可能会失效, 比如软件模拟 alt+f4 因为立即释放了 alt
		// 所以实际输出就只是 f4
		while (KEY_DOWN(VK_LMENU) || KEY_DOWN(VK_RMENU)) { // alt
			count_alt++;
			Sleep(5);
			if (count_alt == 10) {
				break;
			}
		}
		if (count_alt == 10) {
			// std::cout << "alt will releaseL" << std::endl;
			keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RMENU, 0, KEYEVENTF_KEYUP, 0);
		}
		count_alt = 0;
		Sleep(50);
	}
}

DWORD WINAPI releaseLWinKeyState(LPVOID lpParam)
{
	(void)lpParam;

	volatile short count_lwin = 0;
	while (1) {
		if (phy_LWin_pressed == PHY_KEY_DOWN) {
			Sleep(50);
			continue;
		}

		// 这里使用 count_lwin 做次数控制, 是因为如果使用纯软件做按键模拟,
		// 那么物理按键一定是 PHY_KEY_UP 状态 程序就要执行到这里 如果立即释放,
		// 那么软件模拟的按键可能会失效, 比如软件模拟 win+d 因为立即释放了 win
		// 所以实际输出就只是 d
		while (KEY_DOWN(VK_LWIN)) { // lwin
			count_lwin++;
			Sleep(5);
			if (count_lwin == 10) {
				break;
			}
		}
		if (count_lwin == 10) {
			// std::cout << "lwin will releaseL" << std::endl;
			keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
		}
		count_lwin = 0;
		Sleep(50);
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
	// interception_set_filter(context, interception_is_mouse,
	// INTERCEPTION_FILTER_MOUSE_LEFT_BUTTON_DOWN);

	while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0) {
		if (interception_is_keyboard(device)) {
			InterceptionKeyStroke &keystroke = *(InterceptionKeyStroke *)&stroke;
			if (keystroke.code == SCANCODE_LALT || keystroke.code == SCANCODE_RALT) {
				phy_Alt_pressed = keystroke.state;
				// std::cout << "lalt state: " << keystroke.state << std::endl;
				// keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
				// 启动定时器
				if (g_TimerId_Alt != 0) {
					KillTimer(NULL, g_TimerId_Alt);
				}
				g_TimerId_Alt = SetTimer(NULL, 0, 500, TimerCallback_Alt);
			}
			if (keystroke.code == SCANCODE_LSHIFT ||
			    keystroke.code == SCANCODE_RSHIFT) {
				phy_Shift_pressed = keystroke.state;
				if (g_TimerId_Shift != 0) {
					KillTimer(NULL, g_TimerId_Shift);
				}
				g_TimerId_Shift = SetTimer(NULL, 0, 500, TimerCallback_Shift);
			}
			if (keystroke.code == SCANCODE_LCTRL || keystroke.code == SCANCODE_RCTRL) {
				phy_Ctrl_pressed = keystroke.state;
				if (g_TimerId_Ctrl != 0) {
					KillTimer(NULL, g_TimerId_Ctrl);
				}
				g_TimerId_Ctrl = SetTimer(NULL, 0, 500, TimerCallback_Ctrl);
			}
			// 经过测试发现win的物理按键和虚拟按键的键码都一个样, 所以无法判断
			// if (keystroke.code == SCANCODE_LWIN) {
			// 	phy_LWin_pressed = keystroke.state;
			// 	if (g_TimerId_LWin != 0) {
			// 		KillTimer(NULL, g_TimerId_LWin);
			// 	}
			// 	g_TimerId_LWin = SetTimer(NULL, 0, 500, TimerCallback_LWin);
			// }
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
	g_ThreadHandle_Alt = CreateThread(NULL, 0, releaseAltKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_Alt == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	g_ThreadHandle_Ctrl = CreateThread(NULL, 0, releaseCtrlKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_Ctrl == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	g_ThreadHandle_Shift = CreateThread(NULL, 0, releaseLShiftKeyState, NULL, 0, NULL);
	if (g_ThreadHandle_Shift == NULL) {
		printf("线程创建失败\n");
		return 1;
	}
	// g_ThreadHandle_LWin = CreateThread(NULL, 0, releaseLWinKeyState, NULL, 0, NULL);
	// if (g_ThreadHandle_LWin == NULL) {
	// 	printf("线程创建失败\n");
	// 	return 1;
	// }

	// 设置退出标志
	g_ExitThread = TRUE;

	// 等待线程退出
	WaitForSingleObject(g_ThreadHandle, INFINITE);
	WaitForSingleObject(g_ThreadHandle_Alt, INFINITE);
	WaitForSingleObject(g_ThreadHandle_Ctrl, INFINITE);
	WaitForSingleObject(g_ThreadHandle_Shift, INFINITE);
	// WaitForSingleObject(g_ThreadHandle_LWin, INFINITE);

	// 关闭线程句柄
	CloseHandle(g_ThreadHandle);
	CloseHandle(g_ThreadHandle_Alt);
	CloseHandle(g_ThreadHandle_Ctrl);
	CloseHandle(g_ThreadHandle_Shift);
	// CloseHandle(g_ThreadHandle_LWin);

	return 0;
}
