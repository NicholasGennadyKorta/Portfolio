// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <FluidArchitectHexaSphereRuntime/Public/FluidArchitectHexaSphereProfile.h>
#include <FluidArchitectCoreEditor/Public/FluidArchitectCoreEditorProfile.h>
#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"
#include "FluidArchitectHexaSphereEditorAsset.generated.h"

//Fluid Architect HexaSphere Editor Asset Factory
UCLASS()
class UFluidArchitectHexaSphereEditorAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid Architect HexaSphere Editor Asset Type Actions
class FluidArchitectHexaSphereEditorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FluidArchitectHexaSphereEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
   
private:
	EAssetTypeCategories::Type MyAssetCategory;
};
