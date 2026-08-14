// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppMarchRoute.generated.h"

class USplineComponent;


UCLASS()
class PROJECT_PRINSI_API AAppMarchRoute : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppMarchRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//~~ Spline节点生成关联 ~~//
protected:
	UFUNCTION(CallInEditor, Category = "MarchRoute|EUF")
	void EUF_RebuildRouteSpline();

protected:
	UPROPERTY(EditAnywhere, Category = "MarchRoute|Config")
	TArray<FVector> RouteWorldPoints;		// EUF根据这个生成Spline节点

	//ws-----------------------------
public:
	int32 GetRoutePointCount()const;		// 获取行军路线（Spline）上总节点数

	FVector GetRoutePointLocation(int32 PointIndex) const;	// 获取指定路径点世界坐标

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MarchRoute|Scene Component")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "MarchRoute|Scene Component")
	TObjectPtr<USplineComponent> RouteSplineComp;

	//ws2-----------------

};
