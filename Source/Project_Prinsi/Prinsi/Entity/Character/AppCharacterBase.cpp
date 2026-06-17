// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Character/AppCharacterBase.h"

#include "AbilitySystemComponent.h"		// Actor Component_GAS组件
#include "Prinsi\GameplayAbility\AppCharacterAttributeSetBase.h"	// AttributeSet
#include "GameplayEffect.h"				// GameplayEffect


// Sets default values
AAppCharacterBase::AAppCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UAppCharacterAttributeSetBase>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AAppCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// GA初始化 & Attribute初始化
	InitAbilitySystem();
}

// Called every frame
void AAppCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAppCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAppCharacterBase::InitAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		// @note Avatar?（目前Owner与Avator应该都是角色自身）
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		GiveDeafultAbilities();			// GA初始化
		InitDefaultAttributeSet();		// Attribute初始化
	}
}

void AAppCharacterBase::GiveDeafultAbilities()
{
	if (!AbilitySystemComponent) return;

	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities) {
		if (AbilityClass)
		{
			// @note FGameplayAbilitySpec
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
		}
	}
}

void AAppCharacterBase::InitDefaultAttributeSet()
{
	if (!AbilitySystemComponent || !InitialAttributeEffect)return;

	// @note 
	//FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	//// @note 
	//EffectContext.AddSourceObject(this);
	//// @note 
	//FGameplayEffectSpecHandle SpecHandle =
	//	AbilitySystemComponent->MakeOutgoingSpec(InitialAttributeEffect, 1.0f, EffectContext);
	//if (SpecHandle.IsValid()) {
	//	// @note 
	//	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	//}
}

