#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EntityConfigStructs.generated.h"

/*
	//GEngine的写法
	if (GEngine){
		GEngine->AddOnScreenDebugMessage(
			-1,							// Key: 消息 ID
			5.0f,						// Duration: 显示时长（秒）
			FColor::Cyan,				// DisplayColor: 颜色
			TEXT("塔防系统已激活！")	// DebugMessage: 字符串内容
		);
	}

		if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("塔防系统已激活！"));}

*/

//[P]Entity类型枚举
UENUM(BlueprintType)
enum class EEntityType :uint8
{
	Tower	UMETA(DisplayName = "Tower")
};

//[p]Entity主表
USTRUCT(BlueprintType)
struct  FEntityBaseConfig :public FTableRowBase
{
	GENERATED_BODY()

	//ws------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EntityId;				// 实体ID

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEntityType EEntityType;	// 实体类型(通过这个判断读哪个扩展表)
};

//[p]Tower扩展表
USTRUCT(BlueprintType)
struct  FEntityTowerExtraConfig :public FTableRowBase
{
	GENERATED_BODY()

	//ws------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EntityId;				// 实体ID

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BuildCost = 42;
};

