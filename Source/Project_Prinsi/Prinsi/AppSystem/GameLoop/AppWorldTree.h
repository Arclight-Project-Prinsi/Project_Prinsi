// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppWorldTree.generated.h"

class UBoxComponent;
class AAppEnemyCharacterBase;


UCLASS()
class PROJECT_PRINSI_API AAppWorldTree : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppWorldTree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WorldTree|Scene Component")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WorldTree|Scene Component")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WorldTree|Scene Component")
	TObjectPtr<UBoxComponent> CollisionArrivalComp;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldTree|Config")
	FVector EnemyDestination = { 0.0f,0.0f,0.0f };		// @todo 敌人进入世界树后的传送终点

protected:
	UFUNCTION()
	void OnCollisionArrivalBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	virtual void HandleEnemyArrived(AAppEnemyCharacterBase* Enemy);
};
