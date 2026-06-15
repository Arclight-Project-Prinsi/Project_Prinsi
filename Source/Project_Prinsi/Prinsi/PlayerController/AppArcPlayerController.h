// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"				// Subsystem_输入映射配置相关
#include "GameFramework/PlayerController.h"
#include "AppArcPlayerController.generated.h"


// ~~前向声明
class AArclightGridTileManager;		// 逻辑格子管理器
class AAppTowerBase;					// 塔基类

// ~~Enum_玩家控制状态
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
	UPROPERTY(VisibleAnywhere, Category = "Status Controller|Operation")
	EOperationMode OperationMode = EOperationMode::Normal;		// 当前玩家的控制状态

	//――――――――――――――――――――
	// 操作输入相关(Input)
	//――――――――――――――――――――
protected:
	virtual void SetupInputComponent()override;				// Input组件负责监听IA

protected:
	void OnMovePlayer(const FInputActionValue& Value);		// 主角移动

protected:
	UPROPERTY(EditAnywhere, Category = "Config Controller|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;			// 输入映射上下文

	// ~IA绑定
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config Controller|Input")
	TObjectPtr<UInputAction> IA_MovePlayer;			// 主角移动

	UPROPERTY(EditDefaultsOnly, Category = "Config Controller|Input")
	TObjectPtr<UInputAction> IA_EnterPlacementMode;	// 进入建造模式



	//ws--------------------------------------------
	//――――――――――――――――――――
	// AppSystem_Placement（预览体＆塔部署）
	//――――――――――――――――――――
protected:
	UPROPERTY(EditAnywhere, Category = "AppSystem|Placement")
	TSubclassOf<AAppTowerBase> CurrentPlacementTowerClass;		// 当前选中塔的类型	//[TODO]出于测试方便硬编码

protected:
	UPROPERTY(EditAnywhere, Category = "AppSystem|Placement")
	TObjectPtr<AArclightGridTileManager> GridManager;		// 逻辑格子管理器指针

protected:
	void OnEnterPlacementMode();		// 进入建造模式

	//ws2--------------------------------------------
protected:
	UPROPERTY()
	TObjectPtr<AAppTowerBase> CurrentPreviewTower = nullptr;	// 当前预览体对象

	UPROPERTY(VisibleAnywhere, Category = "AppSystem|Placement")
	FIntPoint CurrentPlacementFootprint = FIntPoint(-1, -1);	// 占地（当前预览体）

	UPROPERTY(EditAnywhere, Category = "AppSystem|Placement")
	TObjectPtr<UMaterialInterface> BuildablePreviewMaterial;

	UPROPERTY(EditAnywhere, Category = "AppSystem|Placement")
	TObjectPtr<UMaterialInterface> UnbuildablePreviewMaterial;

protected:
	void CreatePlacementPreviewActor();								// 创建预览体
	void DestroyPlacementPreviewActor();							// 销毁预览体
	void UpdatePlacementPreviewActor();								// 更新预览体
	void SetPreviewTowerVisible(bool bVisible);						// 设置预览体可视性
	void SetPreviewTowerMaterial(UMaterialInterface* Material);		// 设置预览体材质
};
