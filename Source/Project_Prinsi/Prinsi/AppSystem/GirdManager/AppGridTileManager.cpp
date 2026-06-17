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
	GenerateGrid();
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

void AAppGridTileManager::GenerateGrid()
{
	ClearGrid();

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

#if WITH_EDITOR
			// @scaff 为生成的逻辑格子编号方便确认
			NewTile->SetActorLabel(FString::Printf(TEXT("GridTile_%d_%d"), X, Y));
#endif
			// 加入Map管理
			GridMap.Add(Coord, NewTile);
		}
	}
}

void AAppGridTileManager::ClearGrid() {
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

void AAppGridTileManager::EUFGenerateGrid() {
#if WITH_EDITOR
	ClearGrid();
	GenerateGrid();
	MarkPackageDirty();		
#endif
}

void AAppGridTileManager::EUFClearGrid() {
#if WITH_EDITOR
	ClearGrid();
	MarkPackageDirty();
#endif
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

bool AAppGridTileManager::GetTileUnderCursor(APlayerController* PlayerController, AAppGridTile*& OutTile) const {
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

FIntPoint AAppGridTileManager::CalcOriginCoordFromAnchor(const FIntPoint& AnchorCoord, const FIntPoint& FootprintSize) const {
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

void AAppGridTileManager::UpdatePlacementPreview(APlayerController* PlayerController, const FIntPoint& FootprintSize) {
	AAppGridTile* CursorTile = nullptr;

	// 找到有效格子
	if (!GetTileUnderCursor(PlayerController, CursorTile)) {
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