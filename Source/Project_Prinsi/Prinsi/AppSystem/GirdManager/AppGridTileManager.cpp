// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AppSystem/GirdManager/AppGridTileManager.h"
#include "Prinsi/Define/AppDefineDebug.h"				// Define_Debug工具
#include "Prinsi/AppSystem/GirdManager/AppGridTile.h"	// Actor_逻辑格子、
// Misc
#include "EngineUtils.h"


// Sets default values
AAppGridTileManager::AAppGridTileManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAppGridTileManager::BeginPlay()
{
	Super::BeginPlay();

	// 重构GridMap
	RebuildGridMap();

	// @todo 编辑器中如果想隐藏Grid
	if (!bGridVisible)
	{
		bGridVisible = true;
		SetAllGridVisible(bGridVisible);
	}
}

// Called every frame
void AAppGridTileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
*	@brief　根据当前鼠标指针对格子（Grid）进行射线检测
*	@note	ECC_GameTraceChannel1_在Project Setting中定义的逻辑格子专用检测响应通道
*/
bool AAppGridTileManager::GetTileUnderCursor(APlayerController* PlayerController, AAppGridTile*& OutTile) const
{
	OutTile = nullptr;

	// 从当前的控制器的鼠标指针处进行射线检测
	if (!PlayerController)
	{
		return false;
	}

	FHitResult HitResult;
	const bool bHit = PlayerController->GetHitResultUnderCursor(
		ECC_GameTraceChannel1,
		false,
		HitResult
	);

	if (!bHit)
	{
		return false;
	}

	OutTile = Cast<AAppGridTile>(HitResult.GetActor());
	return OutTile != nullptr;
}

/**
*	@brief　清空当前Map中的Grid并销毁对象
*/
void AAppGridTileManager::ClearAllGrid()
{
	ClearAllHighlights();

	for (auto& Pair : GridMap)
	{
		AAppGridTile* Tile = Pair.Value;
		if (Tile)
		{
			Tile->Destroy();
		}
	}

	GridMap.Empty();
}

/**
*	@brief　生成初始化Grid并加入GridMap中
*	@note	先清空GridMap中的格子
*/
void AAppGridTileManager::GenerateAllGrid()
{
	ClearAllGrid();

	if (!TileClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			const FIntPoint Coord(X, Y);
			const FVector SpawnLocation = GridCoordToWorldLocation(Coord);
			const FTransform SpawnTransform(SpawnLocation);

			// 生成单位格子
			AAppGridTile* NewTile = World->SpawnActor<AAppGridTile>(TileClass, SpawnTransform);
			if (!NewTile)
			{
				// @thumb
				APP_ERROR(TEXT("【ERROR】格子生成失败_SpawnActor。"));
				continue;
			}

			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			// 对象初始化	
			// @todo 暂时全部生成为平地
			NewTile->InitTile(Coord, EGridTileType::Ground);

#if !UE_BUILD_SHIPPING
			// 为生成的逻辑格子编号方便确认
			NewTile->SetActorLabel(FString::Printf(TEXT("GridTile_%d_%d"), X, Y));
#endif
			// 加入Map管理
			GridMap.Add(Coord, NewTile);
		}
	}
}

/**
*	@brief　遍历World中的Grid对象并登录到Map中
*	@note	BeginPlay调用
*/
void AAppGridTileManager::RebuildGridMap()
{
	GridMap.Empty();
	// @todo 便利World中的逻辑格子是否合理?
	for (TActorIterator<AAppGridTile> It(GetWorld()); It; ++It)
	{
		AAppGridTile* Tile = *It;
		if (!IsValid(Tile))
		{
			continue;
		}

		const FIntPoint Coord = Tile->GetGridCoord();
		if (GridMap.Contains(Coord))
		{
			//@thumb
			APP_ERROR(TEXT("发现重复格子坐标：(%d, %d)"), Coord.X, Coord.Y);
			continue;
		}

		GridMap.Add(Coord, Tile);
	}
}

/**
*	@brief　切换Gird可视性
*	@note	Debug功能
*/
void AAppGridTileManager::SetAllGridVisible(bool bVisible)
{
	for (auto& Pair : GridMap)
	{
		if (AAppGridTile* Tile = Pair.Value)
		{
			Tile->SetGridVisible(bVisible);
		}
	}
}

