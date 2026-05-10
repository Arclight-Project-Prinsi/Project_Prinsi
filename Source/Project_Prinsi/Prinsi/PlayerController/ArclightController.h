// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"					// ~~(Subsystem)输入映射配置
#include "GameFramework/PlayerController.h"
#include "ArclightController.generated.h"


UCLASS()
class PROJECT_PRINSI_API AArclightController : public APlayerController {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// ~~操作相关(Input)
protected:
	void OnMovePlayer(const FInputActionValue& Value);		// 主角移动
	virtual void SetupInputComponent()override;				// Input组件负责监听IA

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;			// 输入映射上下文

	// ~IA绑定
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	UInputAction* IA_MovePlayer;			// 主角移动
};
