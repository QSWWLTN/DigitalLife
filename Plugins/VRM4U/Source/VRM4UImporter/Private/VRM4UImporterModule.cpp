
#include "CoreMinimal.h"
#include "VRM4UImporterLog.h"
#include "VRM4UDetailCustomize.h"
#include "VrmAssetListThumbnailRenderer.h"
#include "VrmAssetListObject.h"
#include "VrmLicenseObject.h"
#include "VrmMetaObject.h"
#include "VrmCustomStruct.h"

#include "AssetToolsModule.h"
#include "Modules/ModuleManager.h"
#include "Internationalization/Internationalization.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"



#define LOCTEXT_NAMESPACE "VRM4UImporter"

DEFINE_LOG_CATEGORY(LogVRM4UImporter);

//////////////////////////////////////////////////////////////////////////
// FSpriterImporterModule

class FVRM4UImporterModule : public FDefaultModuleImpl
{
public:
	virtual void StartupModule() override
	{
		{
			auto &a = UThumbnailManager::Get();
			a.RegisterCustomRenderer(UVrmAssetListObject::StaticClass(), UVrmAssetListThumbnailRenderer::StaticClass());
			a.RegisterCustomRenderer(UVrmLicenseObject::StaticClass(), UVrmAssetListThumbnailRenderer::StaticClass());
			a.RegisterCustomRenderer(UVrmMetaObject::StaticClass(), UVrmAssetListThumbnailRenderer::StaticClass());
		}

		{
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_VrmAssetList));
			AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_VrmLicense));
			AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_VrmMeta));
		}

		{
			//FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
			//LevelEditor.OnActorSelectionChanged().RemoveAll(this);
		}

		{
			FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

			PropertyEditorModule.RegisterCustomPropertyTypeLayout(
				("VRMRetargetSrcAnimSequence"),
				FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FVRMRetargetSrcAnimSequenceCustomization::MakeInstance)
			);

			PropertyEditorModule.NotifyCustomizationModuleChanged();
		}
	}

	virtual void ShutdownModule() override
	{
		if (UObjectInitialized()){
			UThumbnailManager::Get().UnregisterCustomRenderer(UVrmAssetListObject::StaticClass());
			UThumbnailManager::Get().UnregisterCustomRenderer(UVrmLicenseObject::StaticClass());
			UThumbnailManager::Get().UnregisterCustomRenderer(UVrmMetaObject::StaticClass());

			FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyEditorModule.UnregisterCustomPropertyTypeLayout(("VRMRetargetSrcAnimSequence"));
		}
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FVRM4UImporterModule, VRM4UImporter);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