/**
*	@brief　生成初始化Grid并加入Map中
*	@note	EUF调用接口
*/
void AAppGridTileManager::EUFGenerateGrid()
{
#if WITH_EDITOR
	ClearAllGrid();
	GenerateAllGrid();
	// @memo MarkPackageDirty_关卡已发生改变。
	MarkPackageDirty();
#endif
}

/**
*	@brief　清空当前Map中的Grid并销毁对象
*	@note	EUF调用接口
*/
void AAppGridTileManager::EUFClearGrid()
{
#if WITH_EDITOR
	ClearAllGrid();
	MarkPackageDirty();
#endif
}

/**
*	@brief　寻找Attach的Grid对象并登录到Map中
*	@note	EUF调用接口
*/
void AAppGridTileManager::EUFRebuildGridMap()
{
#if WITH_EDITOR
	RebuildGridMap();
	MarkPackageDirty();
#endif
}

/**
*	@brief　切换Gird可视性
*	@note	EUF调用接口
*	@note	Debug功能
*/
void AAppGridTileManager::EUFToggleAllGridVisible()
{
#if WITH_EDITOR
	bGridVisible = !bGridVisible;
	SetAllGridVisible(bGridVisible);
	MarkPackageDirty();
#endif
}

/**
*	@brief　确认逻辑坐标是否合法
*/
bool AAppGridTileManager::IsValidCoord(const FIntPoint& Coord) const
{
	return Coord.X >= 0
		&& Coord.Y >= 0
		&& Coord.X < GridWidth
		&& Coord.Y < GridHeight;
}

/**
*	@brief　通过逻辑坐标，获得逻辑格子
*/
AAppGridTile* AAppGridTileManager::GetTileByCoord(const FIntPoint& Coord) const
{
	if (AAppGridTile* const* FoundTile = GridMap.Find(Coord))
	{
		return *FoundTile;
	}

	return nullptr;
}

/**
*	@brief　通过逻辑坐标，获得实际坐标
*	@note	GridManager的坐标应为0号格子的坐标
*/
FVector AAppGridTileManager::GridCoordToWorldLocation(const FIntPoint& Coord) const
{
	const FVector Origin = GetActorLocation();

	return Origin + FVector(
		Coord.X * TileSize,
		Coord.Y * TileSize,
		0.0f
	);
}

/**
*	@brief　通过实际坐标，获得逻辑坐标
*/
FIntPoint AAppGridTileManager::WorldLocationToGridCoord(const FVector& WorldLocation) const
{
	// 先本地坐标化
	const FVector LocalLocation = WorldLocation - GetActorLocation();

	// @memo FloorToInt_向下取整
	const int32 X = FMath::FloorToInt(LocalLocation.X / TileSize);
	const int32 Y = FMath::FloorToInt(LocalLocation.Y / TileSize);

	return FIntPoint(X, Y);
}

