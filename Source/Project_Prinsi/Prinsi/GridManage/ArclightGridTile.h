// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prinsi/Define/AppDefineGridManager.h"			//[p]
#include "ArclightGridTile.generated.h"


class UBoxComponent;

UCLASS()
class PROJECT_PRINSI_API AArclightGridTile : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArclightGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "GridTile|Component")
	USceneComponent* RootComp_;

	UPROPERTY(VisibleAnywhere, Category = "GridTile|Component")
	UStaticMeshComponent* TileMesh_;

	UPROPERTY(VisibleAnywhere, Category = "GridTile|Component")
	UStaticMeshComponent* HighlightMesh_;

	//[TODO]根据之后设计考虑是否加入碰撞盒
	//UPROPERTY(VisibleAnywhere, Category = "GridTile|Component")
	//UBoxComponent* ColBoxComp_;

	// ~~Misc
public:
	void InitTile(const FIntPoint& InCoord, EGridTileType InTileType);	// 对象初始化

public:
	FIntPoint GetGridCoord() const { return GridCoord_; }	// (Getter)逻辑坐标
	EGridTileType GetTileType() const { return TileType_; }	// (Getter)格子类型
	void SetTileType(EGridTileType InTileType);				// (Setter)格子类型
	bool IsBuildable() const;								// (Check)是否是可建造格

public:
	void ClearTileHighlight();								// 隐藏格子状态Mesh
	void SetTileHighlight(EGridHighlightType HighlightType);// 根据状态类型设置状态Mesh材质

private:
	void ApplyTileVisual();									// 根据地形类型设置本体Mesh材质


private:
	UPROPERTY(VisibleAnywhere, Category = "State GridGrid")
	FIntPoint GridCoord_ = FIntPoint::ZeroValue;			// 逻辑坐标

	UPROPERTY(EditAnywhere, Category = "State GridGrid")
	EGridTileType TileType_ = EGridTileType::Ground;		// 地形类型

	// ~格子地形类型材质
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* GroundMaterial_;					// 地形类型(平地)

	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* HighGroundMaterial_;				// 地形类型(高地)

	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* BlockedMaterial_;					// 地形类型(禁止区域)

	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* WaterMaterial_;						// 地形类型(水)

	// ~格子状态类型材质
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* BuildableHighlightMaterial_;		// 状态类型(可建造提示)

	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* UnbuildableHighlightMaterial_;		// 状态类型(不可建造提示)

	UPROPERTY(EditDefaultsOnly, Category = "Ref GridGrid|Material")
	UMaterialInterface* SelectedHighlightMaterial_;			// 状态类型(被选中提示)


	// ~~占领处理(Occupied)
private:
	UPROPERTY(VisibleAnywhere, Category = "State GridGrid")
	bool bIsOccupied_ = false;									// 是否被占领

	UPROPERTY(VisibleAnywhere, Category = "State GridGrid")
	AActor* OccupiedActor_ = nullptr;							// 占领对象

public:
	bool IsTileOccupied() const { return bIsOccupied_; }			// (Check)是否被占领
	void ClearTileOccupied();										// 清除占领对象
	void SetTileOccupied(AActor* InActor);							// (Setter)格子的占领对象
	AActor* GetOccupiedActor() const { return OccupiedActor_; }		// (Getter)格子的占领对象
};
