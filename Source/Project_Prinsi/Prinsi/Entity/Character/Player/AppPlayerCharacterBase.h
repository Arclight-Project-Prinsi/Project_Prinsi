// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Prinsi/Component/EntityComponent.h"			// Component Actor_实体组件
#include "GameFramework/SpringArmComponent.h"			// Component Scene_弹簧臂组件
#include "Camera/CameraComponent.h"						// Component Scene_相机组件
#include "GameFramework/Character.h"
#include "Prinsi/Entity/Character/AppCharacterBase.h"	// Character_GAS角色基类
#include "AppPlayerCharacterBase.generated.h"

//――――――――――――――――――――
// 前向声明
//――――――――――――――――――――
class UGameplayAbility;


UCLASS()
class PROJECT_PRINSI_API AAppPlayerCharacterBase : public AAppCharacterBase {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAppPlayerCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|PlayerCharacterBase|Scene Component")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|PlayerCharacterBase|Scene Component")
	TObjectPtr<UCameraComponent> TopDownCamera;

	//――――――――――――――――――――
	// Entity初期配置
	//――――――――――――――――――――
	// 读取配置表进行初始化
	bool InitFromConfig(FName EntityId);

	// 读取配置表实现Player初始化
	bool InitPlayerFromConfig(const FEntityPlayerExtraConfig* PlayerConfig);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|PlayerCharacterBase|Actor Components")
	TObjectPtr<UEntityComponent> EntityComp;			// Actor Component_实体组件

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|PlayerCharacterBase|Entity|Config")
	TObjectPtr<UDataTable> EntityBaseTable;				// 实体主表		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|PlayerCharacterBase|Entity|Config")
	TObjectPtr<UDataTable> PlayerExtraTable;			// Player类拓展配置表


	//ws-----------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase|PlayerCharacterBase|Config|GAS|Attack")
	TSubclassOf<UGameplayAbility> AttackAbilityOne;		// 连击GA1

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase|PlayerCharacterBase|Config|GAS|Attack")
	TSubclassOf<UGameplayAbility> AttackAbilityTwo;		// 连击GA2

protected:
	UFUNCTION(BlueprintCallable)
	void AttackInput();
};
