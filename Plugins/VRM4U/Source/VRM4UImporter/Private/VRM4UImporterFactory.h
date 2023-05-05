// VRM4U Copyright (c) 2021-2022 Haruyoshi Yamamoto. This software is released under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "VRM4UImporterFactory.generated.h"

UCLASS()
class UVRM4UImporterFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory interface
	//virtual FText GetToolTip() const override;
	virtual bool FactoryCanImport(const FString& Filename) override;
	virtual UClass* ResolveSupportedClass() override;
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
	virtual UObject* FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	// End of UFactory interface

protected:

	FString fullFileName;
};
