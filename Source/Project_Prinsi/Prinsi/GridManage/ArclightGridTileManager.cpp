// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GridManage/ArclightGridTileManager.h"

#include "Prinsi/GridManage/ArclightGridTile.h"		


// Sets default values
AArclightGridTileManager::AArclightGridTileManager() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AArclightGridTileManager::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AArclightGridTileManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

AArclightGridTile* AArclightGridTileManager::GetTileByCoord(const FIntPoint& Coord) const {
	// 根据逻辑坐标找到对应格子
	if (AArclightGridTile* const* FoundTile = GridMap_.Find(Coord)) {
		return *FoundTile;
	}
	return nullptr;
}

bool AArclightGridTileManager::IsValidCoord(const FIntPoint& Coord) const {
	return Coord.X >= 0
		&& Coord.Y >= 0
		&& Coord.X < GridWidth_
		&& Coord.Y < GridHeight_;
}

//FVector AArclightGridTileManager::GridCoordToWorldLocation(const FIntPoint& Coord) const {
//	const FVector Origin = GetActorLocation();
//
//	return Origin + FVector(
//		Coord.X * TileSize_,
//		Coord.Y * TileSize_,
//		0.0f
//	);
//
//	return FVector();
//}

TArray<FIntPoint> AArclightGridTileManager::GetFootprintCoords(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const {
	TArray<FIntPoint> Result;
	for (int32 Y = 0; Y < FootprintSize.Y; ++Y) {
		for (int32 X = 0; X < FootprintSize.X; ++X) {
			Result.Add(FIntPoint(OriginCoord.X + X, OriginCoord.Y + Y));
		}
	}

	return Result;
}

//bool AArclightGridTileManager::CanPlaceAtCoord(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const {
//	const TArray<FIntPoint> Coords = GetFootprintCoords(OriginCoord, FootprintSize);
//	return CanPlaceAtCoords(Coords);
//}

bool AArclightGridTileManager::CanPlaceAtCoords(const TArray<FIntPoint>& Coords) const {
	for (const FIntPoint& Coord : Coords) {
		// 是否在逻辑范围内
		if (!IsValidCoord(Coord)) {
			return false;
		}

		const AArclightGridTile* Tile = GetTileByCoord(Coord);
		if (!Tile) {
			return false;
		}

		// 是否为可建造地形	// 确认是否已被占领
		if (!Tile->IsBuildable()) {
			return false;
		}
	}

	return true;

}

bool AArclightGridTileManager::OccupyCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor) {
	if (!OwnerActor) {
		return false;
	}

	// 可部署检测
	if (!CanPlaceAtCoords(Coords)) {
		return false;
	}

	// 设置格子的占领关系
	for (const FIntPoint& Coord : Coords) {
		if (AArclightGridTile* Tile = GetTileByCoord(Coord)) {
			Tile->SetTileOccupied(OwnerActor);
		}
	}

	return true;
}

void AArclightGridTileManager::ClearOccupiedCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor) {
	for (const FIntPoint& Coord : Coords) {
		AArclightGridTile* Tile = GetTileByCoord(Coord);
		if (!Tile) { continue; }

		if (OwnerActor && Tile->GetOccupiedActor() != OwnerActor) { continue; }

		Tile->ClearTileOccupied();
	}
}

void AArclightGridTileManager::ClearAllHighlights() {
	for (AArclightGridTile* Tile : CurrentHighlightedTiles_) {
		if (Tile) {
			Tile->ClearTileHighlight();
		}
	}
	CurrentHighlightedTiles_.Empty();
}

bool AArclightGridTileManager::GetTileUnderCursor(APlayerController* PlayerController, AArclightGridTile*& OutTile) const {
	OutTile = nullptr;

	// 从当前的控制器的鼠标指针处进行射线检测
	if (!PlayerController) { return false; }
	FHitResult HitResult;
	const bool bHit = PlayerController->GetHitResultUnderCursor(
		ECC_Visibility,
		false,
		HitResult
	);
	if (!bHit) { return false; }

	OutTile = Cast<AArclightGridTile>(HitResult.GetActor());
	return OutTile != nullptr;
}

void AArclightGridTileManager::EUFGenerateGrid() {
#if WITH_EDITOR
	ClearGrid();
	GenerateGrid();
	MarkPackageDirty();		// 编辑器改动提示
#endif
}

void AArclightGridTileManager::EUFClearGrid() {
#if WITH_EDITOR
	ClearGrid();
	MarkPackageDirty();
#endif
}

void AArclightGridTileManager::GenerateGrid() {
	ClearGrid();

	if (!TileClass_) {
		UE_LOG(LogTemp, Warning, TEXT("ERROR_格子生成失败_格子类型未设置_ArclightGridTileManager.cpp"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) { return; }

	for (int32 Y = 0; Y < GridHeight_; ++Y) {
		for (int32 X = 0; X < GridWidth_; ++X) {
			const FIntPoint Coord(X, Y);
			const FVector SpawnLocation = GridCoordToWorldLocation(Coord);
			const FTransform SpawnTransform(SpawnLocation);

			// 生成单位格子
			AArclightGridTile* NewTile = World->SpawnActor<AArclightGridTile>(TileClass_, SpawnTransform);
			if (!NewTile) {
				UE_LOG(LogTemp, Warning, TEXT("ERROR_格子生成失败_ArclightGridTileManager.cpp"));
				continue;
			}

			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			// 对象初始化	//[TODO]暂时全部生成为平地
			NewTile->InitTile(Coord, EGridTileType::Ground);

#if WITH_EDITOR
			NewTile->SetActorLabel(FString::Printf(TEXT("GridTile_%d_%d"), X, Y));
#endif
			// 加入Map管理
			GridMap_.Add(Coord, NewTile);
		}
	}
}

void AArclightGridTileManager::ClearGrid() {
	ClearAllHighlights();

	for (auto& Pair : GridMap_) {
		AArclightGridTile* Tile = Pair.Value;
		if (Tile) { Tile->Destroy(); }
	}

	GridMap_.Empty();
}

void AArclightGridTileManager::ShowFootprintHighlight(const TArray<FIntPoint>& Coords, bool bBuildable) {
	ClearAllHighlights();

	// 高光类型选择(可建造||不可建造)
	const EGridHighlightType HighlightType = bBuildable
		? EGridHighlightType::Buildable
		: EGridHighlightType::Unbuildable;

	for (const FIntPoint& Coord : Coords) {
		AArclightGridTile* Tile = GetTileByCoord(Coord);
		if (!Tile) {
			continue;
		}

		Tile->SetTileHighlight(HighlightType);
		CurrentHighlightedTiles_.Add(Tile);
	}
}

FIntPoint AArclightGridTileManager::WorldLocationToGridCoord(const FVector& WorldLocation) const {
	const FVector LocalLocation = WorldLocation - GetActorLocation();

	//[N]FloorToInt向下取整
	const int32 X = FMath::FloorToInt(LocalLocation.X / TileSize_);
	const int32 Y = FMath::FloorToInt(LocalLocation.Y / TileSize_);

	return FIntPoint(X, Y);
}

FVector AArclightGridTileManager::GridCoordToWorldLocation(const FIntPoint& Coord) const {
	const FVector Origin = GetActorLocation();

	return Origin + FVector(
		Coord.X * TileSize_,
		Coord.Y * TileSize_,
		0.0f
	);
}
