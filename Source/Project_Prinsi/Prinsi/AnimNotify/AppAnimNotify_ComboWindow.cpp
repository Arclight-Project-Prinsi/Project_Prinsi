// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/AnimNotify/AppAnimNotify_ComboWindow.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"		// @todo

void UAppAnimNotify_ComboWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// @todo
	APP_SCREEN_ERROR(TEXT("TEST-5"));

	// @memo Notify限定作用于基础角色
	AAppCharacterBase* Character = Cast<AAppCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		// @todo
		APP_SCREEN_ERROR(TEXT("TEST-A-2"));

		Character->SetComboWindow(true);
	}
}