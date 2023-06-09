#include "UserSetWidget.h"

#include "HAL/PlatformFramePacer.h"

void UUserSetWidget::NativePreConstruct() {
	Super::NativePreConstruct();

	InitWidget();
}

void UUserSetWidget::InitWidget() {
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerInfo"), 0)) {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));

		if (GameSaveObject->Config[1]) {
			SwitchBackground->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
		if (GameSaveObject->Config[2]) {
			LimitFPS->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
		if (GameSaveObject->Config[3]) {
			FullScreen->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}

		UpdateSize->SetValue(GameSaveObject->FOVValue);
		FOVValue = GameSaveObject->FOVValue;

		//ApplySaveSet();
	}
}

void UUserSetWidget::UpdateMaxFPS(bool IsClick) {
	if (LimitFPS->IsValidLowLevel()) {
#if PLATFORM_ANDROID
		if (IsClick) {
			FPlatformRHIFramePacer::SetFramePace(60);
			GEngine->SetMaxFPS(60);
		}
		else {
			if (FPlatformRHIFramePacer::SupportsFramePace(144)) {
				FPlatformRHIFramePacer::SetFramePace(144);
			}
			else if (FPlatformRHIFramePacer::SupportsFramePace(120)) {
				FPlatformRHIFramePacer::SetFramePace(120);
			}
			GEngine->SetMaxFPS(10000);
		}
#else
		if (IsClick) {
			GEngine->bUseFixedFrameRate = true;
			GEngine->FixedFrameRate = 60;
		}
		else {
			GEngine->bUseFixedFrameRate = false;
			GEngine->SetMaxFPS(FPlatformRHIFramePacer::GetFramePace());
		}
#endif
	}
}

void UUserSetWidget::ApplySaveSet() {
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerInfo"), 0)) {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));

		UpdateFOV(GameSaveObject->FOVValue);
		FOVValue = GameSaveObject->FOVValue;
		UpdateBackground((bool)GameSaveObject->Config[1]);
		UpdateMaxFPS((bool)GameSaveObject->Config[2]);

#if !PLATFORM_ANDROID
		UpdateWindowState((bool)GameSaveObject->Config[3]);
#endif

		UpdateQualitySetting(GEngine->GameUserSettings->GetTextureQuality());
	}
}

bool UUserSetWidget::SetGameSaveObjectConfig(int32 Index, int32 Value) {
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerInfo"), 0)) {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));

		if (GameSaveObject->Config.IsValidIndex(Index)) {
			GameSaveObject->Config[Index] = Value;
			UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
			return true;
		}
	}
	else {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::CreateSaveGameObject(UGameSaveObject::StaticClass()));
		GameSaveObject->Config.Init(0, 4);

		if (GameSaveObject->Config.IsValidIndex(Index)) {
			GameSaveObject->Config[Index] = Value;
			UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
			return true;
		}
	}
	return false;
}

bool UUserSetWidget::GetGameSaveObjectConfig(int32 Index) {
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerInfo"), 0)) {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
		if (GameSaveObject->Config.IsValidIndex(Index)) {
			return (bool)GameSaveObject->Config[Index];
		}
	}
	return false;
}

void UUserSetWidget::SetFOV(int32 FOV, float Value) {
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerInfo"), 0)) {
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));

		GameSaveObject->FOVValue = Value;
		UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	}
}

void UUserSetWidget::SetQualityIndex(int32 Index) {
	UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
	if (GameSaveObject->IsValidLowLevel()) {
		GameSaveObject->QualityIndex = Index;
		UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	}

	UGameUserSettings* Setting = GEngine->GameUserSettings;

	Setting->SetTextureQuality(Index);
	Setting->SetShadowQuality(Index);
	Setting->SetAntiAliasingQuality(Index);
	Setting->SetGlobalIlluminationQuality(Index);
	Setting->SetPostProcessingQuality(Index);
	Setting->SetReflectionQuality(Index);
	Setting->SetShadingQuality(Index);

	Setting->ApplySettings(true);
	Setting->SaveSettings();
}