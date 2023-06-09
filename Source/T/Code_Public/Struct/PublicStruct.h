#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "PublicStruct.generated.h"

USTRUCT(BlueprintType)
struct FConfig : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ServerIP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ServerPort;
};

USTRUCT(BlueprintType)
struct FHotKeyStruct{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 KeyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharCode;

public:
	friend bool operator==(const FHotKeyStruct& A, const FHotKeyStruct& B) {
		return A.KeyCode == B.KeyCode && A.CharCode == B.CharCode;
	}
};

USTRUCT(BlueprintType)
struct FHotKeyList {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FHotKeyStruct> KeyList;
};