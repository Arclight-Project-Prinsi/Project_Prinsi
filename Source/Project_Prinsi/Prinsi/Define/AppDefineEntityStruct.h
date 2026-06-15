// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AppDefineEntityStruct.generated.h"


// ~~(枚举)Entity类型枚举
UENUM(BlueprintType)
enum class EEntityType :uint8 {
	EntityNone	UMETA(DisplayName = "None"),
	EntityTower	UMETA(DisplayName = "Tower"),
	EntityPlayer UMETA(DisplayName = "Player")
};

// ~~(枚举)Entity阵营类型
UENUM(BlueprintType)
enum class EEntityFaction : uint8 {
	FactionNone UMETA(DisplayName = "None"),
	FactionPlayer UMETA(DisplayName = "Player"),
	FactionEnemy UMETA(DisplayName = "Enemy"),
	FactionNeutral UMETA(DisplayName = "Neutral")
};

// ~~Entity主表
USTRUCT(BlueprintType)
struct  FEntityBaseConfig :public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EntityId;					// [TODO]实体ID（读表时蓝图的EntityId会与RawName匹配，这个字段暂时没意义）

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEntityType EntityType;			// 实体类型(通过这个判断读哪个扩展表)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEntityFaction EntityFaction;	// 实体阵营
};

// ~~Tower扩展表
USTRUCT(BlueprintType)
struct  FEntityTowerExtraConfig :public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EntityId;					// [TODO]实体ID（读表时蓝图的EntityId会与RawName匹配，这个字段暂时没意义）

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BuildCost = 42;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint FootprintSize = { -1,-1 };		// 占地
};

// ~~Player扩展表
USTRUCT(BlueprintType)
struct  FEntityPlayerExtraConfig :public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EntityId;					// [TODO]实体ID（读表时蓝图的EntityId会与RawName匹配，这个字段暂时没意义）

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 10.0f;
};


//ws------------------------------------------------------

class PROJECT_PRINSI_API AppDefineEntityStruct {
public:
	AppDefineEntityStruct();
	~AppDefineEntityStruct();
};

