#include "TestFunctionLibrary.h"

#include "Engine/UserDefinedEnum.h"

#include "../Code_AsyncTask/HookKeyboardAsyncTask.h"

FString UTestFunctionLibrary::GetMoodTypeName(UUserDefinedEnum* EnumPtr, uint8 Index, UDataTable* Table, int32& Num) {

	FString Ret;

	if (EnumPtr->IsValidLowLevel()) {
		Ret = EnumPtr->GetNameStringByValue(Index);
		if (EnumPtr->DisplayNameMap.Contains(*Ret)) {
			Ret = EnumPtr->DisplayNameMap[*Ret].ToString();
		}
	}

	if (Table->IsValidLowLevel()) {
		Num = 0;
		for (auto& Data : Table->GetRowMap()) {
			if (Data.Key.ToString().Contains(Ret)) {
				Num++;
			}
		}
	}
	return Ret;
}

FString UTestFunctionLibrary::GetRootPath() {
#if PLATFORM_ANDROID
	extern FString GFilePathBase;
	return GFilePathBase + FString("/UnrealGame/") + FApp::GetProjectName() + TEXT("/") + FApp::GetProjectName() + FString("/");
#else
	return FPaths::ProjectDir();
#endif
}

void UTestFunctionLibrary::InitRecorder(FString Path) {
	AsyncTask(ENamedThreads::BackgroundThreadPriority, [=]() {
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jstring FilePath = Env->NewStringUTF(TCHAR_TO_UTF8(*Path));
			jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_InitRecorder", "(Ljava/lang/String;)V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GetPackageNameMethodID, FilePath);
		}
#endif	
	});
}

void UTestFunctionLibrary::StopRecorder() {
	AsyncTask(ENamedThreads::BackgroundThreadPriority, [=]() {
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StopRecorder", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GetPackageNameMethodID);
		}
#endif
	});
}

void UTestFunctionLibrary::PlayShake() {
	AsyncTask(ENamedThreads::BackgroundThreadPriority, [=]() {
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PlayShake", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GetPackageNameMethodID);
		}
#endif
	});
}

void UTestFunctionLibrary::QS_DeleteFile(FString Path) {
	AsyncTask(ENamedThreads::BackgroundThreadPriority, [=]() {
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jstring FilePath = Env->NewStringUTF(TCHAR_TO_UTF8(*Path));
			jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_DeleteFileFromPath", "(Ljava/lang/String;)V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GetPackageNameMethodID, FilePath);
		}
#endif
	});
}

void UTestFunctionLibrary::SetWindowsSize(FVector2D Size, bool Adsorption) {
#if PLATFORM_WINDOWS
	UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
	if (!GameEngine->IsValidLowLevel()) {
		return;
	}
	UGameUserSettings* Setting = GameEngine->GameUserSettings;

	TSharedPtr<SWindow> Windows = GameEngine->GameViewportWindow.Pin();

	if (Adsorption) {
		RECT rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

		FVector2D NowSize(GetSystemMetrics(SM_CXSCREEN), rt.bottom);
		Windows->ReshapeWindow(NowSize - Size, Size);

		Setting->SetWindowPosition((NowSize - Size).X, (NowSize - Size).Y);
		Setting->ApplySettings(false);
	}
	else {
		Windows->Resize(Size);
	}
#endif
}

FVector2D UTestFunctionLibrary::GetUserWindowsSize() {
#if !PLATFORM_ANDROID
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	return FVector2D(rt.right, rt.bottom);
#endif
	return FVector2D();
}

void UTestFunctionLibrary::MinimizeWindow() {
#if !PLATFORM_ANDROID
	UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
	if (!GameEngine->IsValidLowLevel()) {
		return;
	}

	TSharedPtr<SWindow> Windows = GameEngine->GameViewportWindow.Pin();
	Windows->Minimize();
#endif
}

FString UTestFunctionLibrary::VirtualKeyValueToString(int32 Key, int32 CharKey) {
	return FInputKeyManager::Get().GetKeyFromCodes(Key, CharKey).ToString();
}

FString UTestFunctionLibrary::RemoveStringEnd(FString Str, const FString& TargetStr) {
	Str.RemoveFromEnd(TargetStr);
	return Str;
}

bool UTestFunctionLibrary::HaveCommand(FString& IP, int32& Port) {
	const TCHAR* Cmd = FCommandLine::Get();
	return FParse::Value(Cmd, TEXT("-IP="), IP) && FParse::Value(Cmd, TEXT("-Port="), Port);
}

void UTestFunctionLibrary::QuitGame() {
	AsyncTask(ENamedThreads::GameThread, [=]() {
		FGenericPlatformMisc::RequestExit(true);
	});
}