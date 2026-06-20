// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"				// GAS的tag容器
#include "GameFramework/Character.h"
#include "AppCharacterBase.generated.h"

//――――――――――――――――――――
// 前向声明
//――――――――――――――――――――
class UAbilitySystemComponent;
class UGameplayAbility;
class UAppCharacterAttributeSetBase;		// AttributeSet
// @note 是否需要使用GE?
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


	//――――――――――――――――――――
	// GAS部分
	//――――――――――――――――――――
public:
	UAbilitySystemComponent* GetAbilitySystemComponent()const { return AbilitySystemComponent; }
	UAppCharacterAttributeSetBase* GetAttributeSet() const { return AttributeSet; }

protected:
	void InitAbilitySystem();			// 初始化AS & 初始化GA
	void GiveDeafultAbilities();
	void InitDefaultAttributeSet();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Actor Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	// Actor Component_ASC组件		

	// @todo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Config|GAS")
	TObjectPtr<UAppCharacterAttributeSetBase> AttributeSet;		// AS（GAS属性管理）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Config|GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;		// 初始GA



	//ws1---------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Config|GAS")
	TSubclassOf<UGameplayEffect>InitialAttributeEffect;			// GE_Attribute初期化用
};
