// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include "Prinsi/Component/EntityComponent.h"		// Actor Component_实体组件
#include "GameFramework/SpringArmComponent.h"		// Scene Component_弹簧臂组件
#include "Camera/CameraComponent.h"					// Scene Component_相机组件

#include "GameFramework/Character.h"
#include "AppPlayerCharacterBase.generated.h"


UCLASS()
class PROJECT_PRINSI_API AAppPlayerCharacterBase : public ACharacter {
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

	//ws--------------------------------------------------------
	//――――――――――――――――――――
	// Entity初期配置
	//――――――――――――――――――――
	// 读取配置表进行初始化
	bool InitFromConfig(FName EntityId);

	// 读取配置表实现Player初始化
	bool InitPlayerFromConfig(const FEntityPlayerExtraConfig* PlayerConfig);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Components")
	TObjectPtr<UEntityComponent> EntityComp;			// Actor Component_实体组件

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	TObjectPtr<UDataTable> EntityBaseTable;			// 实体主表		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity|Player")
	TObjectPtr<UDataTable> PlayerExtraTable;			// Player类拓展配置表

	//ws0-------------------------------
	// ~~Status
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Player")
	int32 Health_ = 42;						// 生命值

	//ws----------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene Component")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene Component")
	TObjectPtr<UCameraComponent> TopDownCamera;
};
