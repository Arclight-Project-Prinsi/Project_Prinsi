// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AnimNotify/AppAnimNotify_ComboWindow.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"		


void UAppAnimNotify_ComboWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	// @memo Notify限定作用于基础角色
	AAppCharacterBase* Character = Cast<AAppCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetComboWindow(true);
	}
}