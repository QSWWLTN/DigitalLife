#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

#include "../Code_Public/GameSaveObject.h"

#include "UserSetWidget.generated.h"

UCLASS()
class T_API UUserSetWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
		void InitWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateBackground(bool IsClick);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateFOV(float Value);
	UFUNCTION(BlueprintCallable)
		void UpdateMaxFPS(bool IsClick);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateWindowState(bool IsClick);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateQualitySetting(int32 Index);

	UFUNCTION(BlueprintCallable)
		void ApplySaveSet();
	UFUNCTION(BlueprintCallable)
		/*
			设置Index对应的值
			@param Index 控件编号
			
			0 - 保留
			1 - 切换背景
			2 - 限制FPS
			3 - 全屏
		*/
		bool SetGameSaveObjectConfig(int32 Index, int32 Value);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetGameSaveObjectConfig(int32 Index);

	UFUNCTION(BlueprintCallable)
		void SetFOV(int32 FOV, float Value);
	UFUNCTION(BlueprintCallable)
		void SetQualityIndex(int32 Index);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* UpdateSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* QualitySelectBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* SwitchBackground;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* LimitFPS;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* FullScreen;

public:
	UPROPERTY(BlueprintReadWrite)
		float FOVValue = 0.f;
};
