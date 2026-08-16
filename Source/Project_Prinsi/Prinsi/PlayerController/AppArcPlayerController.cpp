// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/PlayerController/AppArcPlayerController.h"
// Prinsi Entity
#include "Prinsi/Entity/Tower/AppTowerBase.h"					// 塔基类
// Component
#include "Components/SkeletalMeshComponent.h"					
// Misc
#include "Prinsi/Define/AppDefineDebug.h"						// Debug用文件
#include "Kismet/GameplayStatics.h"								// UE多功能工具包
#include "EnhancedInputComponent.h"								// 输入增强组件
#include "Prinsi/AppSystem/GirdManager/AppGridTileManager.h"	// 逻辑格子管理器
#include "Prinsi/AppSystem/GirdManager/AppGridTile.h"			// 逻辑格子


void AAppArcPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 绑定IMC至EnhancedInput子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);		// 从ULocalPlayer处找它的子系统，
	}

	//  @note 鼠标输入＆鼠标显示
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;	// @todo

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	// @todo
		SetInputMode(InputMode);
	}

	// 找到逻辑格子管理器指针(GridManager)
	if (!GridManager)
	{
		GridManager = Cast<AAppGridTileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AAppGridTileManager::StaticClass()));
	}
}

void AAppArcPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (OperationMode)
	{
	case EOperationMode::Normal:
	{
		break;
	}

	case EOperationMode::Placement:		// 【Case】预览体位置更新 & 格子的部署合法性检查
	{
		if (GridManager)
		{
			GridManager->UpdatePlacementPreview(this, CurrentPlacementFootprint);
			UpdatePlacementPreviewActor();		// @todo 更新预览体
		}
		break;
	}

	default:
		break;
	}
}

void AAppArcPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// IA响应绑定
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 移动玩家角色
		if (IA_MovePlayer)
		{
			EIC->BindAction(IA_MovePlayer, ETriggerEvent::Triggered, this, &AAppArcPlayerController::OnMovePlayer);
		}

		// 进入建造模式
		if (IA_EnterPlacementMode)
		{
			EIC->BindAction(IA_EnterPlacementMode, ETriggerEvent::Started, this, &AAppArcPlayerController::OnEnterPlacementMode);
		}

		// @todo 目前仅部署Tower
		// 确认部署
		if (IA_ConfirmPlacement)
		{
			EIC->BindAction(IA_ConfirmPlacement, ETriggerEvent::Started, this, &AAppArcPlayerController::OnConfirmPlacement);
		}
	}
}

void AAppArcPlayerController::OnEnterPlacementMode()
{
	switch (OperationMode)
	{
	case EOperationMode::Normal:		// Case_进入建造模式
	{
		CreatePlacementPreviewActor();	// 创建预览体
		OperationMode = EOperationMode::Placement;
		break;
	}

	case EOperationMode::Placement:		// Case_退出建造模式
	{
		DestroyPlacementPreviewActor();	// 销毁预览体
		OperationMode = EOperationMode::Normal;

		if (GridManager)
		{
			GridManager->ClearAllHighlights();	// 关闭格子状态高亮显示
		}
		break;
	}

	default:
		break;
	}
}

void AAppArcPlayerController::OnMovePlayer(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	APawn* MyPawn = GetPawn();			// 获得Controller绑定的Pawn
	if (!MyPawn)
	{
		return;
	}

	// 位移处理
	MyPawn->AddMovementInput(FVector::ForwardVector, Input.Y);
	MyPawn->AddMovementInput(FVector::RightVector, Input.X);
}

void AAppArcPlayerController::OnConfirmPlacement()
{
	if (OperationMode != EOperationMode::Placement)
	{
		return;
	}

	// @todo 目前仅部署Tower
	// 尝试部署Tower
	TryPlaceCurrentPreview();
}

/**
* @brief	创建预览体 & 设置占地尺寸（Footprint）
*/
void AAppArcPlayerController::CreatePlacementPreviewActor()
{
	if (IsValid(CurrentPreviewTower))
	{
		return;
	}

	if (!CurrentPlacementTowerClass)
	{
		return;
	}

	// 生成预览体 & 获取占地数据
	// @todo 预览体使用默认旋转?
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	CurrentPreviewTower = World->SpawnActor<AAppTowerBase>(CurrentPlacementTowerClass, FVector::ZeroVector, FRotator::ZeroRotator);

	if (!CurrentPreviewTower)
	{
		return;
	}
	CurrentPreviewTower->InitialTower();	// Entity初始化（占地尺寸等）
	CurrentPreviewTower->SetActorEnableCollision(false);
	SetPreviewTowerVisible(false);

	CurrentPlacementFootprint = CurrentPreviewTower->GetFootprintSize();	// 占地尺寸
}

/**
* @brief	销毁预览体
*/
void AAppArcPlayerController::DestroyPlacementPreviewActor()
{
	if (IsValid(CurrentPreviewTower))
	{
		CurrentPreviewTower->Destroy();
		CurrentPreviewTower = nullptr;
	}
}

