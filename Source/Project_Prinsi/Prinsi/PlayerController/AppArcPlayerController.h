// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include "EnhancedInputSubsystems.h"				// Subsystem_输入映射配置相关

#include "GameFramework/PlayerController.h"
#include "AppArcPlayerController.generated.h"


//――――――――――――――――――――
// 前向声明
//――――――――――――――――――――
class AAppGridTileManager;		// 逻辑格子管理器
class AAppTowerBase;			// 塔基类

//――――――――――――――――――――
// 枚举
//――――――――――――――――――――
// ~玩家控制状态
UENUM(BlueprintType)
enum class EOperationMode : uint8 {
	Normal     UMETA(DisplayName = "Normal"),
	Placement  UMETA(DisplayName = "Placement"),
};


UCLASS()
class PROJECT_PRINSI_API AAppArcPlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//――――――――――――――――――――
	// Misc
	//――――――――――――――――――――
protected:
	UPROPERTY(VisibleAnywhere, Category = "ArcPlayerController|Status|Operation")
	EOperationMode OperationMode = EOperationMode::Normal;	// 当前玩家的控制状态

	//――――――――――――――――――――
	// 操作输入相关(Input)
	//――――――――――――――――――――
protected:
	virtual void SetupInputComponent()override;				// Input组件负责监听IA

protected:
	void OnMovePlayer(const FInputActionValue& Value);		// 主角移动

protected:
	UPROPERTY(EditAnywhere, Category = "ArcPlayerController|Config|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;	// 输入映射上下文

	// ~IA绑定
protected:
	UPROPERTY(EditDefaultsOnly, Category = "ArcPlayerController|Config|Input")
	TObjectPtr<UInputAction> IA_MovePlayer;			// 主角移动

	UPROPERTY(EditDefaultsOnly, Category = "ArcPlayerController|Config|Input")
	TObjectPtr<UInputAction> IA_EnterPlacementMode;	// 进入建造模式



	//ws--------------------------------------------
	//――――――――――――――――――――
	// AppSystem_Placement（预览体＆塔部署）
	//――――――――――――――――――――
protected:
	UPROPERTY(EditAnywhere, Category = "ArcPlayerController|Status|Placement")
	TSubclassOf<AAppTowerBase> CurrentPlacementTowerClass;	// 当前选中塔的类型	// @todo 出于测试方便硬编码（之后在runtime中赋值）

protected:
	UPROPERTY(EditAnywhere, Category = "ArcPlayerController|Status|Placement")
	TObjectPtr<AAppGridTileManager> GridManager;			// 逻辑格子管理器指针

protected:
	void OnEnterPlacementMode();		// 进入建造模式

	//ws2--------------------------------------------
protected:
	UPROPERTY()
	TObjectPtr<AAppTowerBase> CurrentPreviewTower = nullptr;	// 当前预览体对象

	UPROPERTY(VisibleAnywhere, Category = "ArcPlayerController|Status|Placement")
	FIntPoint CurrentPlacementFootprint = FIntPoint(-1, -1);	// 占地（当前预览体）

	UPROPERTY(EditAnywhere, Category = "ArcPlayerController|Config|Placement")
	TObjectPtr<UMaterialInterface> BuildablePreviewMaterial;

	UPROPERTY(EditAnywhere, Category = "ArcPlayerController|Config|Placement")
	TObjectPtr<UMaterialInterface> UnbuildablePreviewMaterial;

protected:
	void CreatePlacementPreviewActor();								// 创建预览体
	void DestroyPlacementPreviewActor();							// 销毁预览体
	void UpdatePlacementPreviewActor();								// 更新预览体
	void SetPreviewTowerVisible(bool bVisible);						// 设置预览体可视性
	void SetPreviewTowerMaterial(UMaterialInterface* Material);		// 设置预览体材质
};
