// Fill out your copyright notice in the Description page of Project Settings.

#include "Prinsi/AppSystem/GameLoop/AppMarchRoute.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Components/SplineComponent.h"


// Sets default values
AAppMarchRoute::AAppMarchRoute()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RouteSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("RouteSpline"));

	SetRootComponent(RootComp);
	RouteSplineComp->SetupAttachment(RootComp);

	// ~~InitComp_Sline组件
	RouteSplineComp->SetClosedLoop(false);
}

// Called when the game starts or when spawned
void AAppMarchRoute::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAppMarchRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AAppMarchRoute::GetRoutePointCount() const
{
	if (!RouteSplineComp)
	{
		return 0;
	}

	return RouteSplineComp->GetNumberOfSplinePoints();
}

FVector AAppMarchRoute::GetRoutePointLocation(int32 PointIndex) const
{
	if (!RouteSplineComp)
	{
		return GetActorLocation();
	}

	const int32 PointCount = RouteSplineComp->GetNumberOfSplinePoints();
	if (PointIndex < 0 || PointIndex >= PointCount)
	{
		// @thumb 
		APP_SCR_ERROR(TEXT("（thumb）已抵达MarchRoute终点却依然执行移动任务。"));
		return GetActorLocation();
	}

	return RouteSplineComp->GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
}

