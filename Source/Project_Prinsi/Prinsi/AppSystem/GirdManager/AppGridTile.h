// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include "Prinsi/Define/AppDefineGridManager.h"			//[p]

#include "GameFramework/Actor.h"
#include "AppGridTile.generated.h"

class UBoxComponent;


UCLASS()
class PROJECT_PRINSI_API AAppGridTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//--- （Debug）格子显示相关 ---
public:
	void SetGridVisible(bool bVisible);		//（Debug）切换TileMesh的可视性



	//ws----------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, Category = "Scene Component")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Scene Component")
	TObjectPtr<UStaticMeshComponent> TileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Scene Component")
	TObjectPtr<UStaticMeshComponent> HighlightMesh;

	//[TODO]根据之后设计考虑是否加入碰撞盒
	//UPROPERTY(VisibleAnywhere, Category = "GridTile|Component")
	//UBoxComponent* ColBoxComp_;

	// ~~Misc
public:
	void InitTile(const FIntPoint& InCoord, EGridTileType InTileType);	// 对象初始化

public:
	FIntPoint GetGridCoord() const { return GridCoord; }	// (Getter)逻辑坐标
	EGridTileType GetTileType() const { return TileType; }	// (Getter)格子类型
	void SetTileType(EGridTileType InTileType);				// (Setter)格子类型
	bool IsBuildable() const;								// (Check)是否是可建造格

public:
	void ClearTileHighlight();								// 隐藏格子状态Mesh
	void SetTileHighlight(EGridHighlightType HighlightType);// 根据状态类型设置状态Mesh材质

protected:
	void ApplyTileVisual();									// 根据地形类型设置本体Mesh材质


protected:
	UPROPERTY(VisibleAnywhere, Category = "Status GridGrid")
	FIntPoint GridCoord = FIntPoint::ZeroValue;			// 逻辑坐标

	UPROPERTY(EditAnywhere, Category = "Status GridGrid")
	EGridTileType TileType = EGridTileType::Ground;		// 地形类型

	// ~格子地形类型材质
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> GroundMaterial;					// 地形类型(平地)

	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> HighGroundMaterial;				// 地形类型(高地)

	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> BlockedMaterial;					// 地形类型(禁止区域)

	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> WaterMaterial;					// 地形类型(水)

	// ~格子状态类型材质
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> BuildableHighlightMaterial;		// 状态类型(可建造提示)

	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> UnbuildableHighlightMaterial;		// 状态类型(不可建造提示)

	UPROPERTY(EditDefaultsOnly, Category = "Config GridGrid|Material")
	TObjectPtr<UMaterialInterface> SelectedHighlightMaterial;			// 状态类型(被选中提示)


	// ~~占领处理(Occupied)
protected:
	UPROPERTY(VisibleAnywhere, Category = "Status GridGrid")
	bool bIsOccupied = false;									// 是否被占领

	UPROPERTY(VisibleAnywhere, Category = "Status GridGrid")
	TObjectPtr<AActor> OccupiedActor = nullptr;							// 占领对象

public:
	bool IsTileOccupied() const { return bIsOccupied; }			// (Check)是否被占领
	void ClearTileOccupied();										// 清除占领对象
	void SetTileOccupied(AActor* InActor);							// (Setter)格子的占领对象
	AActor* GetOccupiedActor() const { return OccupiedActor; }		// (Getter)格子的占领对象
};
