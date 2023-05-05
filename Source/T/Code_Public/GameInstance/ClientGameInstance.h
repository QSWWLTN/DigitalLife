#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"
#include "Common/TcpListener.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Misc/ScopeLock.h"
#include "Engine/DataTable.h"

#include "../../Code_Public/Struct/PublicStruct.h"
#include "../../Code_Public/Tools/JsonObjectConverterEx.h"

#include "../../Code_AsyncTask/SendMessageAsyncTask.h"
#include "../../Code_AsyncTask/ReadMessageAsyncTask.h"
#include "../../Code_AsyncTask/HookKeyboardAsyncTask.h"

#include "../Tools/JsonObjectConverterEx.h"

#include "ClientGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDownLoadFileEnd, FString, TargetFilePath, uint8, StateIndex, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPutFileEnd, bool, State);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchPlayerMesh, uint8, Flag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserKeyBoardInput, uint8, KeyBoardValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffIsEmpty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitServerData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressKeyboard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnServerClose);

UCLASS()
class UClientGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UClientGameInstance();

public:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	UFUNCTION(BlueprintCallable)
		bool InitSocket(UDataTable* SettingTable);
	UFUNCTION(BlueprintCallable)
		bool InitSocketFromString(FString Host, int32 Port);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		uint8 GetSocketState() {
			if (Socket != nullptr) {
				return Socket->GetConnectionState();
			}
			return ESocketConnectionState::SCS_NotConnected;
		}

	UFUNCTION(BlueprintCallable)
		void SendWavFile(FString WavFilePath);
	void CloseBuff() { 
		{
			FScopeLock ScopeLock(&Lock_TestValue);
			ClientBuffData.Empty();
			BuffDataSize = 0;
		}	
	};

	UFUNCTION(BlueprintCallable)
		void NextWav();
	UFUNCTION(BlueprintCallable)
		void RemoveAttachWavFileInfo();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetBuffWavFileNum() { return AttachWavFilePathList.Num(); };

	UFUNCTION(BlueprintCallable)
		TArray<FHotKeyStruct> SetKeyList(int32 Key, int32 CharKey);

	UFUNCTION(BlueprintCallable)
		void MoveWindowToPos();
	UFUNCTION(BlueprintCallable)
		void ReWindowsSize(FVector2D Size);
	UFUNCTION(BlueprintCallable)
		void GameExitEvent();

public:
	UPROPERTY(BlueprintAssignable)
		FOnDownLoadFileEnd OnDownLoadFileEnd;
	UPROPERTY(BlueprintAssignable)
		FOnPutFileEnd OnPutFileEnd;
	UPROPERTY(BlueprintAssignable)
		FOnBuffIsEmpty OnBuffIsEmpty;
	UPROPERTY(BlueprintAssignable)
		FOnWaitServerData OnWaitServerData;

	UPROPERTY(BlueprintAssignable)
		FOnSwitchPlayerMesh OnSwitchPlayerMesh;
	UPROPERTY(BlueprintAssignable)
		FOnPressKeyboard OnPressKeyboard;

	UPROPERTY(BlueprintAssignable)
		FOnServerClose OnServerClose;

public:
	bool StopSocket = true;
	uint64 BuffDataSize = 0;
	TArray<uint8> ClientBuffData;

	TArray<FAttachWavFileInfo> AttachWavFilePathList;

public:
	UPROPERTY(BlueprintReadWrite)
		FHotKeyList KeyList;
	const FString HotKeyConfigPath = FPaths::ProjectConfigDir() + TEXT("HotKey.json");
	UPROPERTY(BlueprintReadWrite)
		bool IsInputHotKey = false;

private:
	FCriticalSection Lock_TestValue;
	FSocket* Socket;
};
