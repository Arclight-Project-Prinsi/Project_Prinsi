// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GameplayAbility/Player/AppGA_MeleeAttack_One.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/GameplayAbility/AppCharacterAttributeSetBase.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"		// @note AbilityTask


UAppGA_MeleeAttack_One::UAppGA_MeleeAttack_One()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;		// @note Actorごと1個のインスタンス？
}

void UAppGA_MeleeAttack_One::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// @note
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	UAnimMontage* MontageToPlay = GetCurrentAttackMontage();
	if (Character && MontageToPlay)
	{
		OnAbilityMeleeStart();

		// @note UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
		CurrentPlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("PlayAttackMontageTask"),
			MontageToPlay,
			1.0f,		// Animation play rate
			NAME_None,	// 最初から
			false,
			1.0f		// Blend Outの時間
		);

		// @note OnCompleted/OnInterrupted/OnCancelled Abilityタスクの特性？
		if (CurrentPlayMontageTask)
		{
			CurrentPlayMontageTask->OnCompleted.AddDynamic(this, &UAppGA_MeleeAttack_One::OnAttackMontageCompleted);
			CurrentPlayMontageTask->OnInterrupted.AddDynamic(this, &UAppGA_MeleeAttack_One::OnAttackMontageInterrupted);
			CurrentPlayMontageTask->OnCancelled.AddDynamic(this, &UAppGA_MeleeAttack_One::OnAttackMontageInterrupted);

			// Abilityタスク準備OK
			CurrentPlayMontageTask->ReadyForActivation();

			// コンボ数を増やす
			AdvanceCombo();
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UAppGA_MeleeAttack_One::OnAbilityMeleeStart()
{
	if (CurrentActorInfo)
	{
		AActor* Avator = CurrentActorInfo->AvatarActor.Get();	// @note AvatorActorからGAの発動対象(Actor)を取得?
		FVector StartLocation = Avator ? Avator->GetActorLocation() : FVector::ZeroVector;
		OnPreAbilityMelee(Avator, StartLocation);
	}
}

void UAppGA_MeleeAttack_One::OnAbilityMeleeFinished()
{
	if (CurrentActorInfo)
	{
		AActor* Avator = CurrentActorInfo->AvatarActor.Get();	// @note AvatorActorからGAの発動対象(Actor)を取得?
		FVector EndLocation = Avator ? Avator->GetActorLocation() : FVector::ZeroVector;
		OnPostAbilityMelee(Avator, EndLocation);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAppGA_MeleeAttack_One::OnAttackMontageCompleted()
{
	ResetCombo();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAppGA_MeleeAttack_One::OnAttackMontageInterrupted()
{
	ResetCombo();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

UAnimMontage* UAppGA_MeleeAttack_One::GetCurrentAttackMontage() const
{
	if (AttackMontages.Num() > 0 && AttackMontages.IsValidIndex(CurrentComboIndex))
	{
		return AttackMontages[CurrentComboIndex];
	}

	return nullptr;
}

void UAppGA_MeleeAttack_One::AdvanceCombo()
{
	if (AttackMontages.Num() > 0)
	{
		CurrentComboIndex = (CurrentComboIndex + 1) % AttackMontages.Num();
	}
}

void UAppGA_MeleeAttack_One::ResetCombo()
{
	CurrentComboIndex = 0;
}

bool UAppGA_MeleeAttack_One::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		// @note Attribute set
		const UAppCharacterAttributeSetBase* AttributeSet =
			Cast<UAppCharacterAttributeSetBase>(ActorInfo->AbilitySystemComponent->GetAttributeSet(UAppCharacterAttributeSetBase::StaticClass()));
		if (AttributeSet)
		{
			// @memo ATTRIBUTE_ACCESSORS_BASICで定義した
			float CurMana = AttributeSet->GetMana();

			return CurMana >= CostMana;	// コストチェック
		}

	}
	return false;
}

void UAppGA_MeleeAttack_One::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		// @note const_cast&&Cast
		UAppCharacterAttributeSetBase* AttributeSet =
			const_cast<UAppCharacterAttributeSetBase*>(Cast<UAppCharacterAttributeSetBase>(ActorInfo->AbilitySystemComponent->GetAttributeSet(UAppCharacterAttributeSetBase::StaticClass())));
		if (AttributeSet)
		{
			// @memo ATTRIBUTE_ACCESSORS_BASICで定義した
			float NewMana = AttributeSet->GetMana() - CostMana;

			// @note Attributeデータを書き込む
			ActorInfo->AbilitySystemComponent->SetNumericAttributeBase(AttributeSet->GetManaAttribute(), NewMana);
		}

	}
}

// @todo
//void UAppGA_MeleeAttack_One::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	//[p]
//	APP_SCREEN_ERROR(TEXT("aaaaa"));
//}
