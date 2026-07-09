// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "AbilitySystemComponent.h"			// Component_ASC

#include "AppCharacterAttributeSetBase.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_PRINSI_API UAppCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAppCharacterAttributeSetBase();

	// @todo 网络部分之后再纠结。

	// @note（ReplicatedUsing）:告知服务器，发生Replication（即数据更新）时就调用这个函数。
	// @memo（ATTRIBUTE_ACCESSORS_BASIC） 这个宏为Attribute提供了访问函数。
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAttributes")
	FGameplayAttributeData Health;			// 生命值
	ATTRIBUTE_ACCESSORS_BASIC(UAppCharacterAttributeSetBase, Health);

	UPROPERTY(BlueprintReadOnly, Category = "CharacterAttributes")
	FGameplayAttributeData MaxHealth;		// 最大生命值
	ATTRIBUTE_ACCESSORS_BASIC(UAppCharacterAttributeSetBase, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "CharacterAttributes")
	FGameplayAttributeData Mana;			// 法力值
	ATTRIBUTE_ACCESSORS_BASIC(UAppCharacterAttributeSetBase, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "CharacterAttributes")
	FGameplayAttributeData MaxMana;			// 最大法力值
	ATTRIBUTE_ACCESSORS_BASIC(UAppCharacterAttributeSetBase, MaxMana);
};
