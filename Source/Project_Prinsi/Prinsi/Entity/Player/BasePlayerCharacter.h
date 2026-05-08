// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Prinsi/Component/EntityComponent.h"		// Entity组件
#include "BasePlayerCharacter.generated.h"

UCLASS()
class PROJECT_PRINSI_API ABasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//ws----------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEntityComponent* EntityComponent_;		// 创建Entity组件

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity")
	UDataTable* EntityBaseTable_;			// 实体主表

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config Entity|Player")
	UDataTable* PlayerExtraTable_;			// Player类拓展配置表

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Player")
	int32 Health_ = 42;						// 生命值

protected:
	// 读取配置表进行初始化
	bool InitFromConfig(FName EntityId);

	// 读取配置表实现Player初始化
	bool InitPlayerFromConfig(const FEntityPlayerExtraConfig* PlayerConfig);
};
