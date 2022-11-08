// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidArchitectCorePreviewScene.h"
#include "DrawDebugHelpers.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fluid Architect Core Preview Scene

FFluidArchitectCorePreviewScene::FFluidArchitectCorePreviewScene():FPreviewScene()
{
	// world setting
	GetWorld()->GetWorldSettings()->NotifyBeginPlay();
	GetWorld()->GetWorldSettings()->NotifyMatchStarted();
	GetWorld()->GetWorldSettings()->SetActorHiddenInGame(false);
	GetWorld()->bBegunPlay = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fluid Architect Core Preview Viewport Client

FFluidArchitectCorePreviewViewportClient::FFluidArchitectCorePreviewViewportClient(FEditorModeTools* InModeTools,FPreviewScene* InPreviewScene):FEditorViewportClient(InModeTools,InPreviewScene)
{
	FEditorViewportClient(InModeTools,InPreviewScene);
}

FFluidArchitectCorePreviewViewportClient::~FFluidArchitectCorePreviewViewportClient()
{
	ArchitectCorePreviewViewportWidget = nullptr;
}

void FFluidArchitectCorePreviewViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);
	PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	
	if (ArchitectCorePreviewViewportWidget == nullptr)
		return;
	
	auto Asset = ArchitectCorePreviewViewportWidget->Asset;
	if (Asset == nullptr)
		return;
	
	for (int i = 0; i < Asset->Modules.Num(); i++)
		if (Asset->Modules[i].UID == ArchitectCorePreviewViewportWidget->ModuleID)
			Asset->OnPreviewViewport(ArchitectCorePreviewViewportWidget->ModuleID,PreviewScene->GetWorld());
}

void FFluidArchitectCorePreviewViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	FEditorViewportClient::Draw(InViewport,Canvas);
	SetRealtime(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fluid Architect Core Preview Viewport Widget

void SFluidArchitectCorePreviewViewportWidget::Construct(const FArguments& InArgs)
{
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

SFluidArchitectCorePreviewViewportWidget::SFluidArchitectCorePreviewViewportWidget()
{
}

SFluidArchitectCorePreviewViewportWidget::~SFluidArchitectCorePreviewViewportWidget()
{
	ViewportClient = nullptr;
}

TSharedRef<class SEditorViewport> SFluidArchitectCorePreviewViewportWidget::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SFluidArchitectCorePreviewViewportWidget::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SFluidArchitectCorePreviewViewportWidget::OnFloatingButtonClicked()
{
}

TSharedRef<FEditorViewportClient> SFluidArchitectCorePreviewViewportWidget::MakeEditorViewportClient()
{
	PreviewScene = MakeShareable(new FFluidArchitectCorePreviewScene());
	ViewportClient = new FFluidArchitectCorePreviewViewportClient(nullptr, PreviewScene.Get());
	ViewportClient->ArchitectCorePreviewViewportWidget = this;
	return (MakeShareable(ViewportClient));
}

TSharedPtr<SWidget> SFluidArchitectCorePreviewViewportWidget::MakeViewportToolbar()
{
	return SNew(SCommonEditorViewportToolbarBase, SharedThis(this));
}
