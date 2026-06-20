// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Character/AppCharacterBase.h"
#include "Prinsi\GameplayAbility\AppCharacterAttributeSetBase.h"	// AttributeSet
#include "AbilitySystemComponent.h"									// Component_Actor_ASC组件
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"											// @note GameplayEffect


// Sets default values
AAppCharacterBase::AAppCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UAppCharacterAttributeSetBase>(TEXT("AttributeSet"));

	// Actor Component Init_移动组件
	// @memo 玩家朝向根据控制器而非移动组件
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;		// Pawn朝向根据移动组件方向决定
		GetCharacterMovement()->bUseControllerDesiredRotation = false;	// Pawn朝向不根据Controller旋转
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	}

	// @todo 控制器影响玩家Yaw旋转（AI控制器呢？）
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

	// @note 目前是通过读表初始化（是否有必要使用GE初始化？）
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

