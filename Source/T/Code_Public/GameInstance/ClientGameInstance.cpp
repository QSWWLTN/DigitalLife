#include "ClientGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "../GameSaveObject.h"

#if PLATFORM_WINDOWS

#include <windows/WindowsWindow.h>
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <shellapi.h>
#include <winuser.h>
#include "Windows/HideWindowsPlatformTypes.h"

#endif

#if !NO_PROTOBUF

#include "libProtobuf/include/google/protobuf/message.h"
#include "../../Code_Struct/DigitalLife.pb.h"

#endif

UClientGameInstance::UClientGameInstance() {
	
}

void UClientGameInstance::Init() {
	Super::Init();

	bool Success = FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + TEXT("Temp")));
#if !WITH_EDITOR
	Success &= FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + TEXT("Movies")));
#endif

	Success &= FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + TEXT("Temp")));
#if !WITH_EDITOR
	Success &= FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + TEXT("Movies")));
#endif

	if (!UGameplayStatics::DoesSaveGameExist(TEXT("PlayerInfo"), 0)) {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::CreateSaveGameObject(UGameSaveObject::StaticClass()));
		GameSaveObject->Config.Init(0, 4);

		GameSaveObject->Config[1] = 1;			//默认黑色背景
		GameSaveObject->Config[2] = 1;			//默认限制FPS

		FHotKeyStruct T;
		T.KeyCode = '`';
		T.CharCode = '`';
		GameSaveObject->KeyList.KeyList.Add(T);	//默认快捷键`

		UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	}

#if !PLATFORM_ANDROID
	if (FPaths::FileExists(HotKeyConfigPath)) {
		FString JsonString;
		FFileHelper::LoadFileToString(JsonString, *HotKeyConfigPath);
		FJsonObjectConverterEX::JsonObjectStringToUStruct(JsonString, &KeyList, 0, 0);

		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
		GameSaveObject->KeyList = KeyList;
		UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	}

	UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
	KeyList = GameSaveObject->KeyList;
#endif
}

void UClientGameInstance::Shutdown() {
	Super::Shutdown();

	GameExitEvent();
	ClientBuffData.Empty();
}

bool UClientGameInstance::InitSocket(UDataTable* SettingTable) {
	if (!SettingTable->IsValidLowLevel()) {
		return false;
	}
	FConfig* T = SettingTable->FindRow<FConfig>(TEXT("ServerInfo"), nullptr);
	if (!T) {
		return false;
	}

	return InitSocketFromString(T->ServerIP, T->ServerPort);
}

bool UClientGameInstance::InitSocketFromString(FString Host, int32 Port) {
	FIPv4Address IP;
	FIPv4Address::Parse(Host, IP);

	TSharedPtr<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(IP.Value);
	addr->SetPort(Port);

	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_Client"), false);
	Socket->SetNonBlocking();
	if (!Socket->Connect(*addr)) {
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode())));

		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
		StopSocket = true;
		return false;
	}
	int32 Size;
	Socket->SetSendBufferSize(1024000000, Size);
	Socket->SetReceiveBufferSize(102400000, Size);
	StopSocket = false;

	(new FAutoDeleteAsyncTask<FSendMessageAsyncTask>(this, Socket))->StartBackgroundTask();
	(new FAutoDeleteAsyncTask<FReadMessageAsyncTask>(this, Socket))->StartBackgroundTask();
#if PLATFORM_WINDOWS
	(new FAutoDeleteAsyncTask<FHookKeyboardAsyncTask>(this))->StartBackgroundTask();
#endif
	return true;
}

