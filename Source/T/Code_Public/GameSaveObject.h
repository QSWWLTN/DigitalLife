#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Struct/PublicStruct.h"

#include "GameSaveObject.generated.h"

UCLASS()
class T_API UGameSaveObject : public USaveGame
{
	GENERATED_BODY()
public:
	UGameSaveObject();

public:
	UPROPERTY()
		TArray<uint8> Config;
	UPROPERTY(BlueprintReadWrite)
		float FOVValue = 0.1f;
	UPROPERTY(BlueprintReadWrite)
		FConfig IPConfig;
	UPROPERTY(BlueprintReadWrite)
		FHotKeyList KeyList;
	UPROPERTY(BlueprintReadWrite)
		FIntPoint WindowPoint = FIntPoint(-1, -1);

	UPROPERTY(BlueprintReadWrite)
		int32 QualityIndex = 2;
};
