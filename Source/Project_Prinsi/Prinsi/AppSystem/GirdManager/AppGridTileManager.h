// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppGridTileManager.generated.h"

class AAppGridTile;

//@sc GridManager一个Level只能有一个，否则会出现GridMap管理上的问题

UCLASS()
class PROJECT_PRINSI_API AAppGridTileManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppGridTileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	bool GetTileUnderCursor(					// 根据当前鼠标指针对格子（Grid）进行射线检测
		APlayerController* PlayerController,
		AAppGridTile*& OutTile
	) const;

	//~~ 格子管理相关 ~~//
protected:
	void ClearAllGrid();						// 清空当前Map中的Grid并销毁对象
	void GenerateAllGrid();						// 生成初始化Grid并加入GridMap中
	void RebuildGridMap();						// 遍历World中的Grid对象并登录到Map中

	void SetAllGridVisible(bool bVisible);		// 切换Gird可视性	//@todo 常态下应该不需要显示逻辑格子?

	// EUF调用接口
public:
	UFUNCTION(CallInEditor, Category = "GridTileManager|EUF")
	void EUFGenerateGrid();
	UFUNCTION(CallInEditor, Category = "GridTileManager|EUF")
	void EUFClearGrid();
	UFUNCTION(CallInEditor, Category = "GridTileManager|EUF")
	void EUFRebuildGridMap();

	UFUNCTION(CallInEditor, Category = "GridTileManager|EUF")
	void EUFToggleAllGridVisible();						//（Debug）切换Gird可视性

public:
	bool IsValidCoord(const FIntPoint& Coord) const;	// 确认逻辑坐标是否合法

	AAppGridTile* GetTileByCoord(const FIntPoint& Coord) const;				// 通过逻辑坐标，获得逻辑格子
	FVector GridCoordToWorldLocation(const FIntPoint& Coord) const;			// 通过逻辑坐标，获得实际坐标
	FIntPoint WorldLocationToGridCoord(const FVector& WorldLocation) const;	// 通过实际坐标，获得逻辑坐标

	FVector CalcFootprintCenterWorldLocation(			// 通逻辑坐标（起点），获得实际坐标（占地对象）
		const FIntPoint& OriginCoord,
		const FIntPoint& FootprintSize
	)const;
	FIntPoint CalcOriginCoordFromAnchor(				// 通过逻辑坐标（锚点），算出逻辑坐标（起点）
		const FIntPoint& AnchorCoord,
		const FIntPoint& FootprintSize
	) const;

	void UpdatePlacementPreview(						// 预览体相关格子状态更新
		APlayerController* PlayerController,
		const FIntPoint& FootprintSize
	);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GridTileManager|Config")
	TSubclassOf<AAppGridTile> TileClass;			// 逻辑格子类类型

	UPROPERTY(VisibleAnywhere, Category = "GridTileManager|Status")
	TMap<FIntPoint, AAppGridTile*> GridMap;			// 逻辑格子管理数组

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Status")
	bool bGridVisible = true;						// Gird可视性标识

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Config")
	int32 GridWidth = 10;							// 横向格子数量

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Config")
	int32 GridHeight = 10;							// 纵向格子数量

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Config", Meta = (ClampMin = "1.0"))
	float TileSize = 100.0f;						// 格子尺寸

protected:
	UPROPERTY()
	TArray<AAppGridTile*> CurrentHighlightedTiles;	// 当前激活状态高光显示的格子

	//~~ 建造相关（Construction） ~~//
public:
	TArray<FIntPoint> GetFootprintCoords(			// 算出某建筑所需的占地(Footprint)格子
		const FIntPoint& OriginCoord,
		const FIntPoint& FootprintSize
	) const;

	bool CanPlaceAtCoords(const TArray<FIntPoint>& Coords) const;	// 格子可部署状态检测
	void ShowFootprintHighlight(									// 根据可部署状态启动格子的状态高光显示
		const TArray<FIntPoint>& Coords,
		bool bBuildable
	);
	void ClearAllHighlights();										// 清空所有格子的状态高光显示

	bool OccupyCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor);	// 逻辑格子与占领对象绑定

	//ws----------------------------------------------------
	void ClearOccupiedCoords(												// 解除逻辑格子与占领对象绑定
		const TArray<FIntPoint>& Coords,
		AActor* OwnerActor = nullptr
	);
};