void UClientGameInstance::SendWavFile(FString WavFilePath) {
	{
		FScopeLock ScopeLock(&Lock_TestValue);
		TArray<uint8> WavFileBuff;
		FFileHelper::LoadFileToArray(WavFileBuff, *WavFilePath);
#if NO_PROTOBUF
		WavFileBuff.Add('?');
		WavFileBuff.Add('!');

		ClientBuffData = WavFileBuff;
		BuffDataSize = WavFileBuff.Num();
#else
		DigitalLifeNameSpace::C_SendData SendData;
		SendData.set_basetype(DigitalLifeNameSpace::BaseMessageType::BASEMESSAGETYPE_CLIENTSENDDATA);
		SendData.set_bsuccess(true);

		SendData.set_wavfiledata(WavFileBuff.GetData(), WavFileBuff.Num());
		ClientBuffData.Init(0, SendData.ByteSize());

		if (!SendData.SerializeToArray(ClientBuffData.GetData(), ClientBuffData.Num())) {
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, TEXT("序列化消息失败"));
			return;
		}
		ClientBuffData.Add('?');
		ClientBuffData.Add('!');

		BuffDataSize = ClientBuffData.Num();
#endif
	}
}

void UClientGameInstance::NextWav() {
	if (AttachWavFilePathList.Num() != 0) {
		FAttachWavFileInfo T = AttachWavFilePathList[0];
		OnDownLoadFileEnd.Broadcast(T.WavFilePath, T.StateIndex, true);

		if (AttachWavFilePathList.Num() == 0) {
			OnBuffIsEmpty.Broadcast();
		}
	}
	else {
#if !PLATFORM_ANDROID
		FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*(UTestFunctionLibrary::GetRootPath() + TEXT("Temp/Temp")));
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*(UTestFunctionLibrary::GetRootPath() / TEXT("Temp/Read.wav")));
#else
		FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*(UTestFunctionLibrary::GetRootPath() + TEXT("Content/Movies/Temp")));
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*(UTestFunctionLibrary::GetRootPath() / TEXT("Content/Movies/Read.wav")));
#endif
		OnBuffIsEmpty.Broadcast();
	}
}

void UClientGameInstance::RemoveAttachWavFileInfo() {
	if (AttachWavFilePathList.Num() != 0) {
		AttachWavFilePathList.RemoveAt(0);
	} 
}

TArray<FHotKeyStruct> UClientGameInstance::SetKeyList(int32 Key, int32 CharKey) {
	UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
	if (!GameSaveObject->IsValidLowLevel()) {
		return TArray<FHotKeyStruct>();
	}

	FHotKeyStruct T;
	T.KeyCode = Key;
	T.CharCode = CharKey;

	if (GameSaveObject->KeyList.KeyList.Contains(T)) {
		return GameSaveObject->KeyList.KeyList;
	}

	GameSaveObject->KeyList.KeyList.Add(T);

	UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	KeyList = GameSaveObject->KeyList;

	FString JsonString;
	FJsonObjectConverterEX::UStructToJsonObjectString(KeyList.StaticStruct(), &KeyList, JsonString, 0, 0, 0);
	FFileHelper::SaveStringToFile(JsonString, *HotKeyConfigPath);

	return GameSaveObject->KeyList.KeyList;
}

void UClientGameInstance::MoveWindowToPos() {
#if PLATFORM_WINDOWS
	UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
	int32 Index = GetSystemMetrics(SM_CMONITORS);
	if (GEngine && GEngine->GameViewport && Index != 1 && GameSaveObject->WindowPoint.X != -1) {
		GEngine->GameViewport->GetWindow()->MoveWindowTo(GameSaveObject->WindowPoint);
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, GameSaveObject->WindowPoint.ToString());
	}
#endif
}

void UClientGameInstance::ReWindowsSize(FVector2D Size) {
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetWindow()->Resize(Size);
	}
}

void UClientGameInstance::GameExitEvent() {
	if (Socket != nullptr) {
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
	StopSocket = true;
	//FMemory::Free(ClientBuffData);

#if PLATFORM_WINDOWS && !WITH_EDITOR
	int32 Index = GetSystemMetrics(SM_CMONITORS);
	if (Index != 1) {
		TSharedPtr<FGenericWindow> NativeWindow = GEngine->GameViewport->GetWindow()->GetNativeWindow();

		int32 X, Y, Width, Height;
		NativeWindow->GetFullScreenInfo(X, Y, Width, Height);

		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
		GameSaveObject->WindowPoint.X = X;
		GameSaveObject->WindowPoint.Y = Y;
		UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	}
#endif
}