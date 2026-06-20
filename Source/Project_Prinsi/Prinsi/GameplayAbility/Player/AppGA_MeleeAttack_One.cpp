// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GameplayAbility/Player/AppGA_MeleeAttack_One.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/GameplayAbility/AppCharacterAttributeSetBase.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"			// Actor_基础角色类
#include "AbilitySystemComponent.h"								// Component_ASC
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"		// AbilityTask_播放动画蒙太奇


UAppGA_MeleeAttack_One::UAppGA_MeleeAttack_One()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAppGA_MeleeAttack_One::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// ~~Cost检查
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	UAnimMontage* MontageToPlay = AttackMontage;
	if (Character && MontageToPlay)
	{
		OnAbilityStart();

		// ~~创建Task
		CurrentPlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("PlayAttackMontageTask"),
			MontageToPlay,
			1.0f,			// AM的播放速率
			NAME_None,		// AM从头播放
			false,
			1.0f			// 混出(Blendout)时间
		);

		if (CurrentPlayMontageTask)
		{
			// ~~绑定Delegate
			CurrentPlayMontageTask->OnCompleted.AddDynamic(this, &UAppGA_MeleeAttack_One::OnAttackMontageCompleted);
			CurrentPlayMontageTask->OnInterrupted.AddDynamic(this, &UAppGA_MeleeAttack_One::OnAttackMontageInterrupted);
			CurrentPlayMontageTask->OnCancelled.AddDynamic(this, &UAppGA_MeleeAttack_One::OnAttackMontageInterrupted);
			// ~~激活Task
			CurrentPlayMontageTask->ReadyForActivation();
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UAppGA_MeleeAttack_One::OnAbilityStart()
{
	Super::OnAbilityStart();

	if (!CurrentActorInfo)
	{
		return;
	}

	if (AActor* Avatar = CurrentActorInfo->AvatarActor.Get())
	{
		// ~~设置Actor的攻击状态
		if (AAppCharacterBase* Character = Cast<AAppCharacterBase>(Avatar))
		{
			Character->SetComboIndex(1);
			Character->SetComboWindow(false);
			Character->SetComboInputBuffered(false);
		}
	}
}

void UAppGA_MeleeAttack_One::OnAbilityFinished()
{
	// @todo 这里可以加入Ability结束前的处理

	Super::OnAbilityFinished();
}

void UAppGA_MeleeAttack_One::OnAttackMontageCompleted()
{
	if (!CurrentActorInfo)
	{
		return;
	}

	AAppCharacterBase* Character = Cast<AAppCharacterBase>(CurrentActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// ~~A_二连击
	if (Character->HasComboInputBuffered())
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		if (ASC && NextComboAbility)
		{
			// @memo ASC中的Ability都是Spec，其中包含Ability本体，以及Level等许多信息。
			for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
			{
				if (Spec.Ability && Spec.Ability->GetClass()->IsChildOf(NextComboAbility))
				{
					// @todo 这个处理合并一下？
					Character->SetComboWindow(false);
					Character->SetComboInputBuffered(false);

					const bool bActivated = ASC->TryActivateAbility(Spec.Handle);

					if (bActivated)
					{
						OnAbilityFinished();
						EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
						return;
					}

					break;
				}
			}
		}
	}

	// ~~B_无二连击输入
	Character->SetComboIndex(0);
	Character->SetComboWindow(false);
	Character->SetComboInputBuffered(false);

	OnAbilityFinished();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAppGA_MeleeAttack_One::OnAttackMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

bool UAppGA_MeleeAttack_One::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		const UAppCharacterAttributeSetBase* AttributeSet =
			Cast<UAppCharacterAttributeSetBase>(ActorInfo->AbilitySystemComponent->GetAttributeSet(UAppCharacterAttributeSetBase::StaticClass()));
		if (AttributeSet)
		{
			// ~~Cost检查_Mana
			float CurMana = AttributeSet->GetMana();
			return CurMana >= CostMana;
		}

	}
	return false;
}

void UAppGA_MeleeAttack_One::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		UAppCharacterAttributeSetBase* AttributeSet =
			const_cast<UAppCharacterAttributeSetBase*>(Cast<UAppCharacterAttributeSetBase>(ActorInfo->AbilitySystemComponent->GetAttributeSet(UAppCharacterAttributeSetBase::StaticClass())));
		if (AttributeSet)
		{
			// ~~Cost消耗_Mana
			float NewMana = AttributeSet->GetMana() - CostMana;
			ActorInfo->AbilitySystemComponent->SetNumericAttributeBase(AttributeSet->GetManaAttribute(), NewMana);
		}

	}
}
