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

/**
* @brief　	根据Spline节点数组重新配置Spline各个节点
* @note		EUF函数
* @note		各节点默认为Linear
*/
void AAppMarchRoute::EUF_RebuildRouteSpline()
{
	if (!RouteSplineComp)
	{
		return;
	}

	// 清空Spline节点
	RouteSplineComp->ClearSplinePoints(false);

	// 根据节点数组生成新节点
	for (int32 i = 0; i < RouteWorldPoints.Num(); ++i)
	{
		RouteSplineComp->AddSplinePoint(RouteWorldPoints[i], ESplineCoordinateSpace::World, false
		);

		// 样式需为Linear
		RouteSplineComp->SetSplinePointType(i, ESplinePointType::Linear, false);
	}
	RouteSplineComp->UpdateSpline();	// @memo Spline修改完成（不要忘了这个）

	// 保存Level资产
	MarkPackageDirty();
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



