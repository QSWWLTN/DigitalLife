#include "SelectKeyWidget.h"
#include "Kismet/GameplayStatics.h"

#include "../Code_Public/GameInstance/ClientGameInstance.h"
#include "../Code_Public/GameSaveObject.h"

FReply USelectKeyWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);

	UClientGameInstance* GameInstance = Cast<UClientGameInstance>(GetGameInstance());
	if (!GameInstance->IsValidLowLevel()) {
		return FReply::Handled();
	}

	if (DownKeyNum == 0) {
		GameInstance->KeyList.KeyList.Empty();
		UGameSaveObject* GameSaveObject = Cast<UGameSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInfo"), 0));
		if (GameSaveObject->IsValidLowLevel()) {
			GameSaveObject->KeyList.KeyList.Empty();
		}
		UGameplayStatics::SaveGameToSlot(GameSaveObject, TEXT("PlayerInfo"), 0);
	}

	FHotKeyStruct Temp;
	Temp.KeyCode = InKeyEvent.GetKeyCode();
	Temp.CharCode = InKeyEvent.GetCharacter();

	if (!GameInstance->KeyList.KeyList.Contains(Temp)) {
		DownKeyNum++;
	}

	TArray<FHotKeyStruct> T = GameInstance->SetKeyList(Temp.KeyCode, Temp.CharCode);

	FString RetStr = TEXT("");
	for (auto& Data : T) {
		RetStr += FInputKeyManager::Get().GetKeyFromCodes(Data.KeyCode, Data.CharCode).ToString();
		RetStr += TEXT(" + ");
	}
	RetStr.RemoveFromEnd(TEXT(" + "));

	OnOverrideKeyDownEvent.Broadcast(RetStr, T.Num());

	return FReply::Handled();
}

FReply USelectKeyWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	Super::NativeOnKeyUp(InGeometry, InKeyEvent);

	DownKeyNum--;
	if (DownKeyNum < 0) {
		DownKeyNum = 0;
	}
	return FReply::Handled();
}