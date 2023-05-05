// VRM4U Copyright (c) 2021-2022 Haruyoshi Yamamoto. This software is released under the MIT License.


#include "AnimNode_VrmConstraint.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#include "VrmMetaObject.h"
#include "VrmUtil.h"

#include <algorithm>
/////////////////////////////////////////////////////
// FAnimNode_ModifyBone


FAnimNode_VrmConstraint::FAnimNode_VrmConstraint() {
}

void FAnimNode_VrmConstraint::UpdateCache(FComponentSpacePoseContext& Output) {
}

void FAnimNode_VrmConstraint::Initialize_AnyThread(const FAnimationInitializeContext& Context) {
	Super::Initialize_AnyThread(Context);

}
void FAnimNode_VrmConstraint::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) {
	Super::CacheBones_AnyThread(Context);
}

void FAnimNode_VrmConstraint::UpdateInternal(const FAnimationUpdateContext& Context){
	Super::UpdateInternal(Context);
}


void FAnimNode_VrmConstraint::GatherDebugData(FNodeDebugData& DebugData)
{
	FString DebugLine = DebugData.GetNodeName(this);

	DebugLine += "(";
	AddDebugNodeData(DebugLine);
	//DebugLine += FString::Printf(TEXT(" Target: %s)"), *BoneToModify.BoneName.ToString());
	//DebugLine += FString::Printf(TEXT(" Target: %s)"), *BoneNameToModify.ToString());
	DebugData.AddDebugItem(DebugLine);

	ComponentPose.GatherDebugData(DebugData);
}

void FAnimNode_VrmConstraint::EvaluateComponentPose_AnyThread(FComponentSpacePoseContext& Output) {
	Super::EvaluateComponentPose_AnyThread(Output);
}

void FAnimNode_VrmConstraint::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	check(OutBoneTransforms.Num() == 0);

	UpdateCache(Output);

	if (VrmMetaObject == nullptr) {
		return;
	}

	for (auto& a : VrmMetaObject->VRMConstraintMeta) {

		int32 dstBoneIndex = Output.AnimInstanceProxy->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex(*(a.Key));
		FCompactPoseBoneIndex dstPoseBoneIndex(dstBoneIndex);
		auto dstRefTrans = Output.Pose.GetComponentSpaceTransform(dstPoseBoneIndex);

		int32 srcBoneIndex = -1;

		switch (a.Value.type) {
			case EVRMConstraintType::None:
			break;
			case EVRMConstraintType::Roll:
			{
				auto& tmp = a.Value.constraintRoll;
				srcBoneIndex = Output.AnimInstanceProxy->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex(*(tmp.sourceName));
			}
			break;
			case EVRMConstraintType::Aim :
			{
				auto& tmp = a.Value.constraintRotation;
				srcBoneIndex = Output.AnimInstanceProxy->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex(*(tmp.sourceName));
			}
			break;
			case EVRMConstraintType::Rotation :
			{
				auto& tmp = a.Value.constraintRotation;
				srcBoneIndex = Output.AnimInstanceProxy->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex(*(tmp.sourceName));
			}
			break;
		}

		if (srcBoneIndex < 0) {
			continue;
		}
		FCompactPoseBoneIndex srcPoseBoneIndex(srcBoneIndex);
		auto srcRefTrans = Output.AnimInstanceProxy->GetSkeleton()->GetReferenceSkeleton().GetRefBonePose()[srcBoneIndex];

		if (a.Value.type == EVRMConstraintType::Rotation) {
			auto& rot = a.Value.constraintRotation;

			auto st = Output.Pose.GetComponentSpaceTransform(srcPoseBoneIndex);
			auto dt = Output.Pose.GetComponentSpaceTransform(dstPoseBoneIndex);

			//rot.weight

			dt.SetRotation(st.GetRotation());
			Output.Pose.SetComponentSpaceTransform(dstPoseBoneIndex, dt);
		}
	}
}

bool FAnimNode_VrmConstraint::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) 
{
	return true;
}

void FAnimNode_VrmConstraint::InitializeBoneReferences(const FBoneContainer& RequiredBones) 
{
	//BoneToModify.Initialize(RequiredBones);
}

void FAnimNode_VrmConstraint::ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp, bool bPreviewForeground) const
{
#if WITH_EDITOR

	if (VrmMetaObject == nullptr || PreviewSkelMeshComp == nullptr) {
		return;
	}
	if (PreviewSkelMeshComp->GetWorld() == nullptr) {
		return;
	}
#endif
}
