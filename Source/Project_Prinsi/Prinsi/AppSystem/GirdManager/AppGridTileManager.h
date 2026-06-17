// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppGridTileManager.generated.h"

//――――――――――――――――――――
// 前向声明
//――――――――――――――――――――
class AAppGridTile;


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

	//――――――――――――――――――――
	// Misc
	//――――――――――――――――――――
public:
	AAppGridTile* GetTileByCoord(const FIntPoint& Coord) const;				// 根据逻辑坐标检索格子
	bool IsValidCoord(const FIntPoint& Coord) const;						// 确认逻辑格子是否合法

	// @memo GridManager的坐标应为0号格子的坐标
	FVector GridCoordToWorldLocation(const FIntPoint& Coord) const;			// 根据逻辑坐标获取世界坐标	
	FIntPoint WorldLocationToGridCoord(const FVector& WorldLocation) const;	// 根据世界坐标获取逻辑坐标

protected:
	void GenerateGrid();	// 生成所有格子
	void ClearGrid();		// 清空所有格子

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GridTileManager|Config")
	TSubclassOf<AAppGridTile> TileClass;	// 逻辑格子类类型

	UPROPERTY(VisibleAnywhere, Category = "GridTileManager|Status")
	TMap<FIntPoint, AAppGridTile*> GridMap;	// 逻辑格子管理数组	

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Config")
	int32 GridWidth = 10;					// 横向格子数量

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Config")
	int32 GridHeight = 10;					// 纵向格子数量

	UPROPERTY(EditAnywhere, Category = "GridTileManager|Config", Meta = (ClampMin = "1.0"))
	float TileSize = 100.0f;				// 格子尺寸

	//――――――――――――――――――――
	// EUF
	//――――――――――――――――――――
	// @memo(MarkPackageDirty) 编辑器改动提示
public:
	UFUNCTION(CallInEditor, Category = "GridTileManager|EUF")
	void EUFGenerateGrid();		// 生成所有格子

	UFUNCTION(CallInEditor, Category = "GridTileManager|EUF")
	void EUFClearGrid();		// 删除所有格子

	//――――――――――――――――――――
	// 建造相关（Construction）
	//――――――――――――――――――――
public:
	// 算出某建筑所需的占地(Footprint)格子（逻辑上塔的占地计算会从左下角算起(X与Y的最小值)）
	TArray<FIntPoint> GetFootprintCoords(const FIntPoint& OriginCoord, const FIntPoint& FootprintSize) const;

	// 格子的可部署检测(复)
	bool CanPlaceAtCoords(const TArray<FIntPoint>& Coords) const;

	// 对象Actor占领格子处理
	bool OccupyCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor);

	// 解除对象Actor对格子的占领
	void ClearOccupiedCoords(const TArray<FIntPoint>& Coords, AActor* OwnerActor = nullptr);


	//――――――――――――――――――――
	// 格子的状态高光显示处理
	//――――――――――――――――――――
protected:
	UPROPERTY()
	TArray<AAppGridTile*> CurrentHighlightedTiles;		// 当前激活状态高光显示的格子

public:
	// 启动格子的状态高光显示(可部署)
	void ShowFootprintHighlight(const TArray<FIntPoint>& Coords, bool bBuildable);

	// 隐藏所有格子的状态高光显示
	void ClearAllHighlights();


	//――――――――――――――――――――
	// 鼠标指针交互（Cursor）
	//――――――――――――――――――――
public:
	bool GetTileUnderCursor(APlayerController* PlayerController, AAppGridTile*& OutTile) const;

	// 通过锚点格子(鼠标)算出建筑原点格子(左下角)
	FIntPoint CalcOriginCoordFromAnchor(const FIntPoint& AnchorCoord, const FIntPoint& FootprintSize) const;

	// 更新预览体位置
	void UpdatePlacementPreview(APlayerController* PlayerController, const FIntPoint& FootprintSize);
};
