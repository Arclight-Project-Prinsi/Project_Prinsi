// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GridManage/ArclightGridTile.h"

#include "Components/BoxComponent.h"


// Sets default values
AArclightGridTile::AArclightGridTile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//[p]Grid是否真的需要网格模型？（还是仅作为逻辑存在？）
	RootComp_ = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	TileMesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	HighlightMesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	//[p]ColBoxComp_ = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	RootComponent = RootComp_;
	TileMesh_->SetupAttachment(RootComponent);
	HighlightMesh_->SetupAttachment(RootComponent);
	//[p]ColBoxComp_->SetupAttachment(RootComponent);

	{
		TileMesh_->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TileMesh_->SetCollisionResponseToAllChannels(ECR_Ignore);
		TileMesh_->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	{
		HighlightMesh_->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HighlightMesh_->SetVisibility(false);
		HighlightMesh_->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));		// 稍微往上挪一点
	}
}


// Called when the game starts or when spawned
void AArclightGridTile::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AArclightGridTile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AArclightGridTile::InitTile(const FIntPoint& InCoord, EGridTileType InTileType) {
	GridCoord_ = InCoord;
	TileType_ = InTileType;

	ApplyTileVisual();
	ClearTileHighlight();
	ClearTileOccupied();
}

void AArclightGridTile::SetTileType(EGridTileType InTileType) {
	TileType_ = InTileType;
	ApplyTileVisual();
}

bool AArclightGridTile::IsBuildable() const {
	// 确认是否已被占领(Occupied)
	if (bIsOccupied_) { return false; }

	// 确认地形是否正确
	switch (TileType_)
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

void AArclightGridTile::ApplyTileVisual() {
	if (!TileMesh_) { return; }
	UMaterialInterface* TargetMat = nullptr;

	switch (TileType_)
	{
	case EGridTileType::Ground:
		TargetMat = GroundMaterial_;
		break;

	case EGridTileType::HighGround:
		TargetMat = HighGroundMaterial_;
		break;

	case EGridTileType::Blocked:
		TargetMat = BlockedMaterial_;
		break;

	case EGridTileType::Water:
		TargetMat = WaterMaterial_;
		break;

	default:
		break;
	}

	if (TargetMat) {
		TileMesh_->SetMaterial(0, TargetMat);
	}
}

void AArclightGridTile::SetTileHighlight(EGridHighlightType HighlightType) {
	if (!HighlightMesh_) { return; }

	UMaterialInterface* TargetMat = nullptr;

	switch (HighlightType)
	{
	case EGridHighlightType::Buildable:
		TargetMat = BuildableHighlightMaterial_;
		break;

	case EGridHighlightType::Unbuildable:
		TargetMat = UnbuildableHighlightMaterial_;
		break;

	case EGridHighlightType::Selected:
		TargetMat = SelectedHighlightMaterial_;
		break;

	case EGridHighlightType::None:
	default:
		ClearTileHighlight();
		return;
	}

	if (TargetMat) { HighlightMesh_->SetMaterial(0, TargetMat); }
	HighlightMesh_->SetVisibility(true);
}

void AArclightGridTile::ClearTileOccupied() {
	bIsOccupied_ = false;
	OccupiedActor_ = nullptr;
}

void AArclightGridTile::SetTileOccupied(AActor* InActor) {
	if (!InActor) {
		ClearTileOccupied();
		return;
	}

	bIsOccupied_ = true;
	OccupiedActor_ = InActor;
}

void AArclightGridTile::ClearTileHighlight() {
	if (HighlightMesh_) {
		HighlightMesh_->SetVisibility(false);
	}
}

