// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GameplayAbility/Player/AppGA_MeleeAttack_Two.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/GameplayAbility/AppCharacterAttributeSetBase.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"		// @todo
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"		// @note AbilityTask


UAppGA_MeleeAttack_Two::UAppGA_MeleeAttack_Two()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;		// @note Actorごと1個のインスタンス？
}

void UAppGA_MeleeAttack_Two::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// @note
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	UAnimMontage* MontageToPlay = AttackMontage;
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
			CurrentPlayMontageTask->OnCompleted.AddDynamic(this, &UAppGA_MeleeAttack_Two::OnAttackMontageCompleted);
			CurrentPlayMontageTask->OnInterrupted.AddDynamic(this, &UAppGA_MeleeAttack_Two::OnAttackMontageInterrupted);
			CurrentPlayMontageTask->OnCancelled.AddDynamic(this, &UAppGA_MeleeAttack_Two::OnAttackMontageInterrupted);

			// Abilityタスク準備OK
			CurrentPlayMontageTask->ReadyForActivation();
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

void UAppGA_MeleeAttack_Two::OnAbilityMeleeStart()
{
	if (CurrentActorInfo)
	{
		AActor* Avator = CurrentActorInfo->AvatarActor.Get();	// @note AvatorActorからGAの発動対象(Actor)を取得?
		FVector StartLocation = Avator ? Avator->GetActorLocation() : FVector::ZeroVector;
		OnPreAbilityMelee(Avator, StartLocation);
	}

	// @todo
	AAppCharacterBase* Character = Cast<AAppCharacterBase>(CurrentActorInfo->AvatarActor.Get());

	if (Character)
	{
		Character->SetComboIndex(2);				// @todo 设置为2连击
		Character->SetComboWindow(false);
		Character->SetComboInputBuffered(false);
	}
}

void UAppGA_MeleeAttack_Two::OnAbilityMeleeFinished()
{
	if (CurrentActorInfo)
	{
		AActor* Avator = CurrentActorInfo->AvatarActor.Get();	// @note AvatorActorからGAの発動対象(Actor)を取得?
		FVector EndLocation = Avator ? Avator->GetActorLocation() : FVector::ZeroVector;
		OnPostAbilityMelee(Avator, EndLocation);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAppGA_MeleeAttack_Two::OnAttackMontageCompleted()
{
	/*ResetCombo();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);*/

	AAppCharacterBase* Character =
		Cast<AAppCharacterBase>(
			CurrentActorInfo->AvatarActor.Get());

	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

		return;
	}

	//------------------------------------------------
	// 有二连击输入
	//------------------------------------------------

	if (Character->HasComboInputBuffered())
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

		if (ASC && NextComboAbility)
		{
			for (const FGameplayAbilitySpec& Spec :
				ASC->GetActivatableAbilities())
			{
				if (Spec.Ability && Spec.Ability->GetClass() == NextComboAbility)
				{
					ASC->TryActivateAbility(
						Spec.Handle);

					break;
				}
			}
		}
	}

	//------------------------------------------------
	// 没有二连击输入
	//------------------------------------------------

	Character->SetComboIndex(0);
	Character->SetComboWindow(false);
	Character->SetComboInputBuffered(false);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAppGA_MeleeAttack_Two::OnAttackMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool UAppGA_MeleeAttack_Two::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void UAppGA_MeleeAttack_Two::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
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
