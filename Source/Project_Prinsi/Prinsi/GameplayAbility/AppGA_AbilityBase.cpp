// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GameplayAbility/AppGA_AbilityBase.h"
#include "GameFramework/Character.h"


void UAppGA_AbilityBase::OnAbilityStart()
{
	if (CurrentActorInfo)
	{
		AActor* Avator = CurrentActorInfo->AvatarActor.Get();
		FVector StartLocation = Avator ? Avator->GetActorLocation() : FVector::ZeroVector;
		BP_OnAbilityStart(Avator, StartLocation);
	}
}

void UAppGA_AbilityBase::OnAbilityFinished()
{
	if (CurrentActorInfo)
	{
		AActor* Avator = CurrentActorInfo->AvatarActor.Get();	
		FVector EndLocation = Avator ? Avator->GetActorLocation() : FVector::ZeroVector;
		BP_OnAbilityFinished(Avator, EndLocation);
	}

}

