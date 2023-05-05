#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/GameEngine.h"
#include "Widgets/SWindow.h"
#include "Misc/Parse.h"
#include "Misc/CommandLine.h"
#include "GameFramework/GameUserSettings.h"
#include "InputCoreTypes.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include<Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJavaEnv.h"
#endif

#include "TestFunctionLibrary.generated.h"

UCLASS()
class T_API UTestFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		static FString GetMoodTypeName(UUserDefinedEnum* EnumPtr, uint8 Index, UDataTable* Table, int32& Num);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString GetRootPath();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static bool IsAndroid() {
#if PLATFORM_ANDROID
		return true;
#endif
		return false;
		}

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static bool IsDeubgMode() {
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		return true;
#endif
		return false;
		}

	UFUNCTION(BlueprintCallable)
		static void DeleteFile(FString FilePath) {
			if (FPaths::FileExists(*FilePath)) {
				FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath);
			}
		}
	UFUNCTION(BlueprintCallable)
		static void DeleteDir(FString DirPath) {
			if (FPaths::DirectoryExists(*DirPath)) {
				FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*DirPath);
			}
		}

	UFUNCTION(BlueprintCallable)
		static void InitRecorder(FString Path);
	UFUNCTION(BlueprintCallable)
		static void StopRecorder();
	UFUNCTION(BlueprintCallable)
		static void PlayShake();
	UFUNCTION(BlueprintCallable)
		static void QS_DeleteFile(FString Path);

	UFUNCTION(BlueprintCallable)
		static void SetWindowsSize(FVector2D Size, bool Adsorption);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector2D GetUserWindowsSize();
	UFUNCTION(BlueprintCallable)
		static void MinimizeWindow();

	UFUNCTION(BlueprintCallable)
		static FString VirtualKeyValueToString(int32 Key, int32 CharKey);
	UFUNCTION(BlueprintCallable)
		static FString RemoveStringEnd(FString Str, const FString& TargetStr);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static bool HaveCommand(FString& IP, int32& Port);
	UFUNCTION(BlueprintCallable)
		static void QuitGame();
};
