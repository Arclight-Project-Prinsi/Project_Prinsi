// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AppGA_MeleeAttack_One.generated.h"

/**
 *
 */

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;


UCLASS()
class PROJECT_PRINSI_API UAppGA_MeleeAttack_One : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAppGA_MeleeAttack_One();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	)override;

	UFUNCTION()
	void OnAbilityMeleeStart();				// GAの開始処理(ロジック層)

	UFUNCTION()
	void OnAbilityMeleeFinished();			// GAの終了処理(ロジック層)

	//ws----------------------------------
protected:
	UFUNCTION()
	void OnAttackMontageCompleted();

	UFUNCTION()
	void OnAttackMontageInterrupted();

	UAnimMontage* GetCurrentAttackMontage() const;
	void AdvanceCombo();
	void ResetCombo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility|Melee|Config ")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;			// @todo 攻撃AM配列(DAから読み取る？)

	UPROPERTY(BlueprintReadOnly, Category = "GameplayAbility|Melee|Status")
	int32 CurrentComboIndex = 0;		// 現在のコンボ数

	// @note GA上にPlayMontageを1つのタスクとして認識される？
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>CurrentPlayMontageTask;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility|Melee|Config|Cost")
	float CostMana = 5.0f;

protected:
	// @note parameter
	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags
	)const override;

	// @note parameter
	virtual void ApplyCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	)const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameplayAbility|Function|Melee")
	void OnPreAbilityMelee(AActor* DashActor, const FVector& StartLocation);	// GAの開始処理(ヴィジュアル層)

	UFUNCTION(BlueprintImplementableEvent, Category = "GameplayAbility|Function|Melee")
	void OnPostAbilityMelee(AActor* DashActor, const FVector& EndLocation);		// GAの終了処理(ヴィジュアル層)

	//ws2----------------------------------------------------
//protected:
//	// @note GA中にもう一回起動されたら？
//	virtual void InputPressed(
//		const FGameplayAbilitySpecHandle Handle,
//		const FGameplayAbilityActorInfo* ActorInfo,
//		const FGameplayAbilityActivationInfo ActivationInfo
//	)override;

};
