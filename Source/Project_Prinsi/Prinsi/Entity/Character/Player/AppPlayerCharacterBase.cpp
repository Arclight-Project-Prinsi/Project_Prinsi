// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/Entity/Character/Player/AppPlayerCharacterBase.h"
#include "Prinsi/Define/AppDefineDebug.h"					// Define_Debug工具


AAppPlayerCharacterBase::AAppPlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 初始化组件
	EntityComp = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	// Scene Component Init_弹簧臂
	{
		SpringArmComp->SetupAttachment(RootComponent);
		SpringArmComp->TargetArmLength = 1400.0f;
		SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
		// @memo 因为玩家角色不通过Controlelr旋转，该设置没意义
		SpringArmComp->bUsePawnControlRotation = false;

		// @todo
		SpringArmComp->SetUsingAbsoluteRotation(true);
	}

	// Scene Component Init_摄像机
	{
		TopDownCamera->SetupAttachment(SpringArmComp);
		TopDownCamera->bUsePawnControlRotation = false;
		TopDownCamera->SetFieldOfView(55.0f);			// 视场角(TopDown不需要广角)
	}
}

// Called when the game starts or when spawned
void AAppPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// ~~通过EntityId进行初始化(主表+扩展表)
	if (!EntityComp)
	{
		APP_ERROR(TEXT("Entity组件为空")); return;
	}
	if (!InitFromConfig(EntityComp->GetEntityId()))
	{
		APP_ERROR(TEXT("通过EntityId进行初始化失败")); return;
	}
}

// Called every frame
void AAppPlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAppPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AAppPlayerCharacterBase::InitFromConfig(FName Id)
{
	// ~~读取主表
	if (!EntityBaseTable)
	{
		APP_ERROR(TEXT("EntityBaseTable为空(BasePlayerCharacter.cpp)"));
		return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable->FindRow<FEntityBaseConfig>(Id, TEXT("无法找到对应Raw"));
	if (!BaseConfig)
	{
		APP_ERROR(TEXT("无法找到对应EntityBase配置数据, EntityId = %s"), *Id.ToString()); return false;
	}

	// ~~确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::EntityPlayer)
	{
		APP_ERROR(TEXT(" EntityType不匹配,EntityId = %s"), *Id.ToString()); return false;
	}

	// ~~Entity组件初始化
	if (!EntityComp->InitFromConfig(BaseConfig))
	{
		APP_ERROR(TEXT("Entity组件初始化失败")); return false;
	}

	// ~~读取Player扩展表(Player)
	if (!PlayerExtraTable)
	{
		APP_ERROR(TEXT("PlayerExtraTable为空")); return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityPlayerExtraConfig* PlayerConfig =
		PlayerExtraTable->FindRow<FEntityPlayerExtraConfig>(Id, TEXT("无法找到PlayerExtra!"));
	if (!PlayerConfig)
	{
		APP_ERROR(TEXT("无法找到对应Player配置数据, EntityId = %s"), *Id.ToString()); return false;
	}

	// ~~Player字段初始化
	if (!InitPlayerFromConfig(PlayerConfig))
	{
		APP_ERROR(TEXT("Player扩展字段初始化失败")); return false;
	}

	return true;
}

bool AAppPlayerCharacterBase::InitPlayerFromConfig(const FEntityPlayerExtraConfig* PlayerConfig)
{
	if (!PlayerConfig)
	{
		return false;
	}

	return true;
}