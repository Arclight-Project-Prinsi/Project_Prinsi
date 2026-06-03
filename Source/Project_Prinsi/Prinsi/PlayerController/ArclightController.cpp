// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/PlayerController/ArclightController.h"

#include "Kismet/GameplayStatics.h"						// UE多功能工具包
#include "EnhancedInputComponent.h"						// 输入增强组件
#include "Prinsi/GridManage/ArclightGridTileManager.h"	// 逻辑格子管理器
#include "Prinsi/GridManage/ArclightGridTile.h"			//[p]逻辑格子

#include "Prinsi/Entity/Tower/TowerBase.h"				//[p]塔基类


void AArclightController::BeginPlay() {
	Super::BeginPlay();

	// ~~绑定IMC至EnhancedInput子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(DefaultMappingContext_, 0);		// 从ULocalPlayer处找它的子系统，
	}

	// ~~[P]鼠标输入＆鼠标显示
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;	//[P]

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	//[P]
		SetInputMode(InputMode);
	}

	// ~~找到逻辑格子管理器指针(GridManager)
	if (!GridManager_) {
		GridManager_ = Cast<AArclightGridTileManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), AArclightGridTileManager::StaticClass())
		);
	}
}

void AArclightController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	switch (OperationMode_)
	{
	case EPlayerOperationMode::Normal:
		break;

	case EPlayerOperationMode::Placement:	// (Case)预览体&格子的部署合法性检查
		if (GridManager_) {
			GridManager_->UpdatePlacementPreview(this, CurrentPlacementFootprint_);
			UpdatePlacementPreviewActor();		//[p]更新预览体
		}
		break;

	default:
		break;
	}
}

void AArclightController::SetupInputComponent() {
	Super::SetupInputComponent();

	// ~~IA响应绑定
	// IMC触发IA，InputComponent监听IA，Controller响应IA（对应函数）
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		// 移动玩家角色
		if (IA_MovePlayer_) { EIC->BindAction(IA_MovePlayer_, ETriggerEvent::Triggered, this, &AArclightController::OnMovePlayer); }

		// 进入建造模式
		if (IA_EnterPlacementMode_) { EIC->BindAction(IA_EnterPlacementMode_, ETriggerEvent::Started, this, &AArclightController::OnEnterPlacementMode); }
	}
}

void AArclightController::OnEnterPlacementMode() {
	switch (OperationMode_)
	{
	case EPlayerOperationMode::Normal:		// 进入建造模式
		//[T]
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("进入建造模式。"));

		//[p]创建预览体
		CreatePlacementPreviewActor();

		OperationMode_ = EPlayerOperationMode::Placement;
		break;

	case EPlayerOperationMode::Placement:	// 退出建造模式
		//[T]
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("退出建造模式。"));

		//[p]销毁预览体
		DestroyPlacementPreviewActor();

		OperationMode_ = EPlayerOperationMode::Normal;

		if (GridManager_) { GridManager_->ClearAllHighlights(); }	// 关闭格子状态高亮显示



		break;

	default:
		break;
	}
}

void AArclightController::CreatePlacementPreviewActor() {
	if (CurrentPreviewTower_) { return; }
	if (!CurrentPlacementTowerClass_) { return; }

	UWorld* World = GetWorld();
	if (!World) { return; }

	CurrentPreviewTower_ = World->SpawnActor<ATowerBase>(CurrentPlacementTowerClass_, FVector::ZeroVector, FRotator::ZeroRotator);

	if (!CurrentPreviewTower_) { return; }

	CurrentPreviewTower_->SetActorEnableCollision(false);
	SetPreviewTowerVisible(false);
}

void AArclightController::DestroyPlacementPreviewActor() {
	if (CurrentPreviewTower_) {
		CurrentPreviewTower_->Destroy();		// Actor Destroy
		CurrentPreviewTower_ = nullptr;
	}
}

void AArclightController::UpdatePlacementPreviewActor() {
	if (!GridManager_ || !CurrentPreviewTower_) { return; }

	AArclightGridTile* CursorTile = nullptr;

	if (!GridManager_->GetTileUnderCursor(this, CursorTile)) {
		SetPreviewTowerVisible(false);
		return;
	}

	// (鼠标指针)锚点格
	const FIntPoint AnchorCoord = CursorTile->GetGridCoord();
	// 逻辑起点格
	const FIntPoint OriginCoord = GridManager_->CalcOriginCoordFromAnchor(AnchorCoord, CurrentPlacementFootprint_);
	// 计算所有占地格
	const TArray<FIntPoint> Coords = GridManager_->GetFootprintCoords(OriginCoord, CurrentPlacementFootprint_);

	const bool bCanPlace = GridManager_->CanPlaceAtCoords(Coords);


	//[p]
	//const FVector PreviewLocation = GridManager_->CalcFootprintCenterWorldLocation(
	//	OriginCoord,
	//	CurrentPlacementFootprint_
	//);
	//CurrentPreviewTower_->SetActorLocation(PreviewLocation);

	//[p]先做预览体显示测试
	CurrentPreviewTower_->SetActorLocation(
		CursorTile->GetActorLocation()
	);

	SetPreviewTowerVisible(true);

	SetPreviewTowerMaterial(
		bCanPlace ? BuildablePreviewMaterial_ : UnbuildablePreviewMaterial_
	);
}

void AArclightController::SetPreviewTowerVisible(bool bVisible) {
	if (!CurrentPreviewTower_) { return; }

	CurrentPreviewTower_->SetActorHiddenInGame(!bVisible);
}

void AArclightController::SetPreviewTowerMaterial(UMaterialInterface* Material) {
	if (!CurrentPreviewTower_ || !Material) { return; }

	//[p]第一个Mesh？将所有Mesh导入数组？
	TArray<UStaticMeshComponent*> MeshComponents;
	CurrentPreviewTower_->GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* MeshComp : MeshComponents) {
		if (!MeshComp) { continue; }

		const int32 MaterialNum = MeshComp->GetNumMaterials();
		for (int32 i = 0; i < MaterialNum; ++i) {
			MeshComp->SetMaterial(i, Material);
		}
	}
}

void AArclightController::OnMovePlayer(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();

	APawn* MyPawn = GetPawn();			// 获得Controller绑定的Pawn
	if (!MyPawn) return;

	// 位移处理
	MyPawn->AddMovementInput(FVector::ForwardVector, Input.Y);
	MyPawn->AddMovementInput(FVector::RightVector, Input.X);
}
