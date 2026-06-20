// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/GameplayAbility/Player/AppGA_MeleeAttack_Three.h"
#include "Prinsi/Define/AppDefineDebug.h"
#include "Prinsi/GameplayAbility/AppCharacterAttributeSetBase.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"			// Actor_基础角色类
#include "AbilitySystemComponent.h"								// Component_ASC
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"		// AbilityTask_播放动画蒙太奇


UAppGA_MeleeAttack_Three::UAppGA_MeleeAttack_Three()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAppGA_MeleeAttack_Three::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// ~~Cost检查
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
		OnAbilityStart();

		// @note UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
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
			CurrentPlayMontageTask->OnCompleted.AddDynamic(this, &UAppGA_MeleeAttack_Three::OnAttackMontageCompleted);
			CurrentPlayMontageTask->OnInterrupted.AddDynamic(this, &UAppGA_MeleeAttack_Three::OnAttackMontageInterrupted);
			CurrentPlayMontageTask->OnCancelled.AddDynamic(this, &UAppGA_MeleeAttack_Three::OnAttackMontageInterrupted);
			// ~~激活Task
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

void UAppGA_MeleeAttack_Three::OnAbilityStart()
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
			// ~~设置Actor的攻击状态
			Character->SetComboIndex(2);				// @todo 设置为2连击
			Character->SetComboWindow(false);
			Character->SetComboInputBuffered(false);
		}
	}
}

void UAppGA_MeleeAttack_Three::OnAbilityFinished()
{
	// @todo 这里可以加入Ability结束前的处理

	Super::OnAbilityFinished();
}

void UAppGA_MeleeAttack_Three::OnAttackMontageCompleted()
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

	// @todo 暂时没有三连击
	// ~~A_三连击
	if (Character->HasComboInputBuffered())
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

		// @todo 检测Tag的存在
		if (ASC && NextComboAbilityTag.IsValid())
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(NextComboAbilityTag);

			Character->SetComboWindow(false);
			Character->SetComboInputBuffered(false);

			// @note 使用tag发动能力，都必须是“一箩筐”？
			const bool bActivated = ASC->TryActivateAbilitiesByTag(TagContainer);

			if (bActivated)
			{
				OnAbilityFinished();
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
				return;
			}
		}
	}

	// ~~B_无三连击输入
	Character->SetComboIndex(0);
	Character->SetComboWindow(false);
	Character->SetComboInputBuffered(false);

	OnAbilityFinished();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAppGA_MeleeAttack_Three::OnAttackMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

bool UAppGA_MeleeAttack_Three::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void UAppGA_MeleeAttack_Three::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
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
