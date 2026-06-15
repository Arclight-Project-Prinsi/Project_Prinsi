// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArclightGridTileManager.generated.h"


class AArclightGridTile;

UCLASS()
class PROJECT_PRINSI_API AArclightGridTileManager : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArclightGridTileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//WS-------------------------------------------------------
	// ~~Misc
public:
	AArclightGridTile* GetTileByCoord(const FIntPoint& Coord) const;	// 根据逻辑坐标检索格子
	bool IsValidCoord(const FIntPoint& Coord) const;					// 确认逻辑格子是否合法

	// 根据逻辑坐标获取世界坐标	// GridManager的坐标应为0号格子的坐标
	FVector GridCoordToWorldLocation(const FIntPoint& Coord) const;
	// 根据世界坐标获取逻辑坐标
	FIntPoint WorldLocationToGridCoord(const FVector& WorldLocation) const;

private:
	void GenerateGrid();			// 生成所有格子
	void ClearGrid();				// 清空所有格子

public:
	UFUNCTION(CallInEditor, Category = "Grid|Editor")
	void EUFGenerateGrid();		// (EUF)生成所有格子

	UFUNCTION(CallInEditor, Category = "Grid|Editor")
	void EUFClearGrid();		// (EUF)删除所有格子

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ref GridTileManager")
	TSubclassOf<AArclightGridTile> TileClass_;				// 格子BP

	UPROPERTY(VisibleAnywhere, Category = "State GridTileManager")
	TMap<FIntPoint, AArclightGridTile*> GridMap_;			// 生成的格子	

	UPROPERTY(EditAnywhere, Category = "Config GridTileManager")
	int32 GridWidth_ = 10;		// 横向格子数量

	UPROPERTY(EditAnywhere, Category = "Config GridTileManager")
	int32 GridHeight_ = 10;		// 纵向格子数量

	UPROPERTY(EditAnywhere, Category = "Config GridTileManager", Meta = (ClampMin = "1.0"))
	float TileSize_ = 100.0f;	// 格子尺寸


	// ~~建造相关(construction)
public:
	// 算出某建筑所需的占地(Footprint)格子	// 逻辑上塔的占地计算会从左下角算起(X与Y的最小值)
	TArray<FIntPoint> GetFootprintCoords(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const;

	// 格子的可部署检测(复)
	bool CanPlaceAtCoords(const TArray<FIntPoint>& Coords) const;

	// 对象Actor占领格子处理
	bool OccupyCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor);

	// 解除对象Actor对格子的占领
	void ClearOccupiedCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor = nullptr);


	// ~~格子状态高光显示
private:
	UPROPERTY()
	TArray<AArclightGridTile*> CurrentHighlightedTiles_;	// 当前激活状态高光显示的格子

public:
	// 启动格子的状态高光显示(可部署)
	void ShowFootprintHighlight(const TArray<FIntPoint>& Coords, bool bBuildable);

	// 隐藏所有格子的状态高光显示
	void ClearAllHighlights();


	// ~~指针交互(Cursor)
	// 根据指针获取对应格子
	bool GetTileUnderCursor(APlayerController* PlayerController, AArclightGridTile*& OutTile) const;

	//ws---------------------------------------

	// 通过锚点格子(鼠标)算出建筑原点格子(左下角)
	FIntPoint CalcOriginCoordFromAnchor(const FIntPoint& AnchorCoord, const FIntPoint& FootprintSize) const;

	// 更新预览体位置
	void UpdatePlacementPreview(APlayerController* PlayerController, const FIntPoint& FootprintSize);

	//ws2----------------------------------------
	//[p]
	//[T]预览体测试
	// ~~测试用 Preview
//private:
//	UPROPERTY(EditAnywhere, Category = "Test")
//	bool bEnablePlacementPreviewTest_ = true;
//
//	UPROPERTY(EditAnywhere, Category = "Test")
//	FIntPoint TestFootprintSize_ = FIntPoint(3, 3);
//
//private:
//	void UpdatePlacementPreviewTest();
};
