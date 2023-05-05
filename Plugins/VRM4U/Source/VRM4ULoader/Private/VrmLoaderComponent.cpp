// VRM4U Copyright (c) 2021-2022 Haruyoshi Yamamoto. This software is released under the MIT License.
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// ApplicationLifecycleComponent.cpp: Component to handle receiving notifications from the OS about application state (activated, suspended, termination, etc)


#include "VrmLoaderComponent.h"
#include "VrmAssetListObject.h"
#include "LoaderBPFunctionLibrary.h"

#include "Misc/CoreDelegates.h"


UVrmLoaderComponent::UVrmLoaderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UVrmLoaderComponent::OnRegister()
{
	Super::OnRegister();
}

void UVrmLoaderComponent::OnUnregister()
{
	Super::OnUnregister();
}

bool UVrmLoaderComponent::LoadVRMFile(const UVrmAssetListObject *InVrmAsset, UVrmAssetListObject *&OutVrmAsset, FString filepath) {
	return ULoaderBPFunctionLibrary::LoadVRMFileLocal(InVrmAsset, OutVrmAsset, filepath);
}

bool UVrmLoaderComponent::LoadVRMFileAsync(const UVrmAssetListObject *InVrmAsset, FString filepath) {

	ULoaderBPFunctionLibrary::LoadVRMFileLocal(InVrmAsset, AssetList, filepath);
	bResultQueue = true;
	return true;
}

void UVrmLoaderComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bResultQueue) {
		bResultQueue = false;
		OnFinishLoad.Broadcast(AssetList);
	}
}
