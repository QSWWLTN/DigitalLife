// VRM4U Copyright (c) 2021-2022 Haruyoshi Yamamoto. This software is released under the MIT License.


#include "VrmAssetListObject.h"
//#include "LoaderBPFunctionLibrary.h"



UVrmAssetListObject::UVrmAssetListObject(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Package = GetTransientPackage();

	//Result = new FReturnedData();
}


void UVrmAssetListObject::CopyMember(UVrmAssetListObject *dst) const {
	dst->bAssetSave = bAssetSave;
	dst->bSkipMorphTarget = bSkipMorphTarget;

	dst->MtoonLitSet = MtoonLitSet;
	dst->MtoonUnlitSet = MtoonUnlitSet;
	dst->SSSSet = SSSSet;
	dst->SSSProfileSet = SSSProfileSet;
	dst->UnlitSet = UnlitSet;
	dst->GLTFSet = GLTFSet;
	dst->CustomSet = CustomSet;


	dst->BaseMToonLitOpaqueMaterial = BaseMToonLitOpaqueMaterial;
	dst->BaseMToonLitTranslucentMaterial = BaseMToonLitTranslucentMaterial;
	dst->OptMToonLitOpaqueMaterial = OptMToonLitOpaqueMaterial;
	dst->OptMToonLitOpaqueTwoSidedMaterial = OptMToonLitOpaqueTwoSidedMaterial;
	dst->OptMToonLitTranslucentMaterial = OptMToonLitTranslucentMaterial;
	dst->OptMToonLitTranslucentTwoSidedMaterial = OptMToonLitTranslucentTwoSidedMaterial;

	dst->BaseMToonUnlitOpaqueMaterial = BaseMToonUnlitOpaqueMaterial;
	dst->BaseMToonUnlitTranslucentMaterial = BaseMToonUnlitTranslucentMaterial;
	dst->OptMToonUnlitOpaqueMaterial = OptMToonUnlitOpaqueMaterial;
	dst->OptMToonUnlitOpaqueTwoSidedMaterial = OptMToonUnlitOpaqueTwoSidedMaterial;
	dst->OptMToonUnlitTranslucentMaterial = OptMToonUnlitTranslucentMaterial;
	dst->OptMToonUnlitTranslucentTwoSidedMaterial = OptMToonUnlitTranslucentTwoSidedMaterial;

	dst->OptMToonOutlineMaterial = OptMToonOutlineMaterial;

	dst->BaseUnlitOpaqueMaterial			= BaseUnlitOpaqueMaterial;
	dst->BaseUnlitTranslucentMaterial = BaseUnlitTranslucentMaterial;
	dst->BasePBROpaqueMaterial = BasePBROpaqueMaterial;


	dst->BaseSkeletalMesh = BaseSkeletalMesh;
	dst->VrmMetaObject = VrmMetaObject;
	dst->VrmLicenseObject = VrmLicenseObject;
	dst->SkeletalMesh = SkeletalMesh;
	dst->Textures = Textures;
	dst->Materials = Materials;
	dst->HumanoidRig = HumanoidRig;
	dst->Package = Package;
	dst->FileFullPathName = FileFullPathName;
	dst->OrigFileName = OrigFileName;
	dst->BaseFileName = BaseFileName;
	dst->HumanoidSkeletalMesh = HumanoidSkeletalMesh;
}
