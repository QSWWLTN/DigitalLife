#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "SelectKeyWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOverrideKeyDownEvent, FString, KeyName, int32, Num);

UCLASS()
class T_API USelectKeyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(BlueprintAssignable)
		FOnOverrideKeyDownEvent OnOverrideKeyDownEvent;

public:
	UPROPERTY(BlueprintReadOnly)
		int32 DownKeyNum = 0;
};
