// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/PlayerController/AppArcPlayerController.h"

#include "Kismet/GameplayStatics.h"						// Tool_UE多功能工具包
#include "EnhancedInputComponent.h"						//[p]输入增强组件
#include "Prinsi/GridManage/ArclightGridTileManager.h"	// Manager_逻辑格子管理器
#include "Prinsi/GridManage/ArclightGridTile.h"			//[p]逻辑格子
#include "Prinsi/Entity/Tower/AppTowerBase.h"				//[p]塔基类


void AAppArcPlayerController::BeginPlay() {
	Super::BeginPlay();

	// ~~绑定IMC至EnhancedInput子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(DefaultMappingContext, 0);		// 从ULocalPlayer处找它的子系统，
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
	if (!GridManager) {
		GridManager = Cast<AArclightGridTileManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), AArclightGridTileManager::StaticClass())
		);
	}
}

void AAppArcPlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	switch (OperationMode)
	{
	case EOperationMode::Normal:
		break;

	case EOperationMode::Placement:	// (Case)预览体&格子的部署合法性检查
		if (GridManager) {
			GridManager->UpdatePlacementPreview(this, CurrentPlacementFootprint);
			UpdatePlacementPreviewActor();		//[p]更新预览体
		}
		break;

	default:
		break;
	}
}

void AAppArcPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// ~~IA响应绑定
	// IMC触发IA，InputComponent监听IA，Controller响应IA（对应函数）
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		// 移动玩家角色
		if (IA_MovePlayer) { EIC->BindAction(IA_MovePlayer, ETriggerEvent::Triggered, this, &AAppArcPlayerController::OnMovePlayer); }

		// 进入建造模式
		if (IA_EnterPlacementMode) { EIC->BindAction(IA_EnterPlacementMode, ETriggerEvent::Started, this, &AAppArcPlayerController::OnEnterPlacementMode); }
	}
}

void AAppArcPlayerController::OnEnterPlacementMode() {
	switch (OperationMode)
	{
	case EOperationMode::Normal:		// 进入建造模式
		//[T]
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("进入建造模式。"));

		//[p]创建预览体
		CreatePlacementPreviewActor();

		OperationMode = EOperationMode::Placement;
		break;

	case EOperationMode::Placement:	// 退出建造模式
		//[T]
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("退出建造模式。"));

		//[p]销毁预览体
		DestroyPlacementPreviewActor();

		OperationMode = EOperationMode::Normal;

		if (GridManager) { GridManager->ClearAllHighlights(); }	// 关闭格子状态高亮显示

		break;

	default:
		break;
	}
}

void AAppArcPlayerController::CreatePlacementPreviewActor() {
	if (CurrentPreviewTower) { return; }
	if (!CurrentPlacementTowerClass) { return; }

	// ~~生成预览体&获取占地数据
	UWorld* World = GetWorld();
	if (!World) { return; }
	CurrentPreviewTower = World->SpawnActor<AAppTowerBase>(CurrentPlacementTowerClass, FVector::ZeroVector, FRotator::ZeroRotator);

	if (!CurrentPreviewTower) { return; }
	CurrentPreviewTower->InitialTower();		//[p]初始化
	CurrentPreviewTower->SetActorEnableCollision(false);
	SetPreviewTowerVisible(false);

	CurrentPlacementFootprint = CurrentPreviewTower->GetFootprintSize();	// 占地
}

void AAppArcPlayerController::DestroyPlacementPreviewActor() {
	if (CurrentPreviewTower) {
		CurrentPreviewTower->Destroy();		// Actor Destroy
		CurrentPreviewTower = nullptr;
	}
}

void AAppArcPlayerController::UpdatePlacementPreviewActor() {
	if (!GridManager || !CurrentPreviewTower) { return; }

	AArclightGridTile* CursorTile = nullptr;

	if (!GridManager->GetTileUnderCursor(this, CursorTile)) {
		SetPreviewTowerVisible(false);
		return;
	}

	// (鼠标指针)锚点格
	const FIntPoint AnchorCoord = CursorTile->GetGridCoord();
	// 逻辑起点格
	const FIntPoint OriginCoord = GridManager->CalcOriginCoordFromAnchor(AnchorCoord, CurrentPlacementFootprint);
	// 计算所有占地格
	const TArray<FIntPoint> Coords = GridManager->GetFootprintCoords(OriginCoord, CurrentPlacementFootprint);

	const bool bCanPlace = GridManager->CanPlaceAtCoords(Coords);


	//[p]
	//const FVector PreviewLocation = GridManager->CalcFootprintCenterWorldLocation(
	//	OriginCoord,
	//	CurrentPlacementFootprint
	//);
	//CurrentPreviewTower->SetActorLocation(PreviewLocation);


	//[p]先做预览体显示测试
	CurrentPreviewTower->SetActorLocation(CursorTile->GetActorLocation());

	SetPreviewTowerVisible(true);

	SetPreviewTowerMaterial(
		bCanPlace ? BuildablePreviewMaterial : UnbuildablePreviewMaterial
	);
}

void AAppArcPlayerController::SetPreviewTowerVisible(bool bVisible) {
	if (!CurrentPreviewTower) { return; }

	CurrentPreviewTower->SetActorHiddenInGame(!bVisible);
}

void AAppArcPlayerController::SetPreviewTowerMaterial(UMaterialInterface* Material) {
	if (!CurrentPreviewTower || !Material) { return; }

	//[p]第一个Mesh？将所有Mesh导入数组？
	TArray<UStaticMeshComponent*> MeshComponents;
	CurrentPreviewTower->GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* MeshComp : MeshComponents) {
		if (!MeshComp) { continue; }

		const int32 MaterialNum = MeshComp->GetNumMaterials();
		for (int32 i = 0; i < MaterialNum; ++i) {
			MeshComp->SetMaterial(i, Material);
		}
	}
}

void AAppArcPlayerController::OnMovePlayer(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();

	APawn* MyPawn = GetPawn();			// 获得Controller绑定的Pawn
	if (!MyPawn) return;

	// 位移处理
	MyPawn->AddMovementInput(FVector::ForwardVector, Input.Y);
	MyPawn->AddMovementInput(FVector::RightVector, Input.X);
}