// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"			// ~~(Subsystem)输入映射配置
#include "GameFramework/PlayerController.h"
#include "ArclightController.generated.h"


class AArclightGridTileManager;		// 逻辑格子管理器
class ATowerBase;					// 塔基类


// ~~玩家的行动模式
UENUM(BlueprintType)
enum class EPlayerOperationMode : uint8 {
	Normal     UMETA(DisplayName = "Normal"),
	Placement  UMETA(DisplayName = "Placement"),
};


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
	UPROPERTY(EditAnywhere, Category = "Input Controller")
	UInputMappingContext* DefaultMappingContext_;			// 输入映射上下文

	// ~IA绑定
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input Controller|Action")
	UInputAction* IA_MovePlayer_;			// 主角移动

	UPROPERTY(EditDefaultsOnly, Category = "Input Controller|Action")
	UInputAction* IA_EnterPlacementMode_;	// 进入建造模式

	//ws--------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, Category = "State Controller")
	EPlayerOperationMode OperationMode_ = EPlayerOperationMode::Normal;		// 当前玩家的控制状态

protected:
	UPROPERTY(EditAnywhere, Category = "Ref Controller|Placement")
	TSubclassOf<ATowerBase> CurrentPlacementTowerClass_;	// 当前选中塔的类型	//[TODO]出于测试方便硬编码

	UPROPERTY(EditAnywhere, Category = "State Controller|Placement")
	FIntPoint CurrentPlacementFootprint_ = FIntPoint(2, 2);	// 当前部署所需占地	//[TODO]出于测试方便硬编码

protected:
	UPROPERTY(EditAnywhere, Category = "Ref Controller|Placement")
	AArclightGridTileManager* GridManager_;		// 逻辑格子管理器指针

protected:
	void OnEnterPlacementMode();		// 进入建造模式

	//ws2--------------------------------------------
protected:
	UPROPERTY()
	ATowerBase* CurrentPreviewTower_ = nullptr;		// 当前预览体

	UPROPERTY(EditAnywhere, Category = "Ref Controller|Placement")
	UMaterialInterface* BuildablePreviewMaterial_;

	UPROPERTY(EditAnywhere, Category = "Ref Controller|Placement")
	UMaterialInterface* UnbuildablePreviewMaterial_;

protected:
	void CreatePlacementPreviewActor();			// 创建预览体
	void DestroyPlacementPreviewActor();			// 销毁预览体
	void UpdatePlacementPreviewActor();			// 更新预览体
	void SetPreviewTowerVisible(bool bVisible);		// 设置预览体可视性

protected:
	void SetPreviewTowerMaterial(UMaterialInterface* Material);
};
