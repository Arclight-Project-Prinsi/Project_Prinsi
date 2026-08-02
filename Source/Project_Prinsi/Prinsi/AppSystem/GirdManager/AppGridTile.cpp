// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AppSystem/GirdManager/AppGridTile.h"

#include "Components/BoxComponent.h"


AAppGridTile::AAppGridTile()
{
	PrimaryActorTick.bCanEverTick = false;

	//[p]Grid是否真的需要网格模型？（还是仅作为逻辑存在？）
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	//[p]ColBoxComp_ = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	RootComponent = RootComp;
	TileMesh->SetupAttachment(RootComponent);
	HighlightMesh->SetupAttachment(RootComponent);
	//[p]ColBoxComp_->SetupAttachment(RootComponent);

	// Scene Component Init_常态格子网格体
	{
		TileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		TileMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	// Scene Component Init_高光格子网格体
	{
		HighlightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HighlightMesh->SetVisibility(false);
		HighlightMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));		// 稍微往上挪一点
	}
}

// Called when the game starts or when spawned
void AAppGridTile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAppGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAppGridTile::SetGridVisible(bool bVisible)
{
#if !UE_BUILD_SHIPPING
	TileMesh->SetVisibility(bVisible);
	if (!bVisible)
	{
		HighlightMesh->SetVisibility(false);
	}
#endif
}

void AAppGridTile::InitTile(const FIntPoint& InCoord, EGridTileType InTileType)
{
	GridCoord = InCoord;
	TileType = InTileType;

	ApplyTileVisual();
	ClearTileHighlight();
	ClearTileOccupied();
}

void AAppGridTile::SetTileType(EGridTileType InTileType)
{
	TileType = InTileType;
	ApplyTileVisual();
}

bool AAppGridTile::IsBuildable() const
{
	// 确认是否已被占领(Occupied)
	if (bIsOccupied) { return false; }

	// 确认地形是否正确
	switch (TileType)
	{
	case EGridTileType::Ground:
	case EGridTileType::HighGround:
		return true;

	case EGridTileType::Blocked:
	case EGridTileType::Water:
	default:
		return false;
	}
}

void AAppGridTile::ApplyTileVisual()
{
	if (!TileMesh) { return; }
	UMaterialInterface* TargetMat = nullptr;

	switch (TileType)
	{
	case EGridTileType::Ground:
		TargetMat = GroundMaterial;
		break;

	case EGridTileType::HighGround:
		TargetMat = HighGroundMaterial;
		break;

	case EGridTileType::Blocked:
		TargetMat = BlockedMaterial;
		break;

	case EGridTileType::Water:
		TargetMat = WaterMaterial;
		break;

	default:
		break;
	}

	if (TargetMat)
	{
		TileMesh->SetMaterial(0, TargetMat);
	}
}

void AAppGridTile::SetTileHighlight(EGridHighlightType HighlightType)
{
	if (!HighlightMesh) { return; }

	UMaterialInterface* TargetMat = nullptr;

	switch (HighlightType)
	{
	case EGridHighlightType::Buildable:
		TargetMat = BuildableHighlightMaterial;
		break;

	case EGridHighlightType::Unbuildable:
		TargetMat = UnbuildableHighlightMaterial;
		break;

	case EGridHighlightType::Selected:
		TargetMat = SelectedHighlightMaterial;
		break;

	case EGridHighlightType::None:
	default:
		ClearTileHighlight();
		return;
	}

	if (TargetMat) { HighlightMesh->SetMaterial(0, TargetMat); }
	HighlightMesh->SetVisibility(true);
}

void AAppGridTile::ClearTileOccupied()
{
	bIsOccupied = false;
	OccupiedActor = nullptr;
}

void AAppGridTile::SetTileOccupied(AActor* InActor)
{
	if (!InActor)
	{
		ClearTileOccupied();
		return;
	}

	bIsOccupied = true;
	OccupiedActor = InActor;
}

void AAppGridTile::ClearTileHighlight()
{
	if (HighlightMesh)
	{
		HighlightMesh->SetVisibility(false);
	}
}
