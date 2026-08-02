// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AppSystem/GirdManager/AppGridTileManager.h"
#include "Prinsi/Define/AppDefineDebug.h"				// Define_Debug工具
#include "Prinsi/AppSystem/GirdManager/AppGridTile.h"	// Actor_逻辑格子


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

	// @todo 先生成所有格子
	//GenerateGrid();

	// @sc
	// 重构GridMap
	RebuildGridMap();

	// @sc
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



AAppGridTile* AAppGridTileManager::GetTileByCoord(const FIntPoint& Coord) const
{
	if (AAppGridTile* const* FoundTile = GridMap.Find(Coord))
	{
		return *FoundTile;
	}

	return nullptr;
}

bool AAppGridTileManager::IsValidCoord(const FIntPoint& Coord) const
{
	return Coord.X >= 0
		&& Coord.Y >= 0
		&& Coord.X < GridWidth
		&& Coord.Y < GridHeight;
}

FVector AAppGridTileManager::GridCoordToWorldLocation(const FIntPoint& Coord) const
{
	const FVector Origin = GetActorLocation();

	return Origin + FVector(
		Coord.X * TileSize,
		Coord.Y * TileSize,
		0.0f
	);
}

FIntPoint AAppGridTileManager::WorldLocationToGridCoord(const FVector& WorldLocation) const
{
	const FVector LocalLocation = WorldLocation - GetActorLocation();

	// @memo FloorToInt向下取整
	const int32 X = FMath::FloorToInt(LocalLocation.X / TileSize);
	const int32 Y = FMath::FloorToInt(LocalLocation.Y / TileSize);

	return FIntPoint(X, Y);
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
*	@brief　生成初始化Grid并加入Map中
*/
void AAppGridTileManager::GenerateAllGrid()
{
	ClearAllGrid();

	if (!TileClass)
	{
		APP_ERROR(TEXT("格子生成失败_格子类型未设置。")); return;
	}

	UWorld* World = GetWorld();
	if (!World) { return; }

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
				APP_ERROR(TEXT("格子生成失败_SpawnActor。")); continue;
			}

			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			// 对象初始化	
			// @todo 暂时全部生成为平地
			NewTile->InitTile(Coord, EGridTileType::Ground);

#if !UE_BUILD_SHIPPING
			// @scaff 为生成的逻辑格子编号方便确认
			NewTile->SetActorLabel(FString::Printf(TEXT("GridTile_%d_%d"), X, Y));
#endif
			// 加入Map管理
			GridMap.Add(Coord, NewTile);
		}
	}
}

/**
*	@brief　寻找Attach的Grid对象并登录到Map中
*	@note	BeginPlay调用
*/
void AAppGridTileManager::RebuildGridMap()
{
	GridMap.Empty();

	// 寻找Attach的GridTile对象并登录到Map
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		AAppGridTile* Tile = Cast<AAppGridTile>(Actor);
		if (!Tile)
		{
			continue;
		}

		// @thumb 
		const FIntPoint Coord = Tile->GetGridCoord();
		if (GridMap.Contains(Coord))
		{

			APP_SCR_ERROR(TEXT("发现重复格子坐标：(%d, %d)"), Coord.X, Coord.Y);
			continue;
		}

		GridMap.Add(Tile->GetGridCoord(), Tile);
	}
}

/**
*	@brief　生成初始化Grid并加入Map中
*	@note	EUF调用
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
*	@note	EUF调用
*/
void AAppGridTileManager::EUFClearGrid()
{
#if WITH_EDITOR
	ClearAllGrid();
	MarkPackageDirty();
#endif
}

void AAppGridTileManager::EUFToggleAllGridVisible()
{
#if WITH_EDITOR
	bGridVisible = !bGridVisible;
	SetAllGridVisible(bGridVisible);
	MarkPackageDirty();
#endif
}

void AAppGridTileManager::EUFRebuildGridMap()
{
#if WITH_EDITOR
	RebuildGridMap();
	MarkPackageDirty();
#endif
}

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

void AAppGridTileManager::ShowFootprintHighlight(const TArray<FIntPoint>& Coords, bool bBuildable)
{
	ClearAllHighlights();

	// 高光类型选择(可建造||不可建造)
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
		ECC_Visibility,
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

FIntPoint AAppGridTileManager::CalcOriginCoordFromAnchor(const FIntPoint& AnchorCoord, const FIntPoint& FootprintSize) const
{
	/*
		1x1：原点格子与锚点格子一致
		2x2：原点格子与锚点格子一致
		3x3：原点格子在锚点格子(-1,-1)处(左下方)(锚点居中)
		4x4：原点格子在锚点格子(-1,-1)处(左下方)
		5x5：原点格子在锚点格子(-2,-2)处(左下方)(锚点居中)
	*/
	const FIntPoint AnchorOffset(
		(FootprintSize.X - 1) / 2,
		(FootprintSize.Y - 1) / 2
	);

	return AnchorCoord - AnchorOffset;
}

FVector AAppGridTileManager::CalcFootprintCenterWorldLocation(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const
{
	// 获取起点逻辑格子的实际坐标
	const FVector OriginTileWorldLocation = GridCoordToWorldLocation(OriginCoord);

	if (FootprintSize.X <= 0 || FootprintSize.X <= 0)
	{
		APP_WARNING(TEXT("FootprintSize不合法!")); return OriginTileWorldLocation;
	}

	const float OffsetX = static_cast<float>(FootprintSize.X - 1) * TileSize * 0.5f;
	const float OffsetY = static_cast<float>(FootprintSize.Y - 1) * TileSize * 0.5f;

	return OriginTileWorldLocation + FVector(OffsetX, OffsetY, 0.0f);
}


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