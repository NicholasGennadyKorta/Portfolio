// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidArchitectCoreEditor.h"
#include "PreviewScene.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/TransformNonVectorized.h"
#include "Materials/Material.h"
#include "GameFramework/Actor.h"
#include "GameFramework/WorldSettings.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "EditorViewportClient.h"
#include "LevelEditorViewport.h"
#include "EditorModeManager.h"

//Preview Scene
class FLUIDARCHITECTCOREEDITOR_API FFluidArchitectCorePreviewScene : public FPreviewScene
{
public:
	FFluidArchitectCorePreviewScene();
};

//Preview Viewport Client
class FLUIDARCHITECTCOREEDITOR_API FFluidArchitectCorePreviewViewportClient : public FEditorViewportClient
{
public:
	FFluidArchitectCorePreviewViewportClient(FEditorModeTools* ModeTools,FPreviewScene* PreviewScene);
	~FFluidArchitectCorePreviewViewportClient();
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(FViewport* InViewport, FCanvas* Canvas) override;

	class SFluidArchitectCorePreviewViewportWidget* ArchitectCorePreviewViewportWidget;
};

//Preview Viewport Widget
class FLUIDARCHITECTCOREEDITOR_API SFluidArchitectCorePreviewViewportWidget : public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SFluidArchitectCorePreviewViewportWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	SFluidArchitectCorePreviewViewportWidget();
	~SFluidArchitectCorePreviewViewportWidget();

	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;

	void CreatePreviewScene();
	
protected:
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

	FFluidArchitectCorePreviewViewportClient* ViewportClient;
	TSharedPtr<class FFluidArchitectCorePreviewScene> PreviewScene;

public:
	UFluidArchitectCoreProfile* Asset;
	FName ModuleID;
	FName ModuleName;	
};