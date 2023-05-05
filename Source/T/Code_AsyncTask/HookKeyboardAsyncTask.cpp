#include "HookKeyboardAsyncTask.h"
#include "Engine/GameEngine.h"
#include "../Code_Public/GameInstance/ClientGameInstance.h"

#if PLATFORM_WINDOWS

#include "Windows/AllowWindowsPlatformTypes.h"
#include "windows.h"
#include "Windows/HideWindowsPlatformTypes.h"

static HHOOK WindowsHook;

#endif

#if PLATFORM_WINDOWS

LRESULT CALLBACK CallHookEvent(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* KBDLL = (KBDLLHOOKSTRUCT*)lParam;
	if (nCode == HC_ACTION) {
		
		UGameEngine* T = Cast<UGameEngine>(GEngine);
		UClientGameInstance* GameInstance = nullptr;
		if (T->IsValidLowLevel()) {
			GameInstance = Cast<UClientGameInstance>(T->GameInstance);
		}
		if (GameInstance == nullptr || GameInstance->KeyList.KeyList.Num() < 1 || GameInstance->IsInputHotKey) {
			return CallNextHookEx(WindowsHook, nCode, wParam, lParam);
		}

		bool Success = KBDLL->vkCode == GameInstance->KeyList.KeyList[GameInstance->KeyList.KeyList.Num() - 1].KeyCode || KBDLL->vkCode == GameInstance->KeyList.KeyList[GameInstance->KeyList.KeyList.Num() - 1].CharCode;
		for (int32 i = 0; i < GameInstance->KeyList.KeyList.Num() - 1; i++) {
			Success &= ((bool)(GetAsyncKeyState(GameInstance->KeyList.KeyList[i].KeyCode) & 0x8000)) || ((bool)(GetAsyncKeyState(GameInstance->KeyList.KeyList[i].CharCode) & 0x8000));
		}

		if (Success) {
			AsyncTask(ENamedThreads::GameThread, [=]() {
				GameInstance->OnPressKeyboard.Broadcast();
			});
		}
	}
	return CallNextHookEx(WindowsHook, nCode, wParam, lParam);
}

#endif

FHookKeyboardAsyncTask::FHookKeyboardAsyncTask(UObject* Owner):
	Owner(Owner)
{
#if PLATFORM_WINDOWS
	if (WindowsHook != nullptr) {
		return;
	}
	WindowsHook = SetWindowsHookEx(WH_KEYBOARD_LL, CallHookEvent, nullptr, 0);
#endif
}

FHookKeyboardAsyncTask::~FHookKeyboardAsyncTask() {
#if PLATFORM_WINDOWS

	if (WindowsHook != nullptr) {
		UnhookWindowsHookEx(WindowsHook);
		WindowsHook = nullptr;
	}

#endif
}

void FHookKeyboardAsyncTask::DoWork() {
#if PLATFORM_WINDOWS
	while (true) {
		FPlatformProcess::Sleep(0.5);
	}
#endif
}