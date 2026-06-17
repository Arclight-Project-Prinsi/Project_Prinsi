// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include "GameplayTagContainer.h"			// @note 

#include "GameFramework/Character.h"
#include "AppCharacterBase.generated.h"

//――――――――――――――――――――
// 前向声明
//――――――――――――――――――――
class UAbilitySystemComponent;
class UGameplayAbility;
class UAppBaseAttributeSet;
// @note
class UGameplayEffect;


UCLASS()
class PROJECT_PRINSI_API AAppCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAppCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ws----------------------------
public:
	UAbilitySystemComponent* GetAbilitySystemComponent()const { return AbilitySystemComponent; }
	// @todo
	//UAppBaseAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	void InitAbilitySystem();
	void GiveDeafultAbilities();
	void InitDefaultAttributeSet();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component|Actor Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	// Actor Component_GA管理组件		

	// @todo
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Status|GAS")
	//TObjectPtr<UAppBaseAttributeSet> AttributeSet;				// Attribute

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Config|GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;		// デフォルトのGA

	//ws1---------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Config|GAS")
	TSubclassOf<UGameplayEffect>InitialAttributeEffect;			// GE_Attribute初期化用
};
