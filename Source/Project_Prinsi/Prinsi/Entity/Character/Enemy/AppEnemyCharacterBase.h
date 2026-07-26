// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"
#include "AppEnemyCharacterBase.generated.h"

class AAppMarchRoute;


UCLASS()
class PROJECT_PRINSI_API AAppEnemyCharacterBase : public AAppCharacterBase
{
	GENERATED_BODY()

public:
	AAppEnemyCharacterBase();

protected:
	virtual void BeginPlay()override;

	//ws------------------------------
public:
	// @todo 已抵达终点（世界树）。暂时只停止移动并停止ST。
	void ArrivedDestination();

	//ws2-----------------------------
protected:
	UPROPERTY()
	TObjectPtr<AAppMarchRoute> MarchRoute = nullptr;

public:
	void SetMarchRoute(AAppMarchRoute* InMarchRoute);
	AAppMarchRoute* GetMarchRoute() const;
};