/**
*	@brief　通逻辑坐标（起点），获得实际坐标（占地对象）
*/
FVector AAppGridTileManager::CalcFootprintCenterWorldLocation(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const
{
	// 获取起点逻辑坐标格子的实际坐标
	const FVector OriginTileWorldLocation = GridCoordToWorldLocation(OriginCoord);

	if (FootprintSize.X <= 0 || FootprintSize.X <= 0)
	{
		// @thumb
		APP_WARNING(TEXT("【ERROR】FootprintSize不合法!"));
		return OriginTileWorldLocation;
	}

	const float OffsetX = static_cast<float>(FootprintSize.X - 1) * TileSize * 0.5f;
	const float OffsetY = static_cast<float>(FootprintSize.Y - 1) * TileSize * 0.5f;

	return OriginTileWorldLocation + FVector(OffsetX, OffsetY, 0.0f);
}

/**
*	@brief　通过逻辑坐标（锚点），算出逻辑坐标（起点）
*	@note	1x1：原点格子与锚点逻辑坐标一致
*	@note	2x2：原点格子与锚点逻辑坐标一致
*	@note	3x3：原点格子锚点逻辑坐标(-1, -1)处(左下方)(锚点居中)
*	@note	原点格子锚点逻辑坐标(-1, -1)处(左下方)
*	@note	原点格子锚点逻辑坐标(-2, -2)处(左下方)(锚点居中)
*/
FIntPoint AAppGridTileManager::CalcOriginCoordFromAnchor(const FIntPoint& AnchorCoord, const FIntPoint& FootprintSize) const
{
	const FIntPoint AnchorOffset(
		(FootprintSize.X - 1) / 2,
		(FootprintSize.Y - 1) / 2
	);

	return AnchorCoord - AnchorOffset;
}

/**
*	@brief　算出某建筑所需的占地(Footprint)格子
*	@note	逻辑上塔的占地计算会从左下角算起(X与Y的最小值)
*/
TArray<FIntPoint> AAppGridTileManager::GetFootprintCoords(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const
{
	TArray<FIntPoint> Result;
	for (int32 Y = 0; Y < FootprintSize.Y; ++Y)
	{
		for (int32 X = 0; X < FootprintSize.X; ++X)
		{
			Result.Add(FIntPoint(OriginCoord.X + X, OriginCoord.Y + Y));
		}
	}

	return Result;
}

/**
*	@brief　格子可部署状态检测
*	@note	对复数格子同时进行检测
*/
bool AAppGridTileManager::CanPlaceAtCoords(const TArray<FIntPoint>& Coords) const
{
	for (const FIntPoint& Coord : Coords)
	{
		// 是否在逻辑范围内
		if (!IsValidCoord(Coord))
		{
			return false;
		}

		const AAppGridTile* Tile = GetTileByCoord(Coord);
		if (!Tile)
		{
			return false;
		}

		// 是否为可建造地形或已被占领
		if (!Tile->IsBuildable())
		{
			return false;
		}
	}

	return true;
}

/**
*	@brief　根据可部署状态启动格子的状态高光显示
*/
void AAppGridTileManager::ShowFootprintHighlight(const TArray<FIntPoint>& Coords, bool bBuildable)
{
	ClearAllHighlights();

	// 高光类型选择(可建造或不可建造)
	const EGridHighlightType HighlightType = bBuildable
		? EGridHighlightType::Buildable
		: EGridHighlightType::Unbuildable;

	for (const FIntPoint& Coord : Coords)
	{
		AAppGridTile* Tile = GetTileByCoord(Coord);
		if (!Tile)
		{
			continue;
		}

		Tile->SetTileHighlight(HighlightType);
		CurrentHighlightedTiles.Add(Tile);
	}
}

/**
*	@brief　清空所有格子的状态高光显示
*/
void AAppGridTileManager::ClearAllHighlights()
{
	for (AAppGridTile* Tile : CurrentHighlightedTiles)
	{
		if (Tile)
		{
			Tile->ClearTileHighlight();
		}
	}
	CurrentHighlightedTiles.Empty();
}

/**
*	@brief　逻辑格子与占领对象绑定
*/
bool AAppGridTileManager::OccupyCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor)
{
	if (!OwnerActor)
	{
		return false;
	}

	// 可部署检测
	if (!CanPlaceAtCoords(Coords))
	{
		return false;
	}

	// 设置格子的占领关系
	for (const FIntPoint& Coord : Coords)
	{
		if (AAppGridTile* Tile = GetTileByCoord(Coord))
		{
			Tile->SetTileOccupied(OwnerActor);
		}
	}

	return true;
}

//@sc
void AAppGridTileManager::ClearOccupiedCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor)
{
	for (const FIntPoint& Coord : Coords)
	{
		AAppGridTile* Tile = GetTileByCoord(Coord);
		if (!Tile)
		{
			continue;
		}

		if (OwnerActor && Tile->GetOccupiedActor() != OwnerActor)
		{
			continue;
		}

		Tile->ClearTileOccupied();
	}
}

//@sc
/**
*	@brief	预览体相关格子状态更新
*	@param	PlayerController_射线检测用
*/
void AAppGridTileManager::UpdatePlacementPreview(APlayerController* PlayerController, const FIntPoint& FootprintSize)
{
	AAppGridTile* CursorTile = nullptr;

	// 找到有效格子
	if (!GetTileUnderCursor(PlayerController, CursorTile))
	{
		ClearAllHighlights();
		return;
	}

	// 当前鼠标所在格子
	const FIntPoint AnchorCoord = CursorTile->GetGridCoord();

	// 计算建筑锚点格子(左下)
	const FIntPoint OriginCoord = CalcOriginCoordFromAnchor(AnchorCoord, FootprintSize);

	// 算出所有占地格子
	const TArray<FIntPoint> Coords =
		GetFootprintCoords(
			OriginCoord,
			FootprintSize
		);

	// 是否允许放置
	const bool bCanPlace = CanPlaceAtCoords(Coords);

	// 显示高光
	ShowFootprintHighlight(Coords, bCanPlace);
}