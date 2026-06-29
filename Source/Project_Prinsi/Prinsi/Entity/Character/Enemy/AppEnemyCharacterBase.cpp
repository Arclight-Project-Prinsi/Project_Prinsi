// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Character/Enemy/AppEnemyCharacterBase.h"


AAppEnemyCharacterBase::AAppEnemyCharacterBase()
{
	// @memo 生成之后立刻启动AIController
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAppEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AddDeadTag();
}