/**
* @brief	更新预览体状态与方位
*/
void AAppArcPlayerController::UpdatePlacementPreviewActor()
{
	if (!IsValid(CurrentPreviewTower))
	{
		return;
	}

	// 获取当前Placement(部署情况)数据
	FIntPoint OriginCoord;
	TArray<FIntPoint> Coords;
	FVector PlacementLocation;
	bool bCanPlace = false;
	if (!GetCurrentPlacementData(OriginCoord, Coords, PlacementLocation, bCanPlace))
	{
		SetPreviewTowerVisible(false);		// 设置预览体可视性（不可视）
		return;
	}

	// @todo 有没有更平滑的预览体移动方式?
	// 设置预览体位置（占地实际坐标）
	CurrentPreviewTower->SetActorLocation(PlacementLocation);

	SetPreviewTowerVisible(true);			// 设置预览体可视性（可视）
	SetPreviewTowerMaterial(bCanPlace ? BuildablePreviewMaterial : UnbuildablePreviewMaterial);
}

/**
* @brief	设置预览体可视性
*/
void AAppArcPlayerController::SetPreviewTowerVisible(bool bVisible)
{
	if (!CurrentPreviewTower)
	{
		return;
	}

	CurrentPreviewTower->SetActorHiddenInGame(!bVisible);
}

/**
* @brief	设置预览体材质
*/
void AAppArcPlayerController::SetPreviewTowerMaterial(UMaterialInterface* Material)
{
	if (!CurrentPreviewTower || !Material)
	{
		return;
	}

	//@todo Tower是否统一为为骨骼模型?
	//TArray<UStaticMeshComponent*> MeshComponents;
	TArray<USkeletalMeshComponent*> SkeletonComps;
	CurrentPreviewTower->GetComponents<USkeletalMeshComponent>(SkeletonComps);
	for (USkeletalMeshComponent* MeshComp : SkeletonComps)
	{
		if (!MeshComp)
		{
			continue;
		}

		// @memo GetNumMaterials 保证可以覆盖到所有的材质
		const int32 MaterialNum = MeshComp->GetNumMaterials();
		for (int32 i = 0; i < MaterialNum; ++i)
		{
			MeshComp->SetMaterial(i, Material);
		}
	}
}

/**
* @brief	正式部署Tower
*/
bool AAppArcPlayerController::TryPlaceCurrentPreview()
{
	if (OperationMode != EOperationMode::Placement)
	{
		return false;
	}

	if (!IsValid(GridManager) || !CurrentPlacementTowerClass)
	{
		return false;
	}

	// 获取Placement数据
	FIntPoint OriginCoord;
	TArray<FIntPoint> Coords;
	FVector PlacementLocation;
	bool bCanPlace = false;
	if (!GetCurrentPlacementData(OriginCoord, Coords, PlacementLocation, bCanPlace))
	{
		return false;
	}
	if (!bCanPlace)
	{
		return false;
	}

	// 生成Tower对象
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//@todo Tower的Owner应该为AC吗?
	AAppTowerBase* NewTower = World->SpawnActor<AAppTowerBase>
		(CurrentPlacementTowerClass, PlacementLocation, FRotator::ZeroRotator, SpawnParams);
	if (!NewTower)
	{
		return false;
	}

	if (!NewTower->InitialTower())
	{
		return false;
	}

	// 逻辑坐标绑定
	if (!GridManager->OccupyCoords(Coords, NewTower))
	{
		NewTower->Destroy();
		// @thumb
		APP_SCR_ERROR(TEXT("【Error】预览体逻辑坐标绑定失败!"));
		return false;
	}

	return true;
}

/**
* @brief	获取当前部署情况数据（参考当前的Footprint）
*/
bool AAppArcPlayerController::GetCurrentPlacementData(FIntPoint& OutOriginCoord, TArray<FIntPoint>& OutCoords, FVector& OutPlacementLocation, bool& bOutCanPlace)
{
	// 初始化要输出的数据结构
	OutOriginCoord = FIntPoint::ZeroValue;
	OutCoords.Empty();
	OutPlacementLocation = FVector::ZeroVector;
	bOutCanPlace = false;

	if (!IsValid(GridManager))
	{
		return false;
	}

	if (CurrentPlacementFootprint.X <= 0 || CurrentPlacementFootprint.Y <= 0)
	{
		return false;
	}

	// 进行可部署检测(独立于预览体更新)
	AAppGridTile* CursorTile = nullptr;
	if (!GridManager->GetTileUnderCursor(this, CursorTile))
	{
		return false;
	}

	// 锚点逻辑坐标
	const FIntPoint AnchorCoord = CursorTile->GetGridCoord();

	// Out_起点逻辑坐标
	OutOriginCoord = GridManager->CalcOriginCoordFromAnchor(AnchorCoord, CurrentPlacementFootprint);
	// Out_所有占地逻辑格子
	OutCoords = GridManager->GetFootprintCoords(OutOriginCoord, CurrentPlacementFootprint);
	// Out_检测占地逻辑格子是否合法
	// @memo 预览体当然存在“可视但不可部署”的情况。
	bOutCanPlace = GridManager->CanPlaceAtCoords(OutCoords);
	// Out_占地实际坐标
	OutPlacementLocation = GridManager->CalcFootprintCenterWorldLocation(OutOriginCoord, CurrentPlacementFootprint);

	return true;
}



